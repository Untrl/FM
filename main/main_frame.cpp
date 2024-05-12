#include "main_frame.h"

/** Initialization the main frame and its components.
*/ 
MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : MainFrameTemplate(parent, id, title, pos, size, style) {
	//Toolbar initialization
	tool_bar->AddTool(ToolbarID::ADD, "Add directory", wxArtProvider::GetBitmapBundle(wxART_NEW, wxART_TOOLBAR), "Adds directory to the left file window.");
	tool_bar->AddTool(ToolbarID::COPY, "Copy", wxArtProvider::GetBitmapBundle(wxART_COPY, wxART_TOOLBAR), "Copies selected file/files to the other file window.");
	tool_bar->AddTool(ToolbarID::MOVE, "Move", wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE, wxART_TOOLBAR), "Moves selected file/files to the other file window.");
	tool_bar->AddTool(ToolbarID::REMOVE, "Remove", wxArtProvider::GetBitmapBundle(wxART_DELETE, wxART_TOOLBAR), "Remove selected file/files.");
	tool_bar->AddTool(ToolbarID::RENAME, "Rename", wxArtProvider::GetBitmapBundle(wxART_EDIT, wxART_TOOLBAR), "Rename selected file.");
	tool_bar->AddTool(ToolbarID::FIND, "Find", wxArtProvider::GetBitmapBundle(wxART_FIND, wxART_TOOLBAR), "Find file and display its directory in left file window.");

	tool_bar->Realize();

	//File window initialization
	image_list = new wxImageList(16, 16, true);
	image_list->Add(wxArtProvider::GetIcon(wxART_FOLDER, wxART_OTHER, wxSize(16, 16)));
	left_files->SetImageList(image_list, wxIMAGE_LIST_SMALL);
	right_files->SetImageList(image_list, wxIMAGE_LIST_SMALL);

	textDialog = new wxTextEntryDialog(this, "", "");

	wxListItem itemCol;
	itemCol.SetText("Name");
	itemCol.SetImage(-1);
	itemCol.SetWidth(wxLIST_AUTOSIZE);
	left_files->InsertColumn(0, itemCol);
	right_files->InsertColumn(0, itemCol);

	itemCol.SetText("Size");
	left_files->InsertColumn(1, itemCol);
	right_files->InsertColumn(1, itemCol);

	itemCol.SetText("Date");
	left_files->InsertColumn(2, itemCol);
	right_files->InsertColumn(2, itemCol);

	// Only useful for POSIX systems.
#ifdef __linux__ 
	itemCol.SetText("Permissions");
	left_files->InsertColumn(3, itemCol);
	right_files->InsertColumn(3, itemCol);
#endif

	working_list = left_files;
	std::filesystem::path root = Files::getRootDirectory();
	left_list_directory = root;
	right_list_directory = root;

	SetFilesWindows(root);

	//Popup Menu initialization
	menu = new wxMenu();
	menu->Append(MenuID::Remove, "Delete");
	menu->Append(MenuID::Rename, "Rename");
	menu->Append(MenuID::Move, "Move");
	menu->Append(MenuID::Copy, "Copy");

	//Menu bar initialization
	wxMenu* about_menu = new wxMenu();
	menu_bar->Append(about_menu, "About");

	//Connect Events to their event handlers
	Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &MainFrame::SetFilesWindowsFromTree, this, WindowID::file_tree);
	Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::SetFilesWindowsFromList, this, WindowID::left_files);
	Bind(wxEVT_LIST_ITEM_ACTIVATED, &MainFrame::SetFilesWindowsFromList, this, WindowID::right_files);
	Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::ShowMenu, this, WindowID::left_files);
	Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MainFrame::ShowMenu, this, WindowID::right_files);
	Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::SetWorkingWindow, this, WindowID::left_files);
	Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::SetWorkingWindow, this, WindowID::right_files);
	Bind(wxEVT_LIST_COL_CLICK, &MainFrame::Sort, this, WindowID::left_files);
	Bind(wxEVT_LIST_COL_CLICK, &MainFrame::Sort, this, WindowID::right_files);
	Bind(wxEVT_MENU, &MainFrame::Delete, this, MenuID::Remove);
	Bind(wxEVT_MENU, &MainFrame::Rename, this, MenuID::Rename);
	Bind(wxEVT_MENU, &MainFrame::Move, this, MenuID::Move);
	Bind(wxEVT_MENU, &MainFrame::Copy, this, MenuID::Copy);
	Bind(wxEVT_TOOL, &MainFrame::Add, this, ToolbarID::ADD);
	Bind(wxEVT_TOOL, &MainFrame::Delete, this, ToolbarID::REMOVE);
	Bind(wxEVT_TOOL, &MainFrame::Rename, this, ToolbarID::RENAME);
	Bind(wxEVT_TOOL, &MainFrame::Move, this, ToolbarID::MOVE);
	Bind(wxEVT_TOOL, &MainFrame::Copy, this, ToolbarID::COPY);
	Bind(wxEVT_TOOL, &MainFrame::Find, this, ToolbarID::FIND);

}

MainFrame::~MainFrame() {

}

/** Returns the current shown directory for given wxCtrlList.
*/
std::filesystem::path MainFrame::GetCurrentDirectoryFromList(wxListCtrl* List) {
	if (List->GetId() == WindowID::left_files) {
		return left_list_directory;
	}
	else {
		return right_list_directory;
	}
}

/** Event handler for column click. Sorts files on column click event on given wxCtrlList.
 * The sort type is selected based on the column number. 
 */
void MainFrame::Sort(wxListEvent& event) {
	wxListCtrl* list;
	std::filesystem::path directory;
	SortType sortType = SortType::Default;

	//Choose the correct wxCtrlList base on event ID
	if (event.GetId() == WindowID::left_files) {
		list = left_files;
		directory = GetCurrentDirectoryFromList(left_files);
	}
	else {
		list = right_files;
		directory = GetCurrentDirectoryFromList(right_files);
	}

	switch (event.GetColumn()) {
		case 0:
			sortType = SortType::ByName;
			break;
		case 1:
			sortType = SortType::BySize;
			break;
		case 2:
			sortType = SortType::ByDate;
			break;
	}

	MainFrame::SetFileWindow(directory, list, sortType);
}

/** Shows files for given directory to wxCtrlList. The files can be sorted by selecting a SortType. 
*  If no SortType is specified it sorts it by directories first.
*/
void MainFrame::SetFileWindow(std::filesystem::path directory, wxListCtrl* windowList, SortType SortBy) {
	if (windowList->GetId() == WindowID::left_files) {
		left_list_directory = directory;
	}
	else {
		right_list_directory = directory;
	}
	
	windowList->DeleteAllItems();
	std::vector<file_specifics> files;
	try {
		files = Files::getContentOfDirectory(directory);
	}
	catch (std::filesystem::filesystem_error& e) {
		WriteErrorMessage(e.what());
		return;
	}
	//Sort
	switch (SortBy) {
	case SortType::Default:
		files = Files::SortByDirectories(files);
		break;
	case SortType::ByName:
		files = Files::SortByName(files);
		break;
	case SortType::ByDate:
		files = Files::SortByDate(files);
		break;
	case SortType::BySize:
		files = Files::SortBySize(files);
		break;
	}

	wxListItem item;
	int returnPathIndex = 0;
	if (directory.root_path() != directory) {
		item.SetData(new std::filesystem::path(directory.parent_path()));
		item.SetText("../");
		item.SetId(0);
		windowList->InsertItem(item);
		returnPathIndex = 1;
	}
	for (int i = 0; i < files.size(); i++)
	{
		item.SetData(new std::filesystem::path(files[i].path));
		item.SetText(files[i].fileName);
		item.SetId(i + returnPathIndex);
		if (files[i].fileType == std::filesystem::file_type::directory) {
			item.SetImage(0);
		}
		else {
			item.SetImage(-1);
		}
		windowList->InsertItem(item);
		windowList->SetItem(i + returnPathIndex, 1, files[i].getFileSize());
		windowList->SetItem(i + returnPathIndex, 2, files[i].getLastWriteTime());
#ifdef __linux__ 
		windowList->SetItem(i + returnPathIndex, 3, files[i].getPerms());
#endif 
	}
}

/** Shows files to wxCtrlList. Used only for Find.
*/
void MainFrame::SetFileWindow(std::vector<file_specifics> files, wxListCtrl* windowList) {
	DeleteFileWindowItems(windowList);
	wxListItem item;

	for (int i = 0; i < files.size(); i++)
	{
		item.SetData(new std::filesystem::path(files[i].path));
		item.SetText(files[i].fileName);
		item.SetId(i);
		if (files[i].fileType == std::filesystem::file_type::directory) {
			item.SetImage(0);
		}
		else {
			item.SetImage(-1);
		}
		windowList->InsertItem(item);
		windowList->SetItem(i, 1, files[i].getFileSize());
		windowList->SetItem(i, 2, files[i].getLastWriteTime());
#ifdef __linux__ 
		windowList->SetItem(i, 3, files[i].getPerms());
#endif 
	}
}

void MainFrame::DeleteFileWindowItems(wxListCtrl* windowList) {
	auto children_list = windowList->GetChildren();
	for (int i = 0; i < children_list.size(); i++) {
		delete (std::filesystem::path*)((wxListItem*)children_list[i])->m_data;
	}
	windowList->DeleteAllItems();
}

/** Shows files for given directory to both wxCtrlList. Used mainly for inicialization.
*/
void MainFrame::SetFilesWindows(std::filesystem::path directory) {
	SetFileWindow(directory, left_files);
	SetFileWindow(directory, right_files);
}

/** Event handler for wxCtrlTree activation. Shows files for selected directory in the wxCtrlTree to both wxCtrlList.
*/
void MainFrame::SetFilesWindowsFromTree(wxTreeEvent& event) {
	auto path = std::filesystem::path(file_tree->GetPath().ToStdString());
	SetFilesWindows(path);
}

/** Event handler for wxCtrlList double click. Shows files for selected directory.
*/
void MainFrame::SetFilesWindowsFromList(wxListEvent& event) {
	std::filesystem::path path = *static_cast<std::filesystem::path*>(wxUIntToPtr(event.GetItem().GetData()));

	if (std::filesystem::is_directory(path)) {
		if (event.GetId() == WindowID::left_files) {
			SetFileWindow(path, left_files);
		}
		else {
			SetFileWindow(path, right_files);
		}
	}
	else if (std::filesystem::is_regular_file(path)) {
		Files::openFile(path);
	}
}

/** Event handler for right click on wxCtrlList item. Shows menu of possible operation on the file(s).
*/
void MainFrame::ShowMenu(wxListEvent& event) {
	if (event.GetId() == WindowID::left_files) {
		working_list = left_files;
	}
	else {
		working_list = right_files;
	}
	PopupMenu(menu);
}

/** On each click on wxCtrlList select it as working list. Used for operation on the file(s).
*/
void MainFrame::SetWorkingWindow(wxListEvent& event) {
	if (event.GetId() == WindowID::left_files) {
		working_list = left_files;
	}
	else {
		working_list = right_files;
	}
}

/** Event handler for delete operation.
*/
void MainFrame::Delete(wxCommandEvent& event) {
	long selectedItem = working_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while (selectedItem != -1)
	{
		std::filesystem::path path = *static_cast<std::filesystem::path*>(wxUIntToPtr(working_list->GetItemData(selectedItem)));
		try {
			Files::removeFile(path);
		}	
		catch (std::filesystem::filesystem_error& e) {
			WriteErrorMessage(e.what());
		}
		selectedItem = working_list->GetNextItem(selectedItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	RefreshFileWindows();
}

/** Event handler for rename operation.
*/
void MainFrame::Rename(wxCommandEvent& event) {
	if (working_list->GetSelectedItemCount() > 1) {
		wxMessageBox("Select only one file to rename.");
		return;
	}
	long selectedItem = working_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	std::filesystem::path filePath = *static_cast<std::filesystem::path*>(wxUIntToPtr(working_list->GetItemData(selectedItem)));
	std::string oldName = filePath.filename().string();

	textDialog = new wxTextEntryDialog(this, "Enter new file name", "", oldName);
	textDialog->ShowModal();

	std::string newName = textDialog->GetValue().ToStdString();
	if (newName != oldName) {
		try {
			Files::renameFile(newName, filePath);
		}
		catch (std::filesystem::filesystem_error& e) {
			WriteErrorMessage(e.what());
		}
	}
	RefreshFileWindows();
}

/** Event handler for move operation.
*/
void MainFrame::Move(wxCommandEvent& event) {
	long selectedItem = working_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while (selectedItem != -1)
	{
		wxListCtrl* destinationList = (working_list->GetId() == WindowID::left_files) ? right_files : left_files;
		std::filesystem::path filePath = *static_cast<std::filesystem::path*>(wxUIntToPtr(working_list->GetItemData(selectedItem)));
		std::filesystem::path destination = GetCurrentDirectoryFromList(destinationList);

		if (filePath.parent_path() != destination) {
			try {
				Files::moveFile(filePath, destination);
			}
			catch (std::filesystem::filesystem_error& e) {
				WriteErrorMessage(e.what());
			}
		}
		selectedItem = working_list->GetNextItem(selectedItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	RefreshFileWindows();
}

/** Event handler for add operation.
*/
void MainFrame::Add(wxCommandEvent& event) {
	auto destination = GetCurrentDirectoryFromList(left_files);

	textDialog = new wxTextEntryDialog(this, "Enter new directory name", "", "New directory");
	textDialog->ShowModal();

	std::string newName = textDialog->GetValue().ToStdString();
	try {
		Files::addDirectory(newName, destination);
	}
	catch (std::filesystem::filesystem_error& e) {
		WriteErrorMessage(e.what());
	}
	RefreshFileWindows();
}

/** Event handler for copy operation.
*/
void MainFrame::Copy(wxCommandEvent& event) {
	long selectedItem = working_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	while (selectedItem != -1)
	{
		wxListCtrl* destinationList = (working_list->GetId() == WindowID::left_files) ? right_files : left_files;
		std::filesystem::path filePath = *static_cast<std::filesystem::path*>(wxUIntToPtr(working_list->GetItemData(selectedItem)));
		std::filesystem::path destination = GetCurrentDirectoryFromList(destinationList);
		if (filePath.parent_path() != destination) {
			try {
				Files::copyFile(filePath, destination);
			}
			catch (std::filesystem::filesystem_error& e) {
				WriteErrorMessage(e.what());
			}
		}
		selectedItem = working_list->GetNextItem(selectedItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}
	RefreshFileWindows();
}

/** Event handler for find operation.
*/
void MainFrame::Find(wxCommandEvent& event) {
	textDialog = new wxTextEntryDialog(this, "Find file:", "", "");
	textDialog->ShowModal();
	std::string filename = textDialog->GetValue().ToStdString();

	std::vector<file_specifics> files = Files::findFile(filename, Files::getRootDirectory());

	if (!files.empty()) {
		SetFileWindow(files, left_files);
	}
	else {
		wxMessageBox("File not found.");
	}
}

/** Refreshes both wxCtrlList by showing the same directory once more.
*/
void MainFrame::RefreshFileWindows() {
	auto path = GetCurrentDirectoryFromList(left_files);
	SetFileWindow(path, left_files);

	path = GetCurrentDirectoryFromList(right_files);
	SetFileWindow(path, right_files);
}

/** Shows error message.
*/
void MainFrame::WriteErrorMessage(std::string message) {
	wxMessageBox(message, "Error occured");
}
