#include "Index.hpp"
#include <string>
#include "Articles.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include "BlogConfig.hpp"

void create_main_page(std::__cxx11::string buildDir,
                      const std::vector< Article >& articles, const TagCollection & tagCollection, const BlogConfig & config)
{
    std::string index = "<!DOCTYPE html><html lang=\"en-GB\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<link rel=\"preload\" href=\"theme.css\" as=\"style\" onload=\"this.rel='stylesheet'\">"
    "<noscript><link rel=\"stylesheet\" href=\"theme.css\"></noscript>"
    "<link href=\"https://fonts.googleapis.com/css?family=Roboto\" rel=\"stylesheet\">"
    "</head><body><div id=\"Container\"><div id=\"TitleBar\"><h1>" + config.blogName + "</h1></div>"
    "<div id=\"leftbar\">Featured Tags<br/>";
    
    auto addTag = [&index](const TagString & tag){
        index = index + "<br/><a href=\"tags/" + url_safe_tag_name(tag) + ".html\">" + tag + "</a>";        
    };
    
    tagCollection.forEachFeaturedTag(addTag);
    
    index += "</div><div id=\"rightbar\">All Tags<br/>";    
    
    tagCollection.forEachTag(addTag);
    
    
    index += "</div><div id=\"ArticleContent\">";

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


