///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main_frame_template.h"

///////////////////////////////////////////////////////////////////////////

MainFrameTemplate::MainFrameTemplate(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* main_frame_layout;
	main_frame_layout = new wxBoxSizer(wxHORIZONTAL);

	file_tree_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	file_tree_window->SetScrollRate(5, 5);
	wxBoxSizer* file_tree_layout;
	file_tree_layout = new wxBoxSizer(wxVERTICAL);

	file_tree = new wxGenericDirCtrl(file_tree_window, WindowID::file_tree, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY | wxDIRCTRL_3D_INTERNAL | wxSUNKEN_BORDER, wxEmptyString, 0);

	file_tree->ShowHidden(false);
	file_tree_layout->Add(file_tree, 1, wxEXPAND | wxALL, 5);


	file_tree_window->SetSizer(file_tree_layout);
	file_tree_window->Layout();
	file_tree_layout->Fit(file_tree_window);
	main_frame_layout->Add(file_tree_window, 1, wxEXPAND | wxALL, 5);

	left_file_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	left_file_window->SetScrollRate(5, 5);
	wxBoxSizer* left_file_window_layout;
	left_file_window_layout = new wxBoxSizer(wxVERTICAL);

	left_files = new wxListCtrl(left_file_window, WindowID::left_files, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	left_file_window_layout->Add(left_files, 1, wxALL | wxEXPAND, 5);


	left_file_window->SetSizer(left_file_window_layout);
	left_file_window->Layout();
	left_file_window_layout->Fit(left_file_window);
	main_frame_layout->Add(left_file_window, 1, wxEXPAND | wxALL, 5);

	right_file_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	right_file_window->SetScrollRate(5, 5);
	wxBoxSizer* right_file_window_layout;
	right_file_window_layout = new wxBoxSizer(wxVERTICAL);

	right_files = new wxListCtrl(right_file_window, WindowID::right_files, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	right_file_window_layout->Add(right_files, 1, wxALL | wxEXPAND, 5);


	right_file_window->SetSizer(right_file_window_layout);
	right_file_window->Layout();
	right_file_window_layout->Fit(right_file_window);
	main_frame_layout->Add(right_file_window, 1, wxEXPAND | wxALL, 5);


	this->SetSizer(main_frame_layout);
	this->Layout();
	menu_bar = new wxMenuBar(0);
	this->SetMenuBar(menu_bar);

	tool_bar = this->CreateToolBar(wxTB_DEFAULT_STYLE | wxTB_TEXT, WindowID::tool_bar);
	tool_bar->Realize();


	this->Centre(wxBOTH);

}

MainFrameTemplate::~MainFrameTemplate()
{

}