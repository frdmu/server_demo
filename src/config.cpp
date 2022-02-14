#include "config.h"
#include <yaml-cpp/yaml.h>
#include <list>
#include <iostream>
namespace frdmu {
    ConfigMgr::ConfMap ConfigMgr::m_conf_map;

    ConfigVarBase::ptr ConfigMgr::lookup(const std::string &name) {
        auto it = m_conf_map.find(name);
        return it == m_conf_map.end() ? nullptr : it->second;
    }

    void listAllNodes(std::string prefix, const YAML::Node &root, std::list<std::pair<std::string, YAML::Node>> &all_nodes_list) {
        if (root.IsMap()) {
            for (auto it = root.begin(); it != root.end(); ++it) {
                std::string name = prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(); 
                all_nodes_list.push_back(make_pair(name, it->second));
                listAllNodes(name, it->second, all_nodes_list);
            }
        }                
    }

    void ConfigMgr::loadFromYaml(const std::string &configFilename) {
        YAML::Node root = YAML::LoadFile(configFilename);
        std::list<std::pair<std::string, YAML::Node>> all_nodes_list;
        listAllNodes("", root, all_nodes_list);
        //for (auto it = all_nodes_list.begin(); it != all_nodes_list.end(); ++it) {
            //std::cout << it->first << "++++" << it->second << std::endl;
        //} 
        // (user defined)configVar  <- name -> node(configFile)
        for (auto pair: all_nodes_list) {
            std::string name = pair.first;
            
            auto configVarPtr = ConfigMgr::lookup(name);
            if (configVarPtr) {
                if (pair.second.IsScalar()) {
                    configVarPtr->fromString(pair.second.Scalar());
                } else {
                    //to do 
                    std::stringstream ss;
                    std::cout << ss.str() << std::endl;
                    configVarPtr->fromString(ss.str());
                } 
            }
        }
    }
}
