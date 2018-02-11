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
	ucout << message.to_string() << std::endl;

	utility::string_t currentpath = get_current_dir_name();
	//Client needs html and javascript code to interact with server
	//Return two or more files back to client, each with a HTTP frame

	//reply with the first response, return html code to client
	http_response message_response;
	message_response.headers().set_content_type("text/html");
	utility::string_t index_html_path = currentpath + "/index.html";
	//ucout << currentpath << std::endl;

	try {
		Concurrency::streams::file_stream<uint8_t>::open_istream(index_html_path).then([&message_response](Concurrency::streams::basic_istream<uint8_t> response_basic_istream) {
			message_response.set_body(response_basic_istream);
		});
	}
	catch (std::exception const &e) {
		std::wcout << "Exception: " << e.what() << std::endl;
		//Error occured, internal error return.
		message_response.set_status_code(status_codes::InternalError);
		message.reply(message_response);
		return;
	}
	message_response.set_status_code(status_codes::OK);

	//reply with the second response, return javascript code to client
	http_response message_response_code;
	message_response_code.headers().set_content_type("text/javascript");
	utility::string_t js_path = currentpath + "/libdemo.js";
	try {
		Concurrency::streams::file_stream<uint8_t>::open_istream(js_path).then([&message_response_code](Concurrency::streams::basic_istream<uint8_t> response_basic_istream) {
			message_response_code.set_body(response_basic_istream);
		});
	}
	catch (std::exception const &e) {
		std::wcout << "Exception: " << e.what() << std::endl;
		//Error occured, internal error return.
		message_response_code.set_status_code(status_codes::InternalError);
		message.reply(message_response_code);
		return;
	}
	message_response_code.set_status_code(status_codes::OK);

	//Print response and replay to the request
	ucout << message_response.to_string() << std::endl;
	message.reply(message_response);
	message.reply(message_response_code);

	const int num = 5;
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
