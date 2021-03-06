#include "Index.hpp"
#include <string>
#include "Articles.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include "BlogConfig.hpp"


void create_main_page(std::string buildDir,
                      const std::vector< Article >& articles, TagCollection & tagCollection, const BlogConfig & config)
{
    std::string index = "<!DOCTYPE html><html lang=\"en-GB\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<link href=\"theme.css\" rel=\"stylesheet\">"
    "</head><body><div id=\"Container\"><div id=\"TitleBar\"><h1>" + config.blogName + "</h1></div>";
    
    index += tagCollection.getSidebarsHTMLForIndexPage();
    
    
    index += "<div id=\"ArticleContent\">";

    for(auto const& article : articles) {
        index = index + "<div class=\"ArticlePreview\"><h2><a href=\"" + article.getFolderName() + ".html" + "\">" + article.title + "</a></h2>";
        
        if(article.meta.size()) {
            index += article.meta;
        }
        
        index += "</div><br/>";
    }
    
    index += "</div>" + 
    get_footer_html(config.blogName, config.copyrightHolder) +
    "</div>";
    
    index = index + get_common_script() + "</body></html>";

    std::ofstream out(buildDir + "index.html");
    out.write(index.c_str(), index.size());
}


