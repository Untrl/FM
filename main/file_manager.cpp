#include "file_manager.h"

using namespace std;

file_specifics::file_specifics() { }

file_specifics::file_specifics(std::filesystem::path file) {
	std::error_code ec;
	path = file;
	fileName = file.filename().string();
	fileSize = filesystem::file_size(file, ec);
	if (ec.value() != 0) {
		fileSize = 0;
		fileSizeAccesible = false;
	}
	fileType = filesystem::status(file).type();
	last_write_time = filesystem::last_write_time(file);
	permissions = filesystem::status(file).permissions();
}

/** Converts perms to string.
*/
std::string file_specifics::getPerms() {
	string perms;
	perms += (filesystem::perms::owner_read & permissions) != filesystem::perms::none ? 'r' : '-';
	perms += (filesystem::perms::owner_write & permissions) != filesystem::perms::none ? 'w' : '-';
	perms += (filesystem::perms::owner_exec & permissions) != filesystem::perms::none ? 'x' : '-';
	perms += (filesystem::perms::group_read & permissions) != filesystem::perms::none ? 'r' : '-';
	perms += (filesystem::perms::group_write & permissions) != filesystem::perms::none ? 'w' : '-';
	perms += (filesystem::perms::group_exec & permissions) != filesystem::perms::none ? 'x' : '-';
	perms += (filesystem::perms::others_read & permissions) != filesystem::perms::none ? 'r' : '-';
	perms += (filesystem::perms::others_write & permissions) != filesystem::perms::none ? 'w' : '-';
	perms += (filesystem::perms::others_exec & permissions) != filesystem::perms::none ? 'x' : '-';
	return perms;
}

/** Converts file_time_type to string.
*/
std::string file_specifics::getLastWriteTime() {
	// Source: Guldrak (https://github.com/gulrak)
	// Rewrite to std::format, when gcc fully supports it
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(last_write_time - std::filesystem::file_time_type::clock::now()
		+ std::chrono::system_clock::now());
	std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
	//
	std::tm* lt = std::localtime(&tt);
	std::stringstream buffer;
	buffer << std::put_time(lt, "%d/%m/%Y");
	return buffer.str();
}

/** Converts file_size(uintmax_t) to string.
*/
std::string file_specifics::getFileSize() {
	if (!fileSizeAccesible) {
		return " ";
	}
	std::uintmax_t roundedFileSize = fileSize;
	int count = 0;
	while (roundedFileSize > 1024) {
		roundedFileSize /= 1024;
		count++;
	}
	std::string result = std::to_string(roundedFileSize);
	switch (count) {
		case 0:
			result += " B";
			break;
		case 1:
			result += " kB";
			break;
		case 2:
			result += " MB";
			break;
		case 3:
			result += " GB";
			break;
		case 4:
			result += " TB";
			break;
		case 5:
			result += " PB";
			break;
	}
	return result;
}

/** Creates file specifications for given directory.
*/
vector<file_specifics> Files::getContentOfDirectory(std::filesystem::path directory) {
	vector<file_specifics> ContentOfDirectory;
	if (filesystem::is_directory(directory)) {
		for (auto const& dir_entry : std::filesystem::directory_iterator{ directory, std::filesystem::directory_options::skip_permission_denied }) {
			ContentOfDirectory.push_back(file_specifics(dir_entry.path()));
		}
	}
	
	return ContentOfDirectory;
}

/** Iterates from given root and matches filenames of all files with given filename. Matches only if the filenames are identical.
* Returns all files which meet the condition. 
*/
std::vector<file_specifics> Files::findFile(std::string filename, std::filesystem::path root) {
	std::vector<file_specifics> files;
	try {
		for (auto dir_entry : std::filesystem::recursive_directory_iterator{ root, std::filesystem::directory_options::skip_permission_denied }) {
			std::string curr_filename = dir_entry.path().filename().string();
			if (curr_filename == filename) {
				files.push_back(file_specifics((dir_entry.path())));
			}
		}
	}
	catch (std::filesystem::filesystem_error& e) {

	}
	return files;
}

/** Returns root directory. OS specific.
*/
std::filesystem::path Files::getRootDirectory() {
#ifdef __linux__ 
	return std::filesystem::path("/");
#elif _WIN32
	DWORD disks = GetLogicalDrives();
	for (int i = 0; i < 32; i++) {
		if (disks & (1 << i)) {
			std::string disk_name;
			if (i < 26) {
				disk_name = char(65 + i);
			}
			else {
				disk_name = 'A' + char(65 + i);
			}
			return std::filesystem::path(disk_name + std::string(":\\"));
		}
	}
#endif
}

/** Opens given file with preferred aplications.
*/
void Files::openFile(std::filesystem::path filePath) {
#ifdef __linux__ 
	/* Could not find working solution for linux, but here is the closes one:
	std::string command = std::string("xdg-open ") + filePath.string();
	int _ = system(command.c_str());*/
#elif _WIN32
	std::string filePathString = filePath.string();
	int stringLength = (int)filePathString.length() + 1;
	int wstringLength = MultiByteToWideChar(CP_ACP, 0, filePathString.c_str(), stringLength, 0, 0);
	wchar_t* buffer = new wchar_t[wstringLength];
	MultiByteToWideChar(CP_ACP, 0, filePathString.c_str(), stringLength, buffer, wstringLength);
	std::wstring r(buffer);
	delete[] buffer;

	ShellExecute(NULL, NULL, r.c_str(), NULL, NULL, SW_SHOW);
#endif
}

void Files::addDirectory(std::string directoryName, std::filesystem::path destination) {
	std::filesystem::create_directory(destination.append(directoryName));
}

void Files::renameFile(std::string newName, std::filesystem::path filePath) {
	auto oldPath = std::filesystem::path(filePath);
	std::filesystem::rename(oldPath, filePath.replace_filename(newName));
}

void Files::moveFile(std::filesystem::path filePath, std::filesystem::path destination) {
	std::filesystem::rename(filePath, destination.append(filePath.filename().string()));
}

void Files::removeFile(std::filesystem::path filePath) {
	std::filesystem::remove_all(filePath);
}

void Files::copyFile(std::filesystem::path filePath, std::filesystem::path destination) {
	std::filesystem::copy(filePath, destination, std::filesystem::copy_options::recursive);
}

/** Compares strings.
*/
bool Files::contains(std::string string, std::string searching) {
	size_t pos = string.find(searching);
	return (pos != std::string::npos);
}

/** Sorts given files by directories first then all the other files. Used as the default sort.
*/
std::vector<file_specifics> Files::SortByDirectories(std::vector<file_specifics> files, bool reversed) {
	std::vector<file_specifics> sorted;
	std::vector<file_specifics> other;

	for (int i = 0; i < files.size(); i++) {
		if (files[i].fileType == std::filesystem::file_type::directory) {
			sorted.push_back(files[i]);
		}
		else {
			other.push_back(files[i]);
		}
	}
	sorted.insert(sorted.end(), other.begin(), other.end());

	if (reversed) {
		std::reverse(sorted.begin(), sorted.end());
	}
	return sorted;
}

/** Sorts given files by filenames.
*/
std::vector<file_specifics> Files::SortByName(std::vector<file_specifics> files, bool reversed) {
	auto sorted = std::vector<file_specifics>(files.size());
	std::vector<std::string> sortedFilenames;

	for (int i = 0; i < files.size(); i++) {
		sortedFilenames.push_back(files[i].fileName);
	}
	std::sort(sortedFilenames.begin(), sortedFilenames.end());

	if (reversed) {
		std::reverse(sortedFilenames.begin(), sortedFilenames.end());
	}

	for (int i = 0; i < files.size(); i++) {
		for (int j = 0; j < sortedFilenames.size(); j++) {
			if (files[i].fileName == sortedFilenames[j]) {
				sorted[j] = files[i];
				break;
			}
		}
	}

	return sorted;
}

/** Sorts given files by date. Last date first.
*/
std::vector<file_specifics> Files::SortByDate(std::vector<file_specifics> files, bool reversed) {
	auto sorted = std::vector<file_specifics>(files.size());
	auto usedIndexes = std::vector<bool>(files.size());
	std::vector<std::filesystem::file_time_type> sortedDates;

	for (int i = 0; i < files.size(); i++) {
		sortedDates.push_back(files[i].last_write_time);
	}
	std::sort(sortedDates.begin(), sortedDates.end());

	if (!reversed) {
		std::reverse(sortedDates.begin(), sortedDates.end());
	}

	for (int i = 0; i < files.size(); i++) {
		for (int j = 0; j < sortedDates.size(); j++) {
			if (!usedIndexes[j] && files[i].last_write_time == sortedDates[j]) {
				sorted[j] = files[i];
				usedIndexes[j] = true;
				break;
			}
		}
	}

	return sorted;
}

/** Sorts given files by size. Largest file size first.
*/
std::vector<file_specifics> Files::SortBySize(std::vector<file_specifics> files, bool reversed) {
	auto sorted = std::vector<file_specifics>(files.size());
	auto usedIndexes = std::vector<bool>(files.size());
	std::vector<std::uintmax_t> sortedSizes;
	for (int i = 0; i < files.size(); i++) {
		sortedSizes.push_back(files[i].fileSize);
	}
	std::sort(sortedSizes.begin(), sortedSizes.end());

	if (!reversed) {
		std::reverse(sortedSizes.begin(), sortedSizes.end());
	}

	for (int i = 0; i < files.size(); i++) {
		for (int j = 0; j < sortedSizes.size(); j++) {
			if (!usedIndexes[j] && files[i].fileSize == sortedSizes[j]) {
				sorted[j] = files[i];
				usedIndexes[j] = true;
				break;
			}
		}
	}

	return sorted;
}