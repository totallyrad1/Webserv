#include "../../inc/Webserv.hpp"

std::string getStatusMessage(int code)
{
	switch (code)
	{
	case 100:
		return ("Continue");
	case 101:
		return ("Switching Protocol");
	case 200:
		return ("OK");
	case 201:
		return ("Created");
	case 202:
		return ("Accepted");
	case 203:
		return ("Non-Authoritative Information");
	case 204:
		return ("No Content");
	case 205:
		return ("Reset Content");
	case 206:
		return ("Partial Content");
	case 300:
		return ("Multiple Choice");
	case 301:
		return ("Moved Permanently");
	case 302:
		return ("Moved Temporarily");
	case 303:
		return ("See Other");
	case 304:
		return ("Not Modified");
	case 307:
		return ("Temporary Redirect");
	case 308:
		return ("Permanent Redirect");
	case 400:
		return ("Bad Request");
	case 401:
		return ("Unauthorized");
	case 403:
		return ("Forbidden");
	case 404:
		return ("Not Found");
	case 405:
		return ("Method Not Allowed");
	case 406:
		return ("Not Acceptable");
	case 407:
		return ("Proxy Authentication Required");
	case 408:
		return ("Request Timeout");
	case 409:
		return ("Conflict");
	case 410:
		return ("Gone");
	case 411:
		return ("Length Required");
	case 412:
		return ("Precondition Failed");
	case 413:
		return ("Payload Too Large");
	case 414:
		return ("URI Too Long");
	case 415:
		return ("Unsupported Media Type");
	case 416:
		return ("Requested Range Not Satisfiable");
	case 417:
		return ("Expectation Failed");
	case 418:
		return ("I'm a teapot");
	case 421:
		return ("Misdirected Request");
	case 425:
		return ("Too Early");
	case 426:
		return ("Upgrade Required");
	case 428:
		return ("Precondition Required");
	case 429:
		return ("Too Many Requests");
	case 431:
		return ("Request Header Fields Too Large");
	case 451:
		return ("Unavailable for Legal Reasons");
	case 500:
		return ("Internal Server Error");
	case 501:
		return ("Not Implemented");
	case 502:
		return ("Bad Gateway");
	case 503:
		return ("Service Unavailable");
	case 504:
		return ("Gateway Timeout");
	case 505:
		return ("HTTP Version Not Supported");
	case 506:
		return ("Variant Also Negotiates");
	case 507:
		return ("Insufficient Storage");
	case 510:
		return ("Not Extended");
	case 511:
		return ("Network Authentication Required");
	default:
		return ("Undefined");
	}
}


std::string generateResPage(int code)
{
	std::string color = "yellow";
	if (code >= 400 && code < 600)
		color = "red";
	else if (code >= 200 && code < 300)
		color = "green";
	

	std::string content = "<html><style>body {    font-family: 'Courier New', Courier, monospace;\nbackground-color: black;\ncolor: white;\nheight: 100vh;\nwidth: 100vw;\nmargin: 0;\ndisplay: flex;\nflex-direction: column;\njustify-content: center;\nalign-items: center;}h1 {\ncolor: " + color + ";}a {\ncolor: white;}a:hover {\ncolor: " + color + ";}div {\ntext-align: center;\nborder: 1px solid white;\npadding: 20px;\nborder-radius: 7px;\nbox-shadow: 0px 0px 60px " + color + ";}</style><body><div><h1>" + to_string(code) + " " + getStatusMessage(code) + "</h1><a href=\"https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/" +
to_string(code) + " \">  https://developer.mozilla.org/en-US/docs/Web/HTTP/Status/" + to_string(code) + "</a></div></body></html>";
	
	return content;
}