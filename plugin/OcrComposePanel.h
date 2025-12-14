// OcrComposePanel.h
#ifndef OCRCOMPOSEPANEL_H
#define OCRCOMPOSEPANEL_H

#include <wx/wx.h>
#include <wx/listctrl.h> // 用于题目列表

// 枚举常量：用于 Panel 内部控件的ID
enum
{
    ID_BTN_IMPORT_PHOTO = 2001,
    ID_BTN_GENERATE_NEW,
    ID_LIST_OCR_RESULTS,
};

class OcrComposePanel : public wxPanel
{
public:
    OcrComposePanel(wxWindow* parent);

private:
    // UI 控件
    wxButton* m_importBtn;
    wxButton* m_generateBtn;
    wxListCtrl* m_resultList; // OCR 识别后的题目列表
    wxStaticBitmap* m_previewImage; // 试卷照片预览

    // 内部函数
    void SetupLayout();

    // Event Handlers
    void OnImportPhoto(wxCommandEvent& event);
    void OnGenerateNew(wxCommandEvent& event);
    void OnListSelect(wxListEvent& event);

    // 事件表声明
    wxDECLARE_EVENT_TABLE();
};

#endif // OCRCOMPOSEPANEL_H