#pragma once

#include <iostream>
//#include <jsoncpp/json/json.h>
#include <json/json.h>
#include "api/Conf.hpp"

namespace zia
{
  class Config
  {
  private:
    std::string const filename;
    api::Conf conf;

    api::ConfObject getObjectFromJson(Json::Value const &) const;
    api::ConfArray getArrayFromJson(Json::Value const &) const;

    template <class Func>
    void addToContainer(Json::Value const &value, Func &&func) const
    {
      switch (value.type())
	{
	case Json::ValueType::booleanValue:
	  func(value.asBool());
	  break;
	case Json::ValueType::intValue:
	  func(static_cast<long long>(value.asInt()));
	  break;
	case Json::ValueType::uintValue:
	  func(static_cast<long long>(value.asInt()));
	  break;
	case Json::ValueType::realValue:
	  func(value.asDouble());
	  break;
	case Json::ValueType::stringValue:
	  func(value.asString());
	  break;
	case Json::ValueType::objectValue:
	  func(getObjectFromJson(value));
	  break;
	case Json::ValueType::arrayValue:
	  func(getArrayFromJson(value));
	  break;
	default:
	  throw std::runtime_error("unknown type in config file");
	}
    }

  public:
    Config(std::string const &filename);

    Config(api::Conf const &conf) : conf(conf)
    {
    }

    void load();

    template <class T>
    T &get(std::string const &name)
    {
      return std::get<T>(conf.at(name).v);
    }

    template <class T>
    T const &get(std::string const &name) const
    {
      return std::get<T>(conf.at(name).v);
    }

    template <class T>
    void set(std::string const &name, T &&value)
    {
      using It = api::ConfObject::iterator;
      std::pair<It, bool> result = conf.emplace(std::make_pair(name, std::forward<T>(value)));
      if (!result.second)
	{
	  conf.at(name) = api::ConfValue(std::forward<T>(value));
	}
    }

    template <class T>
    bool setIfNotSet(std::string const &key, T &&value)
    {
      auto valueIt = std::get_if<std::string>(&conf.at(key).v);
      if (!valueIt)
	{
	  set(key, std::forward<T>(value));
	  return true;
	}
      return false;
    }

    template <class T>
    T getOrDefault(std::string const &key, T defaultValue) const
    {
      using It = api::ConfObject::const_iterator;
      It result = conf.find(key);
      return result == conf.end() ? defaultValue : std::get<T>(result->second.v);
    }

    zia::api::ConfObject::iterator find(std::string const &name);
    void erase(zia::api::ConfObject::iterator it);
    zia::api::ConfObject::iterator end();
    api::ConfObject const &getConf() const;
    api::ConfObject &getConf();
  };
}
