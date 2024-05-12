///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/dirctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

enum WindowID { file_tree = 1000, left_file_window, right_file_window, left_files, right_files, tool_bar};

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrameTemplate : public wxFrame
{
private:

protected:
	wxScrolledWindow* file_tree_window;
	wxGenericDirCtrl* file_tree;
	wxScrolledWindow* left_file_window;
	wxListCtrl* left_files;
	wxScrolledWindow* right_file_window;
	wxListCtrl* right_files;
	wxMenuBar* menu_bar;
	wxToolBar* tool_bar;

public:

	MainFrameTemplate(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(1000, 800), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MainFrameTemplate();

};
