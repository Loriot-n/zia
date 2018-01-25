#pragma once

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "conf.hpp"

namespace zia
{
    namespace bpt = boost::property_tree;
    class Config
    {
    private:
      std::string const filename;

      api::Conf conf;

      template <class T>
      inline void add(std::string const &name, bpt::ptree &pt)
      {
	conf.emplace(std::make_pair(name, pt.get<T>(name)));
      }

    public:
      Config(std::string const &);
      ~Config();

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
    };

    template <>
    inline void Config::add<api::ConfArray>(std::string const &name, bpt::ptree &pt)
    {
      api::ConfArray arr;
      for (auto const &elem : pt.get_child(name))
	arr.push_back(api::ConfValue(elem.second.get<long long>("")));
      conf.emplace(std::make_pair(name, arr));
    }

}
