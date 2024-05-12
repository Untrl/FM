#include "../gui/main_frame_template.h"
#include "file_manager.h"
#include <wx/textdlg.h>
#include <wx/msgdlg.h>

enum MenuID { Add = 2000, Rename, Remove, Move, Copy };
enum ToolbarID { ADD = 3000, RENAME, REMOVE, MOVE, COPY, FIND };

class MainFrame : public MainFrameTemplate {
private:
	std::filesystem::path left_list_directory;
	std::filesystem::path right_list_directory;
	wxMenu* menu;
	wxListCtrl* working_list;
	wxTextEntryDialog* textDialog;
	wxImageList* image_list;
	std::filesystem::path GetCurrentDirectoryFromList(wxListCtrl* List);
	void WriteErrorMessage(std::string);
public:
	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1000, 800), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~MainFrame();

	void SetFilesWindowsFromTree(wxTreeEvent& event);
	void SetFilesWindows(std::filesystem::path directory);
	void SetFilesWindowsFromList(wxListEvent& event);
	void ShowMenu(wxListEvent& event);
	void SetWorkingWindow(wxListEvent& event);
	void Sort(wxListEvent& event);
	void Rename(wxCommandEvent& event);
	void Delete(wxCommandEvent& event);
	void Move(wxCommandEvent& event);
	void Add(wxCommandEvent& event);
	void Copy(wxCommandEvent& event);
	void Find(wxCommandEvent& event);
	void SetFileWindow(std::filesystem::path directory, wxListCtrl* windowList, SortType SortBy = SortType::Default);
	void SetFileWindow(std::vector<file_specifics> files, wxListCtrl* windowList);
	void RefreshFileWindows();
	void DeleteFileWindowItems(wxListCtrl* windowList);
};