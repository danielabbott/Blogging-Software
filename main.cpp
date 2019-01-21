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

std::string load_file(const std::string & file)
{
    std::ifstream cssStream(file);
    if(cssStream.is_open()) {
        std::stringstream buffer;
        buffer << cssStream.rdbuf();
        return buffer.str();
    }
    else {
        throw std::runtime_error("File not found");
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
        
        create_main_page(std::string(argv[1]) + "/build/", articles, tagCollection);
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
