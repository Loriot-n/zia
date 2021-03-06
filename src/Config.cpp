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
      std::ifstream file(filename);

      if (file)
	{
	  Json::Value root;
	  Json::CharReaderBuilder builder;
	  std::string errs;

	  if (Json::parseFromStream(builder, file, &root, &errs))
	    {
	      conf = getObjectFromJson(root);
	    }
	  else
	    {
	      throw std::runtime_error(errs);
	    }
	}
    }

    api::ConfObject const &Config::getConf() const
    {
      return conf;
    }

    zia::api::ConfObject::iterator Config::find(std::string const &name)
    {
      return conf.find(name);
    }

    void Config::erase(zia::api::ConfObject::iterator it)
    {
      conf.erase(it);
    }

    zia::api::ConfObject::iterator Config::end()
    {
      return conf.end();
    }

    api::ConfObject &Config::getConf()
    {
      return conf;
    }


}
