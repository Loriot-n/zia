#pragma once

#include <iostream>
#include <boost/filesystem.hpp>
#include "IModule.hpp"

namespace fs = boost::filesystem;

class FileReader : public IModule
{
public:
  FileReader() : IModule(5) {};

  bool exec(zia::api::HttpDuplex &) override;
  bool config(zia::api::Conf const &) override;

private:
  std::string rootPath;

  void handleDir(fs::path const &target, zia::api::HttpDuplex &);
  void handleFile(fs::path const &target, zia::api::HttpDuplex &);
  bool hasIndexHtml(fs::path const &targetDir) const;
  void handleFileError(fs::path const &target, zia::api::HttpDuplex &);

  void putStringToResp(std::string const &, zia::api::HttpResponse &);
  void addHeader(zia::api::HttpDuplex &, std::string const &key, std::string const &value);
};
