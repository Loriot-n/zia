#pragma once

#include <iostream>
#include <boost/filesystem.hpp>
#include <string_view>
#include "IModule.hpp"

namespace fs = boost::filesystem;

class FileReader : public IModule
{
public:
  FileReader() : IModule(5) {};

  bool exec(zia::api::HttpDuplex &) override;
  bool config(zia::api::Conf const &) override;

private:
  fs::path rootPath;
  fs::path path404;
  fs::path path403;

  void handleDir(fs::path const &target, zia::api::HttpDuplex &) const;
  bool handleFile(fs::path const &target, zia::api::HttpDuplex &) const;
  bool hasIndexHtml(fs::path const &targetDir) const;
  void handleFileError(fs::path const &target, zia::api::HttpDuplex &) const;

  void send404(zia::api::HttpDuplex &) const;
  void send403(zia::api::HttpDuplex &) const;

  void putStringToResp(std::string const &, zia::api::HttpResponse &) const;
  void putStringToResp(std::string_view, zia::api::HttpResponse &) const;
  void addHeader(zia::api::HttpDuplex &, std::string const &key, std::string const &value) const;

  static constexpr std::string_view const html404 = R"|(
<!DOCTYPE html>
<html>
<head>
<title>Object not found!</title>
<style type="text/css"><!--/*--><![CDATA[/*><!--*/
body { color: #000000; background-color: #FFFFFF; }
a:link { color: #0000CC; }
p {margin-left: 3em;}
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
</body>
</html>
)|";


  static constexpr std::string_view const html403 = R"|(
<!DOCTYPE html>
<html>
<head>
<title>Access forbidden!</title>
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
</body>
</html>
)|";


};
