#include <iostream>
#include <fstream>
#include <sstream>
#include "FileReader.hpp"

namespace fs = boost::filesystem;

extern "C" IModule *create()
{
  return new FileReader();
}

bool FileReader::exec(zia::api::HttpDuplex &dup)
{
  fs::path const target = fs::path(rootPath) / fs::path(dup.req.uri);
  std::cout << "Requesting file " << rootPath << " / " << dup.req.uri << std::endl;
  if (fs::is_directory(target))
    {
      handleDir(target, dup);
    }
  else
    {
      handleFile(target, dup);
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

void FileReader::putStringToResp(std::string const &str, zia::api::HttpResponse &resp)
{
  std::byte const *bytes = reinterpret_cast<std::byte const *>(str.c_str());
  resp.body.insert(resp.body.end(), bytes, bytes + str.size());
}

void FileReader::addHeader(zia::api::HttpDuplex &dup,
			   std::string const &key, std::string const &value)
{
  dup.resp.headers.emplace(std::make_pair(key, value));

}

void FileReader::handleDir(fs::path const &target, zia::api::HttpDuplex &dup)
{
  addHeader(dup, "Content-Type", "text/html");
  if (hasIndexHtml(target))
    {
      return handleFile(target / "index.html", dup);
    }

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

void FileReader::handleFile(fs::path const &target, zia::api::HttpDuplex &dup)
{
  if (fs::ifstream inputStream{target, std::ios::binary})
    {
      dup.resp.headers.emplace(std::make_pair("Content-Type", "text/plain"));
      std::uintmax_t size = fs::file_size(target);
      dup.resp.body.resize(size);
      inputStream.read(reinterpret_cast<char *>(dup.resp.body.data()), size);
    }
  else
    {
      handleFileError(target, dup);
    }
}

bool FileReader::hasIndexHtml(boost::filesystem::path const &targetDir) const
{
  for (fs::directory_entry &entry : fs::directory_iterator(targetDir))
    {
      std::cout << "checking " << entry.path().filename().native() << std::endl;
      if (entry.path().filename().native() == "index.html")
	return true;
    }
  return false;
}

void FileReader::handleFileError(fs::path const &target, zia::api::HttpDuplex &)
{
  std::cerr << target << " not found" << std::endl;
}
