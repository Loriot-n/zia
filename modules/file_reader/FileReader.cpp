#include <iostream>
#include <fstream>
#include <sstream>
#include "FileReader.hpp"
#include "Config.hpp"

namespace fs = boost::filesystem;

extern "C" IModule *create()
{
  return new FileReader();
}

bool FileReader::exec(zia::api::HttpDuplex &dup)
{
  std::cout << "-------\nFILE_READER MODULE: " << std::endl;
  fs::path const target = fs::path(rootPath) / fs::path(dup.req.uri);
  std::cout << "Requesting file " << target << std::endl;
  if (fs::is_directory(target))
    {
      if (dup.req.uri.back() != '/')
	{
	  dup.resp.status = zia::api::http::common_status::moved_permanently;
	  dup.resp.reason = "Moved Permanently";
	  addHeader(dup, "Location", "http://localhost:8080" + dup.req.uri + "/");
	  return false;
	}
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
  zia::Config config(conf);
  rootPath = config.getOrDefault<std::string>("document_root", "/home/mobsil");
  if (::access(rootPath.c_str(), R_OK) != 0)
    {
      throw std::runtime_error("Cannot read directory " + rootPath.native());
    }
  path404 = config.getOrDefault<std::string>("404path", "");
  path403 = config.getOrDefault<std::string>("403path", "");
  return true;
}


void FileReader::handleDir(fs::path const &target, zia::api::HttpDuplex &dup)
{
  addHeader(dup, "Content-Type", "text/html");
  if (hasIndexHtml(target))
    {
      handleFile(target / "index.html", dup);
      return;
    }

  std::stringstream top;
  top <<
    R"|(
<!DOCTYPE html>
<html lang="en_UK">
<head>
<meta charset="UTF-8">
<title>Index of )|" << dup.req.uri <<  R"|(</title>
</head>
<body>
<h1> Index of )|" << dup.req.uri << R"|(</h1>
<table>
    )|"
    "<tr><td><a href=\"" << "../\">" << "../</a></td></tr>";
  putStringToResp(top.str(), dup.resp);
  std::cout << "Content:" << std::endl;
  for (fs::directory_entry &entry : fs::directory_iterator(target))
    {
      std::cout << "    " << entry.path() << std::endl;
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
  putStringToResp(std::string_view(
    R"|(
</table>
</body>
</html>
    )|"),
    dup.resp);
  dup.resp.status = zia::api::http::common_status::ok;
  dup.resp.reason = "OK";
}

bool FileReader::handleFile(fs::path const &target, zia::api::HttpDuplex &dup)
{
  if (fs::ifstream inputStream{target, std::ios::binary})
    {
      std::uintmax_t size = fs::file_size(target);
      dup.resp.body.resize(size);
      inputStream.read(reinterpret_cast<char *>(dup.resp.body.data()), size);
      dup.resp.status = zia::api::http::common_status::ok;
      dup.resp.reason = "OK";
      return true;
    }
  else
    {
      handleFileError(target, dup);
      return false;
    }
}

bool FileReader::hasIndexHtml(boost::filesystem::path const &targetDir) const
{
  for (fs::directory_entry &entry : fs::directory_iterator(targetDir))
    {
      if (entry.path().filename().native() == "index.html")
	return true;
    }
  return false;
}

void FileReader::handleFileError(fs::path const &target, zia::api::HttpDuplex &dup)
{
  if (!fs::exists(target))
    {
      if (handleFile(rootPath / path404, dup) == false)
	putStringToResp(FileReader::html404, dup.resp);
      dup.resp.status = zia::api::http::common_status::not_found;
      dup.resp.reason = "Not Found";
    }
  else
    {
      fs::file_status file = fs::status(target);
      if (!(file.permissions() & fs::owner_read))
	{
	  if (handleFile(rootPath / path403, dup) == false)
	    putStringToResp(FileReader::html403, dup.resp);
	  dup.resp.status = zia::api::http::common_status::forbidden;
	  dup.resp.reason = "Forbidden";
	}
    }
}

void FileReader::putStringToResp(std::string_view str, zia::api::HttpResponse &resp)
{
  std::byte const *bytes = reinterpret_cast<std::byte const *>(str.data());
  resp.body.insert(resp.body.end(), bytes, bytes + str.size());
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
