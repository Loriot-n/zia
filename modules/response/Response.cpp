#include <iostream>
#include <sstream>
#include "Response.hpp"

extern "C" IModule *create()
{
  return new Response();
}

int Response::sendStatusLine(zia::api::HttpDuplex &dup)
{
  std::stringstream ss;
  ss << dup.resp.version << ' ' << dup.resp.status << ' ' << dup.resp.reason << "\r\n";
  return sendContainer(ss.str(), dup);
}

int Response::sendHeaders(zia::api::HttpDuplex &dup)
{
  int size_sent = 0;
  for (std::pair<std::string, std::string> &&header : dup.resp.headers)
    {
      std::stringstream ss;
      ss << header.first << ": " << header.second;
      std::cout << ss.str() << std::endl;
      size_sent += stringToBytes(ss.str(),
				   [&dup, this] (std::byte const *bytes, int size)
				   {
					return sendRaw(bytes, size, dup);
				   });
    }
  size_sent += sendContainer(std::string_view("\r\n\r\n"), dup);
  return size_sent;
}

int Response::sendBody(zia::api::HttpDuplex &dup)
{
  return dup.info.sock->stream.send(dup.resp.body.data(), dup.resp.body.size())
    + sendContainer(std::string_view("\r\n\r\n"), dup);
}

bool Response::exec(zia::api::HttpDuplex &dup)
{
  dup.resp.version = zia::api::http::Version::http_1_1;
  dup.resp.status = 200;
  dup.resp.reason = "OK";

  sendStatusLine(dup);
  sendHeaders(dup);
  sendBody(dup);
  return true;
}

bool Response::config(zia::api::Conf const &)
{
  return true;
}
