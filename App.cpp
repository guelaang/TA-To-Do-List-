#include "App.h"
#include "UI.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    UI* mainFrame = new UI("To Do List");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}
