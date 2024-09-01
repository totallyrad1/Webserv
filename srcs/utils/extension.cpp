#include "../../inc/Webserv.hpp"

std::string getExtenson(std::string file)
{
	std::string extension = file.substr(file.find_last_of(".") + 1);
	return (extension);
}

std::string getType(std::string path)
{
	std::string extension = getExtenson(path);
	if (extension == "html")
		return ("text/html");
	else if (extension == "css")
		return ("text/css");
	else if (extension == "js")
		return ("text/javascript");
	else if (extension == "jpg")
		return ("image/jpeg");
	else if (extension == "jpeg")
		return ("image/jpeg");
	else if (extension == "png")
		return ("image/png");
	else if (extension == "gif")
		return ("image/gif");
	else if (extension == "bmp")
		return ("image/bmp");
	else if (extension == "ico")
		return ("image/x-icon");
	else if (extension == "svg")
		return ("image/svg+xml");
	else if (extension == "mp3")
		return ("audio/mpeg");
	else if (extension == "mp4")
		return ("video/mp4");
	else if (extension == "webm")
		return ("video/webm");
	else if (extension == "ogg")
		return ("audio/ogg");
	else if (extension == "wav")
		return ("audio/wav");
	else if (extension == "avi")
		return ("video/x-msvideo");
	else if (extension == "mpeg")
		return ("video/mpeg");
	else if (extension == "txt")
		return ("text/plain");
	else if (extension == "pdf")
		return ("application/pdf");
	else if (extension == "doc")
		return ("application/msword");
	else if (extension == "docx")
		return ("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	else if (extension == "xls")
		return ("application/vnd.ms-excel");
	else if (extension == "xlsx")
		return ("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	else if (extension == "ppt")
		return ("application/vnd.ms-powerpoint");
	else if (extension == "pptx")
		return ("application/vnd.openxmlformats-officedocument.presentationml.presentation");
	else if (extension == "zip")
		return ("application/zip");
	else if (extension == "tar")
		return ("application/x-tar");
	else if (extension == "gz")
		return ("application/gzip");
	else if (extension == "bz2")
		return ("application/x-bzip2");
	else if (extension == "7z")
		return ("application/x-7z-compressed");
	else if (extension == "rar")
		return ("application/x-rar");
	else
		return ("application/octet-stream");
}