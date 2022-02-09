#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <map>
#include <iostream>
namespace frdmu {
    /// abstract base class
    class ConfigVarBase {
        public:
            typedef std::shared_ptr<ConfigVarBase> ptr;
            ConfigVarBase() = default;
            ConfigVarBase(const std::string &name, const std::string &description = ""): m_name(name), m_description(description) {}
            virtual std::string toString() = 0;
            virtual void fromString(std::string &val) = 0;
        protected:
            std::string m_name;
            std::string m_description;
    };

    /// configVar class
    template <class T>
    class ConfigVar : public ConfigVarBase {
        public:
            typedef std::shared_ptr<ConfigVar> ptr;
            ConfigVar() = default;
            ConfigVar(const std::string &name, const T &val, const std::string &description = " "): ConfigVarBase(name, description), m_val(val) {} 
            std::string toString() override{
                return boost::lexical_cast<std::string>(m_val);
            } 
            void fromString(std::string &val) override {
                m_val = boost::lexical_cast<T>(val);
            }
            void print() {
                std::cout << "m_name:" << m_name << " m_val:" << m_val << " m_description:" << m_description << std::endl; 
            }
        private:
            T m_val;
    };
    /// user interface
    class ConfigMgr {
        public:
            typedef std::shared_ptr<ConfigMgr> ptr;
            typedef std::map<std::string, ConfigVarBase::ptr> ConfMap;
            ConfigMgr() = default;
            
            template <typename T> 
            static typename ConfigVar<T>::ptr lookup(const std::string &name) {
                auto it = m_conf_map.find(name);
                if (it != m_conf_map.end())
                    return std::dynamic_pointer_cast<ConfigVar<T>>(it->second); // Base pointer -> Derived pointer
                return nullptr;
            }
            
            template <typename T> 
            static typename ConfigVar<T>::ptr lookup(const std::string &name, const T &val, const std::string &description) {
                auto res = lookup<T>(name); 
                if (res) {
                    return res; 
                }

                if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz.123456789") != std::string::npos) {
                    return nullptr;
                }
                auto tmp_ptr =  typename ConfigVar<T>::ptr(new ConfigVar<T>(name, val, description));
                m_conf_map[name] = tmp_ptr;

                return tmp_ptr;
            }
        private:
            static ConfMap m_conf_map;
    };
}
#endif
