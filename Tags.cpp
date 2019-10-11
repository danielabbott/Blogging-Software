#include "Tags.hpp"
#include "Articles.hpp"
#include <fstream>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

std::string url_safe_tag_name(const TagString & tag)
{
    std::string s;
    for(const char c : tag) {
        if(c == ' ' || c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@'
             || c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+'
                    || c == ',' || c == ';' || c == '='
        ) {
            std::stringstream stream;
            stream << std::hex << (int)c;
            s = s + '%' + stream.str();
        }
        else if (c > 0){
            s += c;
        }
        else {
            std::stringstream stream;
            stream << std::hex << (int)c;
            s = s + 'u' + stream.str();
        }
    }
    return s;
}

void test__tag_to_url()
{
    assert(url_safe_tag_name("bob's tag") == "bob%27s%20tag");
}

void TagCollection::addTag(const TagString & name)
{
    tagMap[name] = std::vector<std::shared_ptr<Article>>();
}

void TagCollection::addAllTagsFromArticle(const Article & article)
{
    for(const auto & tag : article.tags) {
        addTag(tag);
    }
}

void TagCollection::addArticle(std::shared_ptr<Article> article)
{
    for(const auto & tagName : article->tags) {
        tagMap[tagName].push_back(article);        
    }
}


void TagCollection::forEachTag(std::function<void(const TagString&)> callback) const
{
    for(const auto & tagPair : tagMap) {    
        callback(tagPair.first);
    }
}

void TagCollection::forEachFeaturedTag(std::function<void(const TagString&)> callback) const
{
    for(const auto & tag : featuredTags) {    
        callback(tag);
    }
}

void TagCollection::loadFeaturedTags(const std::string & filePath)
{
    std::ifstream s(filePath);
    std::string featuredTag;
    while (std::getline(s, featuredTag)) {
        // Featured tags are ignored if no article has that tag
        
        bool tagExists = std::any_of(tagMap.begin(), tagMap.end(), [&featuredTag](std::pair<TagString, articlePtrList> pair)
        { return pair.first == featuredTag; });
        
        if(tagExists) {
            featuredTags.push_back(featuredTag);
        }
    }
}

void TagCollection::createTagPages(const std::string& buildDir, const BlogConfig & config)
{
    for(const auto & tagPair : tagMap) { 
        std::string tagPage = "<!DOCTYPE html><html lang=\"en-GB\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<link href=\"../theme.css\" rel=\"stylesheet\">"
        "<link href=\"https://fonts.googleapis.com/css?family=Roboto\" rel=\"stylesheet\">"
        "</head><body><div id=\"Container\"><div id=\"TitleBar\"><a href=\"../index.html\"><h1>" + config.blogName + "</h1></a></div>";
        
        
        tagPage += getSidebarsHTMLForTagPage();
        
        
        tagPage = tagPage + "<div id=\"ArticleContent\"><div class=\"ArticlePreview\"><h2>Tag: " + tagPair.first + "</h2></div><br/>";

        for(auto const& article : tagPair.second) {
            tagPage = tagPage + "<div class=\"ArticlePreview\"><h2><a href=\"../" + article->getFolderName() + ".html" + "\">" + article->title + "</a></h2>";
            
            if(article->meta.size()) {
                tagPage += article->meta;
            }
            
            tagPage += "</div><br/>";
        }
        
        tagPage += "</div>" + 
        get_footer_html(config.blogName, config.copyrightHolder) +
        "</div>";
        
        tagPage = tagPage + get_common_script() + "</body></html>";
        
        std::ofstream f(buildDir + "tags/" + tagPair.first + ".html");
        f.write(tagPage.c_str(), tagPage.size());
    }
}

const std::string & TagCollection::getSidebarsHTMLForTagPage()
{
    if(sidebarsHTMLCache.size()) {
        return sidebarsHTMLCache;
    }
    sidebarsHTMLCache = "<div id=\"leftbar\">Featured Tags<br/>";
    
    auto addTag = [this](const TagString & tag){
        sidebarsHTMLCache = sidebarsHTMLCache + "<br/><a href=\"" + url_safe_tag_name(tag) + ".html\">" + tag + "</a>";        
    };
    
    forEachFeaturedTag(addTag);
    
    sidebarsHTMLCache += "</div><div id=\"rightbar\">All Tags<br/>";    
    
    forEachTag(addTag);
    
    sidebarsHTMLCache += "</div>";
    
    return sidebarsHTMLCache;
}

std::string TagCollection::getSidebarsHTMLForIndexPage()
{
    std::string html = "<div id=\"leftbar\">Featured Tags<br/>";
    
    auto addTag = [&html](const TagString & tag){
        html = html + "<br/><a href=\"tags/" + url_safe_tag_name(tag) + ".html\">" + tag + "</a>";        
    };
    
    forEachFeaturedTag(addTag);
    
    html += "</div><div id=\"rightbar\">All Tags<br/>";    
    
    forEachTag(addTag);
    
    html += "</div>";
    
    return html;
}


