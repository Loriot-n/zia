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
  std::cout << "-------\nFILE_READER MODULE: " << std::endl;
  fs::path target = fs::path(rootPath) / fs::path(dup.req.uri);
  std::cout << "Requesting file " << target << std::endl;
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
  auto root = std::get_if<std::string>(&conf.at("document_root").v);
  if (root)
    {
      rootPath = *root;
      return true;
    }
  return false;
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
    R"|(
    <!DOCTYPE html>
    <html lang="en_UK">
    <head>
    <meta charset="UTF-8">
    <title>Index of /)|" <<  R"|(</title>
    </head>
    <body>
    <h1> Index of /)|" << R"|(</h1>
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
  putStringToResp(
    R"|(
    </table>
    </body>
    </html>
    )|",
    dup.resp);
  dup.resp.status = zia::api::http::common_status::ok;
  dup.resp.reason = "OK";
}

void FileReader::handleFile(fs::path const &target, zia::api::HttpDuplex &dup)
{
  if (fs::ifstream inputStream{target, std::ios::binary})
    {
      addHeader(dup, "Content-Type", "text/plain");
      std::uintmax_t size = fs::file_size(target);
      dup.resp.body.resize(size);
      inputStream.read(reinterpret_cast<char *>(dup.resp.body.data()), size);
      dup.resp.status = zia::api::http::common_status::ok;
      dup.resp.reason = "OK";
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
      if (entry.path().filename().native() == "index.html")
	return true;
    }
  return false;
}

void FileReader::handleFileError(fs::path const &target, zia::api::HttpDuplex &dup)
{
  if (!fs::exists(target))
    {
      dup.resp.status = zia::api::http::common_status::not_found;
      dup.resp.reason = "Not Found";
      putStringToResp(R"|(
<!DOCTYPE html>
<html>
<head>
<title>Object not found!</title>
<link rev="made" href="mailto:you@example.com" />
<style type="text/css"><!--/*--><![CDATA[/*><!--*/
body { color: #000000; background-color: #FFFFFF; }
a:link { color: #0000CC; }
p, address {margin-left: 3em;}
span {font-size: smaller;}
/*]]>*/--></style>
</head>

<body>
<h1>Object not found!</h1>
<p>


The requested URL was not found on this server.



If you entered the URL manually please check your
spelling and try again.



</p>
<p>
If you think this is a server error, please contact
the <a href="mailto:you@example.com">webmaster</a>.

</p>

<h2>Error 404</h2>
</address>
</body>
</html>

)|",
      dup.resp);
    }
  else
    {
      fs::file_status file = fs::status(target);
      if (!(file.permissions() & fs::owner_read))
	{
	  putStringToResp(R"|(
<!DOCTYPE html>
<html>
<head>
<title>Access forbidden!</title>
<link rev="made" href="mailto:you@example.com" />
<style type="text/css"><!--/*--><![CDATA[/*><!--*/
body { color: #000000; background-color: #FFFFFF; }
a:link { color: #0000CC; }
p, address {margin-left: 3em;}
span {font-size: smaller;}
/*]]>*/--></style>
</head>
<body>
<h1>Access forbidden!</h1>
<p>
You don't have permission to access the requested object.
It is either read-protected or not readable by the server.
</p>
<p>
If you think this is a server error, please contact
the <a href="mailto:you@example.com">webmaster</a>.
</p>
<h2>Error 403</h2>
</body>)|",
	  dup.resp);
	  dup.resp.status = zia::api::http::common_status::forbidden;
	  dup.resp.reason = "Forbidden";
	}
    }
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
