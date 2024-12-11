#include "../logging/module.h"
#include "imports.h"
#include "types.h"

unordered_map<string, string> read_kvp_file(const string& filePath,
                                            const char delimiter);

wstring to_wstring(const string str);
bool file_exists(string filePath);
string extract_file_name(string filePath);

vector<FileEntry> directory_files(const string directory);
