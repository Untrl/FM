# FM
FM is a file manager written in C++  with a GUI created using the wxWidgets library. 

## Features

- file tree for selecting working directory
- two seperated file windows for ease of use
- shows date, file size and permissions for files
- supports sorting files and directories by name, date and by directories and files
- supports adding of directory and renaming, deleting, moving and copying files and directories
- opening files with preferred application

## How to install
Clone or download this repository to your local environment and build it.
### Useful links
[User Manual](docs/user_manual.md)

[Programmer's Guide](docs/programmers_guide.md)
### How to build
#### Windows
Prerequisites: [CMake](https://cmake.org/download/), C++ compiler ([List of compilers](https://isocpp.org/get-started))

Build the project with CMake and run FM.exe.
#### Linux
Prerequisites: Build essentials, CMake and GTK 3 library
##### Ubuntu 22.04
```
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libgtk-3-dev
cmake .
cmake --build <directory>
./FM
```
