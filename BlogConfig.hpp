#pragma once

#include <string>

// Stores configuration options for a blog
// These are stored on the file system in an ASCII text file
struct BlogConfig {
    std::string blogName;
    
    // Root URL. Files in the build directory are placed under this URL.
    std::string blogURL;
    
    // e.g. https://danielabbott-test.disqus.com/embed.js
    // If empty then disqus is diabled (Default setting)
    std::string disqusURL;
    

    // Reads the config.txt file and sets variables to match
    // Root directory path should end with a path seperator ('/' or '\')
    BlogConfig(std::string rootDirectory_);
    
//     void saveConfigFile();
};
