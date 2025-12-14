#include <wx/wx.h>
#include <wx/aui/aui.h>
//#include "MainFrame.h"
#include "plugin/MainFrame.h"

class MainApp : public wxApp
{

	public:
		virtual bool OnInit();
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit()
{
    SetAppName("E24Train");

    MainFrame* frame = new MainFrame("E24Train");

    frame->Show(true);
    
    return true;
}

