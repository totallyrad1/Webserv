#include "../../inc/Webserv.hpp"

std::string &trim(std::string &s, const std::string &tobetrimmed)
{
	size_t	end;

	if (s.empty())
		return (s);
	end = s.find_last_not_of(tobetrimmed);
	if (end != std::string::npos)
		s = s.substr(0, end + 1);
	else
		s.clear();
	return (s);
}

bool	endsWith(const std::string &str, const std::string &suffix)
{
	return (str.size() >= suffix.size() && 0 == str.compare(str.size()
			- suffix.size(), suffix.size(), suffix));
}

void	findAndReplaceAll(std::string &str, const std::string toFind,
		const std::string toReplace)
{
	size_t startPos = 0;
	if ((startPos = str.find(toFind, startPos)) != std::string::npos)
	{
		str.replace(startPos, toFind.length(), toReplace);
		startPos += toReplace.length(); // Move past the last replaced substring
	}
}

bool	findChar(const std::string &str, const std::string chars)
{
	for (size_t i = 0; i < chars.size(); i++)
	{
		if (str.find(chars[i]) != std::string::npos)
			return (true);
	}
	return (false);
}

std::string	toLower(std::string &str)
{
	std::string tmp(str);
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	return (tmp);
}

std::string toLower(const std::string str)
{
	std::string tmp(str);
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	return (tmp);
}

std::string reduce(std::string str, char c)
{
	// if there is more than one c in a row, it will be reduced to one
	std::string tmp;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == c && (i == 0 || str[i - 1] != c))
			tmp += c;
		else if (str[i] != c)
			tmp += str[i];
	}
	return tmp;
}

std::string to_string(long long i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

long Stol(const std::string &str, std::size_t *idx = (std::size_t *)0, int base = 10)
{
	const char *cstr = str.c_str();
	char *end;
	long l;
	errno = 0;
	l = std::strtol(cstr, &end, base);
	if (errno == ERANGE || l > LLONG_MAX || l < LLONG_MIN)
		throw std::out_of_range("Stol: out of range");
	if (end == cstr)
		throw std::invalid_argument("Stol: invalid argument");
	if (idx != NULL)
		*idx = end - cstr;
	return l;
}