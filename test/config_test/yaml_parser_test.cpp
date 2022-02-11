#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace std;

int main(int argc,char** argv)
{
    YAML::Node config = YAML::LoadFile("./config.yaml");
    /*
    cout << "name:" << config["name"].as<string>() << endl;
    cout << "sex:" << config["sex"].as<string>() << endl;
    cout << "age:" << config["age"].as<int>() << endl;
    */
    cout << "skills:" << config["skills"] << endl;
    //cout << config << endl;
    return 0;
}
