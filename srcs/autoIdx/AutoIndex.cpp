#include "../../inc/Webserv.hpp"

AutoIndex::AutoIndex(std::string path, std::string pub_path) : _path(path), _pub_path(pub_path)
{
}

AutoIndex::~AutoIndex()
{
}

std::string AutoIndex::getIndex()
{
    std::string index = "<html>\n<head>\n<title>Index of " + _path + "</title>\n</head>\n<style>\
body{background-color:black;\ncolor: white;\n}\na{\nfont-size:1rem;\npadding: 5px;\ntext-dec\
oration: none;\ncolor: white;\nborder: .1px solid white;\nwidth:90%;\nalign-self: center;\no\
verflow: hidden;}a:hover {\nbackground-color: white;\ncolor: black;}div {\nmargin: auto;\npa\
dding-bottom: 1rem;\nwidth: 80%;\ndisplay: flex;\nflex-direction: column;\nalign-items:basel\
ine;\nborder: 1px solid white;}h1 {\nalign-self: center; width:90%;\noverflow:hidden;}</styl\
e>\n<body>\n<div>\n<h1>Index of " + _pub_path + "</h1>\n";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(_path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (_pub_path[_pub_path.size() - 1] != '/')
                _pub_path += "/";
            if (ent->d_name[0] == '.' && (ent->d_name[1] != '.' || ent->d_name[2] != '\0'))
                continue;
            if (ent->d_type == DT_DIR)
                index += "<a href=\"" + _pub_path + ent->d_name + "/\">" + ent->d_name + "/</a>\n";
            else
                index += "<a href=\"" + _pub_path  + ent->d_name + "\">" + ent->d_name + "</a>\n";
        }
        closedir(dir);
    }
    index += "</div>\n</body>\n</html>";
    return index;
}
