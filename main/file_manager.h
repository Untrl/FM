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

/** Struct for storing file specifics.
* Stores all specifics provided by std::filesystem library.
*/
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
	/** Converts file size to string.
	 *	Converts file size to string. Uses the highest possible multiple of unit.
	 * @see fileSize
	 * @return String of perms in POSIX format.
	 */
	std::string getFileSize();
	/** Converts perms to string.
	 *	Converts std::perms to string in POSIX format in order owner, group and others. 
	 * @see permissions
	 * @return String of perms in POSIX format.
	 */
	std::string getPerms();
	/** Converts file_time_type to string.
	* Converts std::filesystem::file_time_type to string in format "Day:Month:Year".
	* @see last_write_time
	* @return String of time in format "Day:Month:Year".
	*/
	std::string getLastWriteTime();
};

/** Filesystem library.
* Extension over std::filesystem library.
* @see std::filesystem
* @see file_specifics
*/
class Files {
public:
	/** Gets file specifics for every file in given directory.
	* Returns empty vector, if given path is not a directory.
	* @param directory a directory path.
	* @return File specifics for every file.
	*/
	static std::vector<file_specifics> getContentOfDirectory(std::filesystem::path directory);
	/** Finds given filename and gets its file specifics.
	* Iterates from given root and matches filenames of all files with given filename. Matches only if the filenames are identical.
	* @param filename a filename to search.
	* @param root path from which the search starts.
	* @return File specifics for every file which matches filename.
	*/
	static std::vector<file_specifics> findFile(std::string filename, std::filesystem::path root);
	/** Returns root directory.
	* OS specific.
	* @return Root directory.
	*/
	static std::filesystem::path getRootDirectory();
	/** Opens given file with preferred aplications.
	* @param filePath file to open.
	*/
	static void openFile(std::filesystem::path filePath);
	static void addDirectory(std::string directoryName, std::filesystem::path destination);
	static void renameFile(std::string newName, std::filesystem::path filePath);
	static void removeFile(std::filesystem::path filePath);
	static void copyFile(std::filesystem::path filePath, std::filesystem::path destination);
	static void moveFile(std::filesystem::path filePath, std::filesystem::path destination);
	/** Sorts given files by directories first then all the other files.
	* Used as the default sort.
	* @param files files to sort.
	* @param reversed reverse
	* @return Sorted files.
	*/
	static std::vector<file_specifics> SortByDirectories(std::vector<file_specifics> files, bool reversed = false);
	/** Sorts given files by date.
	* Last date first.
	* @param files files to sort.
	* @param reversed reverse
	* @return Sorted files.
	*/
	static std::vector<file_specifics> SortByDate(std::vector<file_specifics> files, bool reversed = false);
	/** Sorts given files by filenames.
	* Uses default string comparision.
	* @param files files to sort.
	* @param reversed reverse
	* @return Sorted files.
	*/
	static std::vector<file_specifics> SortByName(std::vector<file_specifics> files, bool reversed = false);
	/** Sorts given files by size.
	* Largest file size first.
	* @param files files to sort.
	* @param reversed reverse
	* @return Sorted files.
	*/
	static std::vector<file_specifics> SortBySize(std::vector<file_specifics> files, bool reversed = false);
private:
	/** Compares strings.
	* Used for file searching.
	*/
	static bool contains(std::string string, std::string searching);
};