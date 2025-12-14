// OcrComposePanel.cpp
#include "OcrComposePanel.h"
#include <wx/app.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include <wx/splitter.h> // 用于切分视图

// 绑定事件处理器
wxBEGIN_EVENT_TABLE(OcrComposePanel, wxPanel)
EVT_BUTTON(ID_BTN_IMPORT_PHOTO, OcrComposePanel::OnImportPhoto)
EVT_BUTTON(ID_BTN_GENERATE_NEW, OcrComposePanel::OnGenerateNew)
EVT_LIST_ITEM_SELECTED(ID_LIST_OCR_RESULTS, OcrComposePanel::OnListSelect)
wxEND_EVENT_TABLE()

OcrComposePanel::OcrComposePanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    SetupLayout();
}

/**
 * @brief 设置 Panel 的布局 (科学交互友好布局)
 *
 * 布局设计：
 * 1. 使用 wxSplitterWindow 将 Panel 切分为左右两部分：
 * - 左侧：**试卷照片预览** (大图展示)
 * - 右侧：**操作区** (导入、题目列表、生成按钮)
 * 2. 右侧操作区采用 wxBoxSizer 垂直布局，清晰划分功能区域。
 */
void OcrComposePanel::SetupLayout()
{
    // 主 Sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 1. 分割窗口 (Splitter Window)
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D);
    splitter->SetSashGravity(0.7); // 左侧预览区占比 70%

    // --- 左侧：试卷照片预览区 ---
    wxPanel* previewPanel = new wxPanel(splitter);
    previewPanel->SetBackgroundColour(*wxWHITE);
    wxBoxSizer* previewSizer = new wxBoxSizer(wxVERTICAL);

    // 占位符/默认图片显示
    m_previewImage = new wxStaticBitmap(previewPanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    wxStaticText* placeholder = new wxStaticText(previewPanel, wxID_ANY, "在此预览导入的试卷照片 (OCR 擦除笔迹后)");

    previewSizer->Add(placeholder, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    previewSizer->Add(m_previewImage, 1, wxEXPAND | wxALL, 5); // 占据剩余空间
    previewPanel->SetSizer(previewSizer);


    // --- 右侧：操作与列表区 ---
    wxPanel* controlsPanel = new wxPanel(splitter);
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxVERTICAL);

    // 顶部：操作按钮 (水平布局)
    wxBoxSizer* topButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_importBtn = new wxButton(controlsPanel, ID_BTN_IMPORT_PHOTO, "导入试卷照片");   //📷 
    m_generateBtn = new wxButton(controlsPanel, ID_BTN_GENERATE_NEW, "AI 生成新卷并打印"); //🤖 
    m_generateBtn->Enable(false); // 初始禁用

    topButtonSizer->Add(m_importBtn, 1, wxEXPAND | wxRIGHT, 5);
    topButtonSizer->Add(m_generateBtn, 1, wxEXPAND | wxLEFT, 5);

    controlsSizer->Add(topButtonSizer, 0, wxEXPAND | wxALL, 10);

    // 中部：题目列表 (List Control)
    wxStaticText* listLabel = new wxStaticText(controlsPanel, wxID_ANY, "OCR 识别题目列表 (✓ 选择待重组题目):");
    m_resultList = new wxListCtrl(controlsPanel, ID_LIST_OCR_RESULTS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL);
    // 定义列
    m_resultList->InsertColumn(0, "✓", wxLIST_FORMAT_LEFT, 30);
    m_resultList->InsertColumn(1, "题号", wxLIST_FORMAT_LEFT, 50);
    m_resultList->InsertColumn(2, "题目简述 (OCR 结果)", wxLIST_FORMAT_LEFT, 200);
    m_resultList->InsertColumn(3, "难度/标签", wxLIST_FORMAT_LEFT, 80);

    controlsSizer->Add(listLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    controlsSizer->Add(m_resultList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10); // 占据垂直空间

    // 底部：功能说明 (可选)
    wxStaticText* footerText = new wxStaticText(controlsPanel, wxID_ANY,
        "提示: 点击题目可预览，勾选题目后点击 'AI 生成新卷'。", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
    controlsSizer->Add(footerText, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    controlsPanel->SetSizer(controlsSizer);

    // 分割窗口初始化
    splitter->SplitVertically(previewPanel, controlsPanel, 700);

    mainSizer->Add(splitter, 1, wxEXPAND | wxALL, 0);
    SetSizer(mainSizer);
}

// --- 事件处理器实现 ---

void OcrComposePanel::OnImportPhoto(wxCommandEvent& WXUNUSED(event))
{
    // 1. 打开文件对话框
    wxFileDialog openFileDialog(this, "选择试卷照片文件", "", "",
        "图片文件 (*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // 用户取消

    wxString path = openFileDialog.GetPath();

    // 2. 图像加载与预览
    wxImage image(path);
    if (!image.IsOk()) {
        wxLogError("无法加载图片文件: %s", path);
        return;
    }

    // 保持纵横比缩放以适应预览区 (m_previewImage)
    wxSize previewSize = m_previewImage->GetSize();
    image.Rescale(previewSize.GetWidth(), previewSize.GetHeight());
    m_previewImage->SetBitmap(wxBitmap(image));

    // 3. 模拟 OCR 识别和擦除笔迹
    // 实际项目中，这里调用 OCR API/模型，然后将 "擦除笔迹" 后的图片显示在预览区
    wxLogMessage("OCR 视觉识别和笔迹擦除完成。");

    // 4. 填充题目列表 (模拟数据)
    m_resultList->DeleteAllItems();
    // 假设 OCR 返回了 5 个题目
    for (int i = 0; i < 5; ++i) {
        long itemIndex = m_resultList->InsertItem(i, ""); // ✓ 列，留空
        m_resultList->SetItem(itemIndex, 1, wxString::Format("%d", i + 1));
        m_resultList->SetItem(itemIndex, 2, wxString::Format("求二次函数 $f(x)=x^2+2x+1$ 的最小值。 (OCR 结果%d)", i + 1));
        m_resultList->SetItem(itemIndex, 3, (i % 2 == 0 ? "中等" : "困难"));
        m_resultList->SetItemData(itemIndex, i); // 可存储原始题目数据ID
    }

    m_generateBtn->Enable(true);
}

void OcrComposePanel::OnListSelect(wxListEvent& event)
{
    // 当用户点击列表项时，可以在预览区突出显示对应的题目区域 (如果 OCR 提供了坐标信息)
    long item = event.GetItem().GetId();
    wxString questionText = m_resultList->GetItemText(item, 2);

    //wxLogStatus(wxGetApp().GetTopWindow(), "选中题目 %d: %s", item + 1, questionText);    //gyt mark

    // 实际项目中，可在此处加载对应题目的原始图片片段进行局部预览
}

void OcrComposePanel::OnGenerateNew(wxCommandEvent& WXUNUSED(event))
{
    // 1. 收集被选中的题目
    // 遍历 m_resultList，找到所有选中的/被“勾选”的题目 (本骨架中，我们简单地假设所有题目)
    wxArrayString selectedQuestions;
    // 实际代码需要检查 wxLC_STATE_SELECTED 或自定义的 CheckBox 状态
    selectedQuestions.Add("题目1：求二次函数的最小值");
    selectedQuestions.Add("题目3：解不等式");

    if (selectedQuestions.IsEmpty()) {
        wxMessageBox("请先选择您想要重新生成类似题目以组成新试卷的题目。", "错误", wxOK | wxICON_ERROR);
        return;
    }

    // 2. AI 模型调用
    // 关键步骤：将选中的题目 (文本/LaTeX) 和难度等信息，发送给 AI 模型（例如：OpenAI, Claude, 或本地 LLM/Seq2Seq 模型）。
    wxLogMessage("正在调用 AI 模型，基于 %d 个题目生成新的类似题目...", selectedQuestions.GetCount());

    // 3. 生成试卷 (PDF 或打印预览)
    wxMessageBox("AI 已成功生成新的类似题目并组成试卷。即将打开打印预览/PDF导出界面。",
        "生成成功", wxOK | wxICON_INFORMATION);

    // 4. 实际操作：打开一个新的 Panel 或对话框，显示生成的试卷，并提供**难度设计**和**试卷打印**选项。
}