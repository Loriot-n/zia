#pragma once

#include <iostream>
#include "IModule.hpp"

class Response : public IModule
{
public:
  Response() : IModule(0) {};

  bool exec(zia::api::HttpDuplex &) override;
  bool config(zia::api::Conf const &) override;
};
