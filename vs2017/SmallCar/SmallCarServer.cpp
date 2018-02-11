#include "SmallCarServer.h"
#include <regex>
#include <cpprest/streams.h>
#include <cpprest/filestream.h>
#include <unistd.h>

SmallCarGPIO SmallCarServer::gpio;

SmallCarServer::SmallCarServer()
{
}

SmallCarServer::SmallCarServer(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&SmallCarServer::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&SmallCarServer::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&SmallCarServer::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&SmallCarServer::handle_delete, this, std::placeholders::_1));
}


SmallCarServer::~SmallCarServer()
{
}

void SmallCarServer::handle_get(http_request message)//If request has empty body, means he want the index.html
{
	const int num = 5;
	ucout << message.to_string() << std::endl;

	//message.reply(status_codes::OK);
	http_response message_response;

	message_response.headers().set_content_type("text/html");
	utility::string_t currentpath = get_current_dir_name();
	currentpath += "/index.html";
	ucout << currentpath << std::endl;

	try {
		Concurrency::streams::file_stream<uint8_t>::open_istream(currentpath).then([&message_response](Concurrency::streams::basic_istream<uint8_t> response_basic_istream) {
			message_response.set_body(response_basic_istream);
		});
	}catch(std::exception const &e){
		std::wcout << e.what() << std::endl;
	}
	/*
	message_response.set_body(U("<html>\
<title>Test Demo</title>\
<body><h1>Hellow World</h1><br/>\
<form action=\"\" method=\"POST\" target=\"nn\" enctype=\"text/plain\">\
<input type=\"text\" name=\"delayTime\"value=\"5\"/><br/><br/>\
<input type=\"submit\" value=\"Yellow LED Flash\"/></form></body>\
<iframe id=\"idid\" name=\"nn\" style=\"display:none;\" />\
</html>"));
*/
	message_response.set_status_code(status_codes::OK);

	//reply with the second response
	http_response message_response_2th;
	message_response_2th.headers().set_content_type("text/html");

	//Print response and replay to the request
	ucout << message_response.to_string() << std::endl;
	message.reply(message_response);

	gpio.ledFlash(num, 1);
}

void SmallCarServer::handle_put(http_request message)//If request has content body
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void SmallCarServer::handle_post(http_request message)
{
	const int num = 6;
	ucout << message.to_string() << std::endl;
	std::string temp = message.extract_string(true).get();

	//use regex to get delaytime
	std::regex tempRegex(R"([0-9]{1,2})");
	std::cmatch tempSm;

	message.reply(status_codes::OK);
	if (std::regex_search(temp.c_str(), tempSm, tempRegex)) {
		ucout << tempSm.size() << std::endl;
		ucout << tempSm[0].length() << std::endl;
		ucout << tempSm[0].str() << std::endl;
	}
	if (tempSm.size() > 0) {
		gpio.ledFlash(num, std::atoi(tempSm[0].str().c_str()));
	}
}

void SmallCarServer::handle_delete(http_request message)
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}
