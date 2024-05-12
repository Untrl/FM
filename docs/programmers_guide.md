# Architecture Overview

The File Manager application is built using the C++ programming language with the wxWidgets library for the graphical user interface. Uses wxWidgets version 3.2.
## Components

### GUI
The application consists only of main frame. The main frame is a wxFrame and consists of two wxListCtrl as file windows and wxGenericDirCtrl (specialization of wxTreeCtrl) as file tree. Also has a wxToolbar for operation buttons. The main frame is responsible for handling events and calling correct file operations. For more documentation on the GUI elements please refer to [wxWidgets](https://docs.wxwidgets.org/3.2/).

### File manager library
Provides all the file funcionality to the GUI. Based on standart library filesystem. Defines file_specifics structure which contains all useful information for a file (name, size, last write time, size and permissions). Contains add directory, rename, move, copy and delete operations. It can also open a file in preferred application. It is missing add file operation. Also can sort files based on their attributes.  
