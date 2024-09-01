#include "../../inc/Webserv.hpp"

// DEBUG
Route::Route()
{
	this->methods.clear();
	this->root_dir = "";
	this->dirlst = 0;
	this->def_file.clear();
	this->cgi_extensions.clear();
	this->upload = 0;
	this->upload_path = "";
	this->redirection = "";
	this->cgi_timeout = 30;
}


std::set<std::string> Route::get_methods() const
{
	return (this->methods);
}
std::string Route::get_roor_dir() const
{
	return (this->root_dir);
}
bool Route::get_dirlst() const
{
	return (this->dirlst);
}
std::set<std::string> Route::get_default_file() const
{
	return (this->def_file);
}
std::set<std::string> Route::get_cgi_extensions() const
{
	return (this->cgi_extensions);
}

int Route::get_cgi_timeout() const
{
	return (this->cgi_timeout);
}
bool Route::get_upload() const
{
	return (this->upload);
}
std::string Route::get_upload_path() const
{
	return (this->upload_path);
}
std::string Route::get_redirection_url() const
{
	return (this->redirection);
}

void Route::route_reset()
{
	this->methods.clear();
	this->root_dir = "";
	this->dirlst = 0;
	this->def_file.clear();
	this->cgi_extensions.clear();
	this->upload = 0;
	this->upload_path = "";
	this->redirection = "";
}
int Route::fill_attribute(const std::string &key, const std::string &value)
{
	if (key == "METHODS")
	{
		std::string tmp = value;
		std::string token;
		std::istringstream ss(tmp);
		while (std::getline(ss, token, ' '))
		{
			if(token != "GET" && token != "POST" && token != "DELETE"){
				std::cerr << RED << "invalid root allowed method : " << "["<< token << "]"<<RESET << std::endl;
				return 0;
			}else{
				this->methods.insert(token);
			}
		}
	}
	else if (key == "ROOT")
	{
		this->root_dir = value;
		if (this->root_dir[this->root_dir.length() - 1] != '/')
			this->root_dir += "/";
	}
	else if (key == "DIRLST")
	{
		if (value == "Y")
			this->dirlst = true;
		else
			this->dirlst = false;
	}
	else if (key == "DEFAULT")
	{
		std::string tmp = value;
		std::string token;
		std::istringstream ss(tmp);
		while (std::getline(ss, token, ' '))
		{
			this->def_file.insert(token);
		}
	}
	else if (key == "UPLOAD")
	{
		if (value != "")
		{
			this->upload = true;
			this->upload_path = value;
			if (this->upload_path[this->upload_path.length() - 1] != '/')
				this->upload_path += "/";
		}
		else
			this->upload = false;
	}
	else if (key == "CGI_EXT")
	{
		std::string tmp = value;
		std::string token;
		std::istringstream ss(tmp);
		while (std::getline(ss, token, ' '))
		{
			this->cgi_extensions.insert(token);
		}
	}
	else if (key == "REDIR")
		this->redirection = value;
	else if (key == "CGI_TIMEOUT")
	{
		try
		{
			this->cgi_timeout = Stol(value,0, 10);
			if (this->cgi_timeout < 0)
			{
				std::cerr << RED << "invalid cgi timeout : " << "["<< value << "]"<< RESET << std::endl;
				return 0;
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << "invalid cgi timeout : " << "["<< value << "]"<< RESET << std::endl;
			return 0;
		}
	}
	else{
		std::cerr << RED << "invalid route key : " << key << RESET << " \nonly valid keys are :\nMETHODS\nROOT\nDEFAULT\nDIRLST\nUPLOAD\nCGI_EXT\nREDIR\nCGI_TIMEOUT" << std::endl;
		return 0;
	}
	return 1;
}