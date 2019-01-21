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

std::string load_file(const std::string & file)
{
    std::ifstream stream(file);
    if(stream.is_open()) {
        std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }
    else {
        throw std::runtime_error("File not found");
    }
}

void save_file(const std::string & file, const std::string & text)
{
    std::ofstream stream(file);
    if(stream.is_open()) {
        stream.write(text.c_str(), text.size());
    }
    else {
        throw std::runtime_error("File not found");
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
    }
    
    try {
        BlogConfig config(argv[1]);
        
        std::vector<Article> articles = getArticles(argv[1]);
        TagCollection tagCollection;
        
        for(auto& a : articles) {
            tagCollection.addAllTagsFromArticle(a);
            a.build(config, argv[1]);
            a.saveExport(std::string(argv[1]) + "/build/");
        }
        
        for(auto& a : articles) {
            tagCollection.addArticle(std::make_shared<Article>(a));            
        }
        
        tagCollection.loadFeaturedTags(std::string(argv[1]) + "/featuredtags.txt");
        
        create_main_page(std::string(argv[1]) + "/build/", articles, tagCollection, config);
        
        save_file(std::string(argv[1]) + "/build/blog.rss", make_rss(articles, config));
        
        copy_css_file(std::string(argv[1]) + "/build/");
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
    
    
    // Run main function
       
    
    std::string s("testblog/");
    char* ptrs[] = {(char *)"", (char *)s.c_str()};
    assert(main__(2, ptrs) == 0);
    
    std::cout << "All tests passed" << std::endl;
}

#endif
