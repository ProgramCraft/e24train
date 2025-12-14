// MainFrame.h
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/aui/aui.h> // 引入 AUI Notebook
#include "OcrComposePanel.h" // 引入错图组卷Panel

// 枚举常量：用于菜单和工具栏的ID
enum
{
    // 文件菜单
    ID_FILE_OPEN = wxID_OPEN,
    ID_FILE_QUIT = wxID_EXIT,
    // AI 菜单
    ID_AI_SETTINGS = 1001,
    // 帮助菜单
    ID_HELP_ABOUT = wxID_ABOUT,
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();

private:
    // AUI 管理器：用于管理窗口布局
    wxAuiManager m_auiManager;
    // AUI Notebook：用于实现 Tab 切换
    wxAuiNotebook* m_notebook;

    // 内部函数
    void CreateMenuAndToolbar();
    void CreateNotebookPanels();

    // Event Handlers
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);

    // 确保 OcrComposePanel 能够被 MainFrame 访问
    OcrComposePanel* m_ocrComposePanel;

    // 事件表声明
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H