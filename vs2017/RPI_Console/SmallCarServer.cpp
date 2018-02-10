#include "SmallCarServer.h"

SmallCarGPIO SmallCarServer::gpio;

SmallCarServer::SmallCarServer()
{
}

SmallCarServer::SmallCarServer(utility::string_t url):m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&SmallCarServer::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&SmallCarServer::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&SmallCarServer::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&SmallCarServer::handle_delete, this, std::placeholders::_1));
}


SmallCarServer::~SmallCarServer()
{
}

void SmallCarServer::handle_get(http_request message)
{
	const int num = 5;
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
	gpio.ledFlash(num);
}

void SmallCarServer::handle_put(http_request message)
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void SmallCarServer::handle_post(http_request message)
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}

void SmallCarServer::handle_delete(http_request message)
{
	ucout << message.to_string() << std::endl;
	message.reply(status_codes::OK);
}
