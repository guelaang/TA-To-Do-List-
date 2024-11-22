#include "UI.h"
#include <wx/wx.h>
#include <string>
#include <vector>
#include "Task.h"
#include <wx/timer.h>
#include <wx/datetime.h>

UI::UI(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindEventHandlers();
	AddSavedTasks();
	StartTimer();
}

void UI::CreateControls()
{
	wxFont headLineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);
	panel->SetBackgroundColour(wxColour(169, 169, 200));

	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

	nameText = new wxStaticText(panel, wxID_ANY, "Gilang", wxDefaultPosition, wxSize(400, 35), wxALIGN_LEFT);
	topSizer->Add(nameText, 0, wxALIGN_LEFT | wxALL, 5);

	nimText = new wxStaticText(panel, wxID_ANY, "21120124140116", wxDefaultPosition, wxSize(400, 35), wxALIGN_RIGHT);
	topSizer->Add(nimText, 0, wxALIGN_RIGHT | wxALL, 5);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(topSizer, 0, wxEXPAND);

	headlineText = new wxStaticText(panel, wxID_ANY, "To Do List", wxDefaultPosition, wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headLineFont);
	mainSizer->Add(headlineText, 0, wxALIGN_CENTER | wxALL, 10);

	wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);

	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(495, 35), wxTE_PROCESS_ENTER);
	addbutton = new wxButton(panel, wxID_ANY, "Add", wxDefaultPosition, wxSize(100, 35));

	inputSizer->Add(inputField, 1, wxEXPAND | wxALL, 5);
	inputSizer->Add(addbutton, 0, wxALL, 5);

	mainSizer->Add(inputSizer, 0, wxEXPAND);

	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(600, 400));
	mainSizer->Add(checkListBox, 1, wxEXPAND | wxALL, 5);

	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxDefaultPosition, wxSize(100, 35));
	mainSizer->Add(clearButton, 0, wxEXPAND | wxALL, 5);

	timeDisplay = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(160, 35));
	timeDisplay->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	timeDisplay->SetForegroundColour(*wxBLACK);
	mainSizer->Add(timeDisplay, 0, wxALIGN_CENTER | wxALL, 5);

	panel->SetSizerAndFit(mainSizer);
}

void UI::BindEventHandlers()
{
	addbutton->Bind(wxEVT_BUTTON, &UI::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &UI::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &UI::OnListDelete, this);
	clearButton->Bind(wxEVT_BUTTON, &UI::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &UI::AppClosed, this);
}

void UI::AddSavedTasks()
{
	std::vector<Task> tasks = loadTaskFromFile("task.txt");
	
	for (const Task& task : tasks) {
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

void UI::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void UI::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void UI::OnListDelete(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode()) {
	case WXK_BACK:
			DeleteSelectedTask();
			break;
	case WXK_UP:
		MoveSelectedTask(-1);
		break;
	case WXK_DOWN:
		MoveSelectedTask(1);
		break;
	}
}

void UI::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty()) {
		return;
	}
	
	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks", "Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();

	if (result == wxID_YES) {
		checkListBox->Clear();
	}
}

void UI::AppClosed(wxCloseEvent& evt)
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++) {
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTaskToFile(tasks, "task.txt");
	evt.Skip();
}

void UI::StartTimer()
{
	timer = new wxTimer(this);
	Bind(wxEVT_TIMER, &UI::OnTimerTick, this, wxID_ANY);
	timer->Start(1000);
}

void UI::OnTimerTick(wxTimerEvent& evt)
{
	wxDateTime now = wxDateTime::Now();
	wxString timeString = now.Format("%H:%M:%S  %d-%m-%Y");
	timeDisplay->SetLabel(timeString);
}

void UI::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}

void UI::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();
	
	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	checkListBox->Delete(selectedIndex);
}

void UI::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	int newIndex = selectedIndex + offset;
	if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
		SwapTask(selectedIndex, newIndex);
		checkListBox->SetSelection(newIndex, true);
	}
}

void UI::SwapTask(int i, int j)
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };

	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);

}
