#pragma once

#include <iostream>
#include "IModule.hpp"

class FileReader : public IModule
{
private:
  std::string rootPath;

  void putStringToResp(std::string const &, zia::api::HttpResponse &);

public:
  FileReader() : IModule(5) {};

  bool exec(zia::api::HttpDuplex &) override;
  bool config(zia::api::Conf const &) override;
};
