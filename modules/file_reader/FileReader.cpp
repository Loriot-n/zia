#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include "FileReader.hpp"

namespace fs = boost::filesystem;

extern "C" IModule *create()
{
  return new FileReader();
}

void FileReader::putStringToResp(std::string const &str, zia::api::HttpResponse &resp)
{
  std::byte const *bytes = reinterpret_cast<std::byte const *>(str.c_str());
  resp.body.insert(resp.body.end(), bytes, bytes + str.size());
}

bool FileReader::exec(zia::api::HttpDuplex &dup)
{
  for (std::byte b : dup.raw_req)
    {
      std::cout << static_cast<char>(b);
    }
  std::cout << std::endl;
  fs::path const target = fs::path(rootPath) / fs::path(dup.req.uri);
  std::cout << "Requesting file " << rootPath << " / " << dup.req.uri << std::endl;
  if (fs::is_directory(target))
    {
      dup.resp.headers.emplace(std::make_pair("Content-Type", "text/html"));

      std::stringstream top;
      top <<
	"<!DOCTYPE html>\n"
	"<html lang=\"en_UK\">\n"
	"<head>\n"
	"<meta charset=\"UTF-8\">\n"
	"<title>Index of /" << target.filename().native() << "</title>\n"
	"</head>\n"
	"<body>\n"
	"<table>\n"
	"<tr><td><a href=\"" << "../\">" << "../</a></td></tr>\n";
      putStringToResp(top.str(), dup.resp);
      for (fs::directory_entry &entry : fs::directory_iterator(target))
	{
	  std::cout << entry.path() << std::endl;
	  std::stringstream ss;
	  ss << "<tr><td><a href=\"" << entry.path().filename().native();
	  if (fs::is_directory(entry))
	    ss << '/';
	  ss << "\">" << entry.path().filename().c_str();
	  if (fs::is_directory(entry))
	    ss << '/';
	  ss << "</a></td></tr>\r\n";
	  putStringToResp(ss.str(), dup.resp);
	}
      putStringToResp(
	"</table>\n"
	"</body>\n"
	"</html>\n",
	dup.resp);
    }
  else if (fs::ifstream inputStream{target,
      std::ios::binary | std::ios::ate})
    {
      int size = inputStream.tellg();
      inputStream.seekg(0);
      dup.resp.body.resize(size);
      inputStream.read(reinterpret_cast<char *>(dup.resp.body.data()), size);
    }
  else
    {
      std::cerr << "Couldn't find file " << target << std::endl;
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
