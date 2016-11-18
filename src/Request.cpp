﻿#include "Request.hpp"
#include "Socket.hpp"
#include <sstream>
#include "Response.hpp"

Burst::Request::Request(std::unique_ptr<Socket> socket)
	: socket_(std::move(socket))
{}

bool Burst::Request::canSend() const
{
	return socket_ != nullptr && socket_->isConnected();
}

Burst::Response Burst::Request::sendPost(const std::string& url, const std::string& body, const std::string& header)
{
	if (!send(url, "POST", body, header))
		return { nullptr };
	return { close() };
}

Burst::Response Burst::Request::sendGet(const std::string& url)
{
	// empty body and head for more performance
	static const std::string body = "";
	static const std::string head = "";

	if (!send(url, "GET", body, head))
		return { nullptr };
	return { close() };
}

std::unique_ptr<Burst::Socket> Burst::Request::close()
{
	return std::move(socket_);
}

bool Burst::Request::send(const std::string& url, const std::string& method, const std::string& body, const std::string& header) const
{
	if (!canSend())
		return false;

	// TODO: \r\n to std::endl
	std::stringstream request;
	request << method << " " << url << " HTTP/1.0" << "\r\n" << header << "\r\n\r\n" << body;
	
	return socket_->send(request.str());
}
