#include <boost/property_tree/json_parser.hpp>
#include "Config.hpp"

namespace zia
{
    Config::Config(std::string const &file)
      : filename(file)
    {
      load();
    }

    Config::~Config()
    {

    }

    void Config::load()
    {
      try
	{
	  bpt::ptree root;
	  bpt::read_json(filename, root);

	  add<long long>("oui", root);
	  std::cout << get<long long>("oui") << std::endl;
	  add<api::ConfArray>("non", root);
	  for (auto const &e : get<api::ConfArray>("non"))
	    std::cout << std::get<long long>(e.v) << std::endl;
	}
      catch (std::runtime_error const &e)
	{
	  std::cerr << e.what() << std::endl;
	}
    }
}

