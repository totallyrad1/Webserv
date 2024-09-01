#include "../../inc/Webserv.hpp"

int	checkFileAccess(const std::string &path)
{
	if (access(path.c_str(), F_OK) == 0)
	{
		if (access(path.c_str(), R_OK) != 0)
		{
			log("File "+path + " is not readable", ERROR);
			return (403);
		}
		else
			return (200);
	}
	else
	{
		log("File "+path + " does not exist", ERROR);
		return (404);
	}
}

bool	isDir(const std::string &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) != 0)
		return (false);
	else if (info.st_mode & S_IFDIR)
		return (true);
	return (false);
}

std::string searchDir(const std::string &path,
		const std::set<std::string> &files)
{
	DIR				*dir;
	struct dirent	*entry;
	std::string		file;

	dir = opendir(path.c_str());
	if (dir == NULL)
		return ("");
	while ((entry = readdir(dir)) != NULL)
	{
		file = entry->d_name;
		if (files.find(file) != files.end())
		{
			closedir(dir);
			return (file);
		}
	}
	closedir(dir);
	return ("");
}

off_t getFileSize(const char *filePath)
{
	int fd = open(filePath, O_RDONLY);
	if (fd < 0)
	{
		log("open failed", ERROR);
		return -1;
	}
	off_t fileSize = lseek(fd, 0, SEEK_END);
	if (fileSize == -1)
	{
		log("lseek failed", ERROR);
		close(fd);
		return -1;
	}
	close(fd);
	return fileSize;
}