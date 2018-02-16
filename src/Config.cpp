#include <fstream>
#include "Config.hpp"

namespace zia
{
    Config::Config(std::string const &file)
      : filename(file)
    {
      load();
    }

    api::ConfArray Config::getArrayFromJson(Json::Value const &json) const
    {
      api::ConfArray array;

      for (Json::Value value : json)
	{
	  addToContainer(value,
			 [&array](auto const &castedValue)
			 {
			   array.emplace_back(castedValue);
			 });
	}
      return array;
    }

    api::ConfObject Config::getObjectFromJson(Json::Value const &json) const
    {
      api::ConfObject object;

      for (std::string const &name : json.getMemberNames())
	{
	  Json::Value value = json[name];
	  addToContainer(value,
			 [&object, &name](auto const &castedValue)
			 {
			   object.emplace(std::make_pair(name, castedValue));
			 });
	}
      return object;
    }

    void Config::load()
    {
      Json::Value root;
      Json::CharReaderBuilder builder;
      std::string errs;
      std::ifstream file(filename);

      if (Json::parseFromStream(builder, file, &root, &errs))
	{
	  conf = getObjectFromJson(root);
	}
      else
	{
	  throw std::runtime_error(errs);
	}
    }

    api::ConfObject const &Config::getConf() const
    {
      return conf;
    }

    api::ConfObject &Config::getConf()
    {
      return conf;
    }


}
