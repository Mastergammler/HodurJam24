// this is needed else it complains about redefinition of default arguments ...
#pragma once

#include "../logging/module.h"
#include "imports.h"
#include "types.h"

unordered_map<string, string> read_kvp_file(const string& filePath,
                                            const char delimiter);

wstring to_wstring(const string str);
bool file_exists(string filePath);
string extract_file_name(string filePath);

vector<FileEntry> directory_files(const string directory);

/**
 * Reads a file line by line and returns the result
 */
vector<string> read_all_lines(const string filePath);

//  PARSING
bool starts_with(const string& input,
                 const string& startSequence,
                 bool caseSensitive = false);
