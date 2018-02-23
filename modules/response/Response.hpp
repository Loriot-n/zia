#pragma once

#include <iostream>
#include "IModule.hpp"

class Response : public IModule
{
public:
  Response() : IModule(0) {};

  bool exec(zia::api::HttpDuplex &) override;
  bool config(zia::api::Conf const &) override;

private:
  int sendStatusLine(zia::api::HttpDuplex &dup);
  int sendHeaders(zia::api::HttpDuplex &dup);
  int sendBody(zia::api::HttpDuplex &dup);

  template <class T>
  int sendRaw(T const &buffer, int size, zia::api::HttpDuplex &dup)
  {
    return dup.info.sock->stream.send(buffer, size);
  }

  template <class T>
  int sendContainer(T const &container, zia::api::HttpDuplex &dup)
  {
    return dup.info.sock->stream.send(container.data(), container.size());
  }

  template<class Func>
  auto stringToBytes(std::string const &str, Func &&func)
  {
    std::byte const *bytes = reinterpret_cast<std::byte const *>(str.c_str());
    return func(bytes, str.size());
  }
};

std::ostream &operator<<(std::ostream &os, zia::api::http::Version version)
{
  switch (version)
    {
    case zia::api::http::Version::http_0_9:
      os << "HTTP/0.9";
      break;
    case zia::api::http::Version::http_1_0:
      os << "HTTP/1.0";
      break;
    case zia::api::http::Version::http_1_1:
      os << "HTTP/1.1";
      break;
    case zia::api::http::Version::http_2_0:
      os << "HTTP/2.0";
      break;
    case zia::api::http::Version::unknown:
      throw std::runtime_error("unknown http version");
    }
  return os;
}
