#pragma once
#include <wx/wx.h>
#include <string>
#include <vector>
#include "Task.h"
#include <wx/timer.h>
#include <wx/datetime.h>

class UI : public wxFrame
{
public:
    UI(const wxString& title);

private:
    void CreateControls();
    void BindEventHandlers();
    void AddSavedTasks();
    void StartTimer();
    void OnTimerTick(wxTimerEvent& evt);

    void OnAddButtonClicked(wxCommandEvent& evt);
    void OnInputEnter(wxCommandEvent& evt);
    void OnListDelete(wxKeyEvent& evt);
    void OnClearButtonClicked(wxCommandEvent& evt);
    void AppClosed(wxCloseEvent& evt);
    void AddTaskFromInput();
    void DeleteSelectedTask();
    void MoveSelectedTask(int offset);
    void SwapTask(int i, int j);

    void CreateLoginPage();
    void OnLoginButtonClicked(wxCommandEvent& evt);
    void ShowTaskPage();

    wxPanel* panel;
    wxStaticText* headlineText;
    wxTextCtrl* inputField;
    wxButton* addbutton;
    wxCheckListBox* checkListBox;
    wxButton* clearButton;
    wxTimer* timer;
    wxStaticText* timeDisplay;

    wxTextCtrl* usernameField;
    wxTextCtrl* passwordField;
    wxButton* loginButton;
    wxStaticText* errorText;
    wxStaticText* nameText;
    wxStaticText* nimText;
};
