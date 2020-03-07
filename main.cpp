#include <iostream>
#include <string>
#include "BlogConfig.hpp"
#include "String.hpp"
#include "Articles.hpp"
#include <cassert>
#include "Index.hpp"
#include <fstream>
#include <sstream>
#include "Tags.hpp"
#include "RSS.hpp"
#include <filesystem>

std::string load_file(const std::string & file)
{
    std::ifstream stream(file);
    if(stream.is_open()) {
        std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }
    else {
        throw std::runtime_error("File not found: " + file);
    }
}

void save_file(const std::string & file, const std::string & text)
{
    std::ofstream stream(file);
    if(stream.is_open()) {
        stream.write(text.c_str(), text.size());
    }
    else {
		throw std::runtime_error("Error saving file: " + file);
    }
}

void copy_css_file(const std::string & buildDir)
{
    std::string destinationPath = buildDir + "theme.css";
    
    bool fileAlreadyExits = false;
    
    {
        std::ifstream s(destinationPath);
        fileAlreadyExits = s.is_open(); 
    }
    
    if(!fileAlreadyExits) {
        auto css = load_file("default.css");
        save_file(destinationPath, css);
    }
}

#ifdef _DEBUG
int main__(int argc, char **argv) {
#else
int main(int argc, char **argv) {
#endif
    if(argc != 2) {
        std::cout << "Usage: blogsoft [path to root directory of blog]" << std::endl;
        return 1;
    }
    
    try {
        std::string rootDir = std::string(argv[1]);
        if(rootDir[rootDir.size()-1] != '/' && rootDir[rootDir.size()-1] != '\\') {
            rootDir += '/';
        }

        std::filesystem::create_directories(std::string(rootDir) + "build/tags");
        
        BlogConfig config(rootDir);
        
        std::vector<Article> articles = getArticles(rootDir);
        TagCollection tagCollection;
        
        for(auto& a : articles) {
            tagCollection.addAllTagsFromArticle(a);
            a.build(config, rootDir);
            a.saveExport(std::string(rootDir) + "build/");
        }
        
        for(auto& a : articles) {
            tagCollection.addArticle(std::make_shared<Article>(a));            
        }
        
        tagCollection.loadFeaturedTags(std::string(rootDir) + "featuredtags.txt");
        
        tagCollection.createTagPages(std::string(rootDir) + "build/", config);
        
        create_main_page(std::string(rootDir) + "build/", articles, tagCollection, config);
        
        save_file(std::string(rootDir) + "build/blog.rss", make_rss(articles, config));
        
        copy_css_file(std::string(rootDir) + "build/");
    }
    catch(std::exception& e) {
        std::cout << "The software has encountered a fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

#ifdef _DEBUG

#include "tests.h"
#include "Tags.hpp"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    // Run test cases
    
    test__load_test_article();
    test__edit_log_is_valid();
    test__edit_log_set_time();
    test__tag_to_url();
    
    
    // Run main function
       
    
    std::string s("testblog/");
    char* ptrs[] = {(char *)"", (char *)s.c_str()};
    assert(main__(2, ptrs) == 0);
    
    std::cout << "All tests passed" << std::endl;
}

#endif
