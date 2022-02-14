#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>

void printYaml(const YAML::Node &node) {
    if (node.IsScalar()) {
        std::cout << node.Scalar() << std::endl;
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            std::cout <<  it->first.Scalar() << ":"; 
            printYaml(it->second);
        }
    } else if (node.IsSequence()) {
        size_t n = node.size();
        for (auto i = 0; i < n; ++i) {
            printYaml(node[i]);
        }
    } else {
        std::cout << " " << std::endl;
    }
}

int main(int argc,char** argv)
{
    YAML::Node node = YAML::LoadFile("./config.yaml");
    printYaml(node); 
    return 0;
}
