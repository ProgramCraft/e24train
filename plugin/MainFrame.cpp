// MainFrame.cpp
#include "MainFrame.h"
#include <wx/msgdlg.h>
#include <wx/statline.h>
#include <wx/artprov.h>

// 绑定事件处理器
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_FILE_QUIT, MainFrame::OnQuit)
    EVT_MENU(ID_HELP_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_FILE_OPEN, MainFrame::OnFileOpen)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700))
{
    // 1. 初始化 AUI 管理器
    m_auiManager.SetManagedWindow(this);

    // 2. 创建菜单栏和工具栏
    CreateMenuAndToolbar();

    // 3. 创建 AUI Notebook
    m_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ALL_TABS);

    // 4. 创建 Notebook Panles (Tab页)
    CreateNotebookPanels();

    // 5. 状态栏
    CreateStatusBar(2);
    SetStatusText("Ready", 0);
    SetStatusText("AI Model: Idle", 1);

    // 6. AUI 布局最终化
    m_auiManager.AddPane(m_notebook, wxAuiPaneInfo().CenterPane());
    m_auiManager.Update();

    // 确保窗口居中
    Centre(wxBOTH);
}

MainFrame::~MainFrame()
{
    m_auiManager.UnInit();
}

/**
 * @brief 创建菜单栏和工具栏
 */
void MainFrame::CreateMenuAndToolbar()
{
    // --- 菜单栏设计 ---
    wxMenuBar* menuBar = new wxMenuBar;

    // 文件 (File) 菜单：基本操作
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_FILE_OPEN, "打开试卷照片...\tCtrl+O", "打开试卷照片进行识别");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_FILE_QUIT, "退出\tAlt+F4", "退出应用程序");
    menuBar->Append(fileMenu, "文件");

    // AI/数据 (AI/Data) 菜单：核心功能配置
    wxMenu* aiMenu = new wxMenu;
    aiMenu->Append(ID_AI_SETTINGS, "AI 模型设置...", "配置 OCR 和题目生成模型");
    aiMenu->Append(wxID_ANY, "数据库连接设置...", "配置题目数据库");
    menuBar->Append(aiMenu, "AI/数据");

    // 帮助 (Help) 菜单
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(ID_HELP_ABOUT, "关于...", "关于本应用");
    menuBar->Append(helpMenu, "帮助");

    SetMenuBar(menuBar);

#if 0
    // --- 工具栏设计 (ToolBar) ---
    wxToolBar* toolBar = CreateToolBar();
    toolBar->AddTool(ID_FILE_OPEN, "打开", wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR), "打开试卷照片");
    toolBar->AddSeparator();
    toolBar->AddTool(ID_AI_SETTINGS, "AI设置", wxArtProvider::GetBitmap(wxART_FIND, wxART_TOOLBAR), "配置 AI 模型");
    toolBar->Realize();
#endif
}

/**
 * @brief 创建 AUI Notebook 的 Tab 页
 */
void MainFrame::CreateNotebookPanels()
{
    // 1. 错题组卷 Panel (核心功能)
    m_ocrComposePanel = new OcrComposePanel(m_notebook);
    // 使用图标和更清晰的名称
    m_notebook->AddPage(m_ocrComposePanel, "错题组卷", true);   //🤖 
    // 

    // 2. 题目数据库管理 Panel (用于数据库操作)
    // 假设未来会创建 DatabasePanel 类
    wxPanel* dbPanel = new wxPanel(m_notebook);
    dbPanel->SetBackgroundColour(*wxLIGHT_GREY);
    wxStaticText* dbText = new wxStaticText(dbPanel, wxID_ANY, "【题目数据库管理】 - 待实现");
    wxBoxSizer* dbSizer = new wxBoxSizer(wxVERTICAL);
    dbSizer->Add(dbText, 1, wxALIGN_CENTER | wxALL, 20);
    dbPanel->SetSizer(dbSizer);
    m_notebook->AddPage(dbPanel, "题库查询", false);

    // 3. AI 模型训练/配置 Panel (用于 AI/ML 配置)
    // 假设未来会创建 AISettingsPanel 类
    wxPanel* aiPanel = new wxPanel(m_notebook);
    aiPanel->SetBackgroundColour(wxColour(240, 255, 240)); // 淡绿色背景
    wxStaticText* aiText = new wxStaticText(aiPanel, wxID_ANY, "【AI 模型配置与训练】 - 待实现");
    wxBoxSizer* aiSizer = new wxBoxSizer(wxVERTICAL);
    aiSizer->Add(aiText, 1, wxALIGN_CENTER | wxALL, 20);
    aiPanel->SetSizer(aiSizer);
    m_notebook->AddPage(aiPanel, "AI模型训练", false);
}

// --- 事件处理器实现 ---

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("基于学生错题生成类似题目供训练的智能系统。\n\n技术栈：wxWidgets, OCR, 数据库, AI/ML。",
                 "关于 智能错题组卷系统", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event))
{
    // 此处可以添加全局的文件打开逻辑
    // 为了聚焦在错图组卷功能，这里只是一个提示
    wxMessageBox("您选择了 [打开试卷照片] 功能，将在 '错题组卷' Tab 中处理。", "提示", wxOK | wxICON_INFORMATION);

    // 自动切换到错题组卷 Tab
    if (m_ocrComposePanel) {
        m_notebook->SetSelection(m_notebook->GetPageIndex(m_ocrComposePanel));
    }
}