#include <iostream>
#include "Response.hpp"

extern "C" IModule *create()
{
  return new Response();
}

bool Response::exec(zia::api::HttpDuplex &dup)
{
  dup.info.sock->stream.send(dup.resp.body.data(), dup.resp.body.size());
  return true;
}

bool Response::config(zia::api::Conf const &)
{
  return true;
}
