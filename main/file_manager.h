#include <filesystem>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>
#endif

enum SortType { Default, ByName, ByDate, BySize };

struct file_specifics {
	std::filesystem::path path;
	std::string fileName;
	std::filesystem::perms permissions;
	std::uintmax_t fileSize;
	bool fileSizeAccesible = true;
	std::filesystem::file_type fileType;
	std::filesystem::file_time_type last_write_time;
	file_specifics();
	file_specifics(std::filesystem::path file);
	std::string getFileSize();
	std::string getPerms();
	std::string getLastWriteTime();
};

class Files {
public:
	Files();
	static std::vector<file_specifics> getContentOfDirectory(std::filesystem::path directory);
	static std::vector<file_specifics> findFile(std::string filename, std::filesystem::path root);
	static std::filesystem::path getRootDirectory();
	static void openFile(std::filesystem::path filePath);
	static void addDirectory(std::string directoryName, std::filesystem::path destination);
	static void renameFile(std::string newName, std::filesystem::path filePath);
	static void removeFile(std::filesystem::path filePath);
	static void copyFile(std::filesystem::path filePath, std::filesystem::path destination);
	static void moveFile(std::filesystem::path filePath, std::filesystem::path destination);
	static std::vector<file_specifics> SortByDirectories(std::vector<file_specifics> files, bool reversed = false);
	static std::vector<file_specifics> SortByDate(std::vector<file_specifics> files, bool reversed = false);
	static std::vector<file_specifics> SortByName(std::vector<file_specifics> files, bool reversed = false);
	static std::vector<file_specifics> SortBySize(std::vector<file_specifics> files, bool reversed = false);
	void setCurrentDirectory(std::filesystem::path path);
	std::filesystem::path getCurrentDirectory();
private:
	std::filesystem::path curr_path;
	static bool contains(std::string string, std::string searching);
};