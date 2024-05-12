#include <wx/wx.h>
#include "main/main_frame.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override;
    int OnExit() override;
private:
    MainFrame* frame;
};

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
    frame = new MainFrame(nullptr, wxID_ANY);
    frame->Show(true);
    return true;
}

int MyApp::OnExit() {
    delete frame;
    return 0;
}