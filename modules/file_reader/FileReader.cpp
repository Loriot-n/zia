#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "FileReader.hpp"

namespace fs = boost::filesystem;

extern "C" IModule *create()
{
  return new FileReader();
}

bool FileReader::exec(zia::api::HttpDuplex &dup)
{
  if (fs::ifstream inputStream{fs::path(rootPath) / fs::path(dup.req.uri),
      std::ios::binary | std::ios::ate})
    {
      int size = inputStream.tellg();
      inputStream.seekg(0);
      dup.resp.body.resize(size);
      inputStream.read(reinterpret_cast<char *>(dup.resp.body.data()), size);
    }
  else
    {
      std::cerr << "Couldn't find file " << fs::path(dup.req.uri) / fs::path(rootPath) << std::endl;
    }
  return true;
}

bool FileReader::config(zia::api::Conf const &conf)
{
  auto root = std::get_if<std::string>(&conf.at("root").v);
  if (root)
    {
      rootPath = *root;
      return true;
    }
  return false;
}
