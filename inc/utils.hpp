#include "Webserv.hpp"

#pragma once

// strings.cpp
std::string &trim(std::string &s, const std::string &tobetrimmed);
bool	endsWith(const std::string &str, const std::string &suffix);
void	findAndReplaceAll(std::string &str, const std::string toFind,
			const std::string toReplace);
bool	findChar(const std::string &str, const std::string chars);
std::string toLower(const std::string str);
std::string reduce(std::string str, char c);
std::string to_string(long long i);
long Stol(const std::string &str, std::size_t *idx, int base);

// extention.cpp
std::string getExtenson(std::string file);
std::string getType(std::string path);

// files.cpp
int		checkFileAccess(const std::string &path);
bool	isDir(const std::string &path);
std::string searchDir(const std::string &path,
						const std::set<std::string> &files);
off_t getFileSize(const char *filePath);


// erros.cpp
std::string getStatusMessage(int code);
std::string generateResPage(int code);



// time.cpp
long getTime();
void log(std::string msg, log_type type);