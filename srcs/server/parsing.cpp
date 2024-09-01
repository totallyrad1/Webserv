#include "../../inc/Webserv.hpp"

int value_valid_chars(char c){
	std::string valid = "'0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,.=/ _:-";
	int i = 0;
	while(valid[i]){
		if(c == valid[i]){
			return 1;
		}
		i++;
	}
	return 0;
}

int keyword_valid_chars(char c){
	std::string valid = "':abcdefghijklmnopqrstuvwxyz_";
	int i = 0;
	while(valid[i]){
		if(c == valid[i]){
			return 1;
		}
		i++;
	}
	return 0;
}

int check_syntax(std::string line){
	int i = 0;
	if(line[i] == ' ' || line[i] == '\t'){
		i++;
	}else{
		std::cerr << RED << "gonna ignore this server block because of bad syntax at line " << RESET <<std::endl << "[" << line << "]" << std::endl;
		return 0;
	}
	////****** keyword check*****/////
	while(line[i] && line[i] != ' ' && line[i] != '\t'){
		if(keyword_valid_chars(line[i]) == 0){
			std::cerr << RED<< "gonne ignore this server block because of bad char"<< "'"<<  line[i] <<"'"<<"found at keyword in line" << RESET << std::endl << "[" << line << "]" << std::endl;
			return 0;
		}
		i++;
	}
	if(line[i - 1] != ':'){
		std::cerr << RED<< "u are sure u closed this keyword with a ':' ?" << RESET << std::endl << "[" << line << "]" << std::endl;
		return 0;
	}
	//***** delimiter after keyowrd
	if(line[i] == ' ' || line[i] == '\t'){
		i++;
	}else{
		std::cerr << RED << "gonna ignore this server block because of bad syntax at line " << RESET <<std::endl << "[" << line << "]" << std::endl;
		return 0;
	}
	//+******* value check *******//
	while(line[i] && line[i] != ';'){
		if(value_valid_chars(line[i]) == 0){
			std::cerr << RED<< "gonne ignore this server block because of bad char"<< "'"<<  line[i] <<"'"<<"found at value in line" << RESET << std::endl << "[" << line << "]" << std::endl;
			return 0;
		}
		i++;
	}
	if(line[i] != ';'){
		std::cerr << RED<< "u are sure u closed this line with a ';' ?" << RESET << std::endl << "[" << line << "]" << std::endl;
		return 0;
	}
	return 1;
}

int fill_host(Config &config, std::string value){
	int i = 0;
	int number;
	int tmpend;
	int tmpstart;
	std::string strnum;
	int ncount = 0;
	while(value[i] && ncount < 4){
		tmpstart = i;
		while(value[i] && value[i] != '.')
			i++;
		tmpend = i;
		strnum = value.substr(tmpstart, tmpend - tmpstart);
		std::stringstream ss(strnum);
		ss >> number;
		if(!ss.fail() && ss.eof()){
			if(number < 0 || number > 255){
				std::cerr << RED << "invalid number range 0-255 only" << RESET << std::endl;
				return 0;
			}
			ncount++;
		}else{
			std::cerr << RED << "invalid host value 2" << RESET << std::endl;
			return 0;
		}
		if(value[i] == '.')
			i++;
	}
	if(!value[i] && ncount == 4){
		config.set_host(value);
		return 1;
	}else{
		std::cerr << RED << "invalid host value" << RESET << std::endl;
		return 0;
	}
}

int fill_allowed_methods(Config &config, std::string value){
	std::set<std::string> allowed_methods;
	std::stringstream ss(value);
	std::string method;
	while(std::getline(ss, method, ',')){
		if(method != "GET" && method != "POST" && method != "DELETE"){
			std::cerr << RED << "invalid allowed method : " << "["<< method << "]"<<RESET << std::endl;
			return 0;
		}else{
			allowed_methods.insert(method);
		}
	}
	config.set_allowed_methods(allowed_methods);
	return 1;
}

int fill_ports(Config &config, std::string value){
	std::set<int> ports;
	std::stringstream ss(value);
	std::string port;
	int number;
	while(std::getline(ss, port, ',')){
		std::stringstream ss1(port);
		ss1 >> number;
		if(!ss1.fail() && ss1.eof()){
			if(number >= 1024 && number <= 65535)
				ports.insert(number);
			else
				std::cerr << RED << "invalid port : " << "["<< number << "] port range is between 1024 and 65535"<< RESET << std::endl;
		}else{
			std::cerr << RED << "invalid port : " << "["<< port << "]"<< RESET << std::endl;
			return 0;
		}
	}
	config.set_ports(ports);
	return 1;
}

int fill_max_body_size(Config &config, std::string value){
	unsigned long max_body_size;
	std::stringstream ss(value);

	ss >> max_body_size;
	if(!ss.fail() && ss.eof()){
		config.set_max_body_size(max_body_size);
		config.set_has_max_body_size(1);
		return 1;
	}else{
		config.set_has_max_body_size(0);
		std::cerr << RED << "invalid max_body_size : " << "["<< value << "]"<< RESET << std::endl;
		return 0;
	}
}

int fill_server_names(Config &config, std::string value){
	std::set<std::string> server_names;
	std::stringstream ss(value);
	std::string server_name;
	while(getline(ss, server_name, ',')){
		server_names.insert(server_name);
	}
	config.set_servernames(server_names);
	return 1;
}

int fill_default_error_pages(Config &config, std::string value){
	std::map<int, std::string> default_error_pages;
	int number;
	std::string strnum;
	std::string error_path;
	std::stringstream ss(value);
	std::string codeandvalue;
	while(getline(ss, codeandvalue, ',')){
		if(codeandvalue.empty()){
			std::cerr << RED << "invalid default error page " << RESET << std::endl;
			return 0;
		}
		std::stringstream ss1(codeandvalue);
		getline(ss1, strnum, '=');
		std::stringstream ss2(strnum);
		ss2 >> number;
		if(!ss2.fail() && ss2.eof()){
			if(number < 400 || number > 599){
				std::cerr << RED << "invalid error number :" << "[" << number << "]" << RESET << std::endl;
				return 0;
			}
		}else{
			std::cerr << RED << "2invalid error number :" << "[" << strnum << "]" << RESET << std::endl;
		}
		getline(ss1, error_path);
		if(error_path.empty()){
			std::cerr << RED << "invalid error path for " << "[" << number << "]" << RESET << std::endl;
			return 0;
		}
		default_error_pages.insert(std::make_pair(number, error_path));
	}
	config.set_errorpages(default_error_pages);
	return 1;
}

int fill_routes(Config &config, std::string value){
	Route routeobj;
	std::string route;
	std::stringstream ss(value);
	std::string keyandvalue;
	std::string key;
	std::string value1;
	getline(ss, route, '=');
	if(route[0] != '/'){
		std::cerr << RED << "invalid route [" << route << "]" << RESET << std::endl;
		return 0;
	}
	while(getline(ss, keyandvalue, ',')){
		std::stringstream ss1(keyandvalue);
		if(ss1.fail()){
			std::cerr << RED << "invalid key and value [" << keyandvalue << "]" << RESET << std::endl;
			return 0;
		}
		getline(ss1, key, '=');
		getline(ss1, value1);
		if(routeobj.fill_attribute(key, value1) == 0){
			return 0;
		}
	}
	config.insert_route(route, routeobj);
	return 1;
}

int fill_config(Config &config, std::string keyword, std::string value){
	if(keyword == "host"){
		if(fill_host(config, value) == 0)
			return 0;
	}else if(keyword == "allowed_methods"){
		if(fill_allowed_methods(config, value) == 0)
			return 0;
	}else if(keyword == "ports"){
		if(fill_ports(config, value) ==0 )
			return 0;
	}else if(keyword == "max_body_size"){
		if(fill_max_body_size(config, value) == 0)
			return 0;
	}else if(keyword == "server_names"){
		fill_server_names(config, value);
	}else if(keyword == "default_error_pages"){
		if(fill_default_error_pages(config,value) == 0)
			return 0;
	}else if(keyword == "route"){
		if(fill_routes(config, value) == 0)
			return 0;
	}else{
		std::cerr << RED << "invalid keyword : " << keyword << RESET << " \nonly valid keywords are :\nhost\nallowed_methods\nports\nmax_body_size\nserver_names\ndefault_error_pages\nroute" << std::endl;
		return 0;
	}
	return 1;
}

void make_tokens_and_insert_server_if_valid(Server &servers, const std::string &config){
	(void)servers;
	std::vector<std::string> tokens;
	int i = 0;
	int tmp;
	int stop = 0;
	//////*************making tokens ***************////////////////
	while(config[i] && !stop){
		if(config[i] == '\n'){
			i++;
			tmp = i;
			while(config[i] && config[i] != '\n' && !stop){
				if(config[i] == '}')
					stop = 1;
				i++;
			}
			if(!stop)
				tokens.push_back(config.substr(tmp, i - tmp));
		}else
			i++;
	}
	/////////////////////////////////////////////////////////////////

	//****** syntax check****//////
	std::vector<std::string>::iterator it = tokens.begin();
	while(it != tokens.end()){
		if(check_syntax(*it) == 0){
			return ;
		}
		++it;
	}
	/////////////////////////////////////////

	////////**** extracting keyword and value and filling into config from line********//////////////////
	std::string keyword;
	std::string value;
	std::string line;
	Config server_config;
	int start;
	int end;
	std::vector<std::string>::iterator it1 = tokens.begin();
	while(it1 != tokens.end()){
		i = 0;
		line = *it1;
		i++; // skip the first space in line
		start = i;
		while(line[i] && keyword_valid_chars(line[i]))
			i++;
		end = i - 1;
		if(start < end){
			keyword = line.substr(start, end - start);
		}else{
			keyword = "";
		}
		if(keyword == ""){
			std::cerr << RED << "invalid keyword gonna stop parsing this server block and prob gonna ignore it" << RESET << std::endl;
		}
		i++; //skipping space after : value
		start = i;
		while(line[i] && value_valid_chars(line[i]) && line[i] != ';')
			i++;
		end = i;
		if(start < end){
			value = line.substr(start, end - start);
		}else{
			value = "";
		}
		if(value == ""){
			std::cerr << RED << "invalid keyword gonna stop parsing this server block and prob gonna ignore it" << RESET << std::endl;
		}
		if(fill_config(server_config, keyword, value) == 0){
			return ;
		}
		++it1;
	}
	if(server_config.getPorts().empty() || server_config.getHost().empty() || server_config.getRoutes().empty()){
		std::cerr << RED << "server must have a host and a port and a route configured" << RESET << std::endl;
	}else{
		servers.insert_server(server_config);
	}
}

void parsing(Server &servers, std::string config_path)
{
	std::string currline;
	std::string server_config_to_parse;
	std::ifstream in;
	int count;

	count = 0;
	in.open(config_path.c_str());
	if(!in)
	{
		std::cerr << "unable to open " << config_path << std::endl;
		exit(1);
	}
	while(!in.eof())
	{
		std::getline(in, currline);
		server_config_to_parse += currline + "\n";
		if(currline.find('{') != std::string::npos && count == 0)
			count++;
		if(currline.find('}') != std::string::npos && count == 1)
			count++;
		if(count == 2)
		{
			count = 0;
			make_tokens_and_insert_server_if_valid(servers, server_config_to_parse);
			server_config_to_parse = "";
		}
		else if(count == 0 && currline.find('{') == std::string::npos){
			server_config_to_parse = "";
			std::cerr << RED << "Bad line in Config file make sure every config has { in the start and } in the end" << RESET <<std::endl;
		}
	}
	if(count == 1){
		std::cout << RED << "u probably forgot to close a server {} im gonna ignore that server block"<< RESET << std::endl;
	}
	in.close();
}
