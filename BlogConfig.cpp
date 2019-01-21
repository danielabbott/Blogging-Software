#include "BlogConfig.hpp"
#include "String.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

BlogConfig::BlogConfig(std::string rootDirectory)
{
    std::ifstream in(rootDirectory + "config.txt");

    if(!in.is_open()) {
        throw runtime_error("Config file not found");
    }

    std::string line;
    std::string lowercase;
    while (std::getline(in, line))
    {
        lowercase = line;
        make_lowercase(lowercase);

        if(lowercase.substr(0, 12) == "disqus-url: ") {
            disqusURL = line.substr(12);
        }
        else if(lowercase.substr(0, 6) == "name: ") {
            blogName = line.substr(6);
        }
        else {
            clog << "Unrecognised setting in configuration file: " << line << "\n";
        }
    }
}
