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
			   array.push_back(api::ConfV {castedValue});
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

      bool ok = Json::parseFromStream(builder, file, &root, &errs);
      conf = getObjectFromJson(root);
    }

    api::ConfObject const &Config::getConf() const
    {
      return conf;
    }
}
