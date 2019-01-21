#include "Index.hpp"
#include <string>
#include "Articles.hpp"
#include <vector>
#include <fstream>
#include <iostream>

void create_main_page(std::__cxx11::string buildDir,
                      const std::vector< Article >& articles, const TagCollection & tagCollection)
{
    std::string index = "<!DOCTYPE html><html lang=\"en-GB\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"default.css\">"
    "<link href=\"https://fonts.googleapis.com/css?family=Roboto\" rel=\"stylesheet\">";

    index += "</head><body><div id=\"Container\"><div id=\"TitleBar\"><h1>[Blog Title]</h1></div>"
    "<div id=\"leftbar\">Featured Tags<br/>";
    
    auto addTag = [&index](const TagString & tag){ index = index + "<br/>" + tag; };
    
    tagCollection.forEachFeaturedTag(addTag);
    
    index += "</div><div id=\"rightbar\">All Tags<br/>";    
    
    tagCollection.forEachTag(addTag);
    
    
    index += "</div><div id=\"ArticleContent\">";

    for(auto const& article : articles) {
        index = index + "<div class=\"ArticlePreview\"><h2><a href=\"" + article.getFolderName() + "/index.html" + "\">" + article.title + "</a></h2>";
        
        if(article.meta.size()) {
            index += article.meta;
        }
        
        index += "</div><br/>";
    }

    
    index += "</div><footer><p>[blog name] Copyright © bleh</p></footer></div>";
    
    index += "</body></html>";

    std::ofstream out(buildDir + "index.html");
    out.write(index.c_str(), index.size());
}


