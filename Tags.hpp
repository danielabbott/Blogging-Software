#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "BlogConfig.hpp"

typedef std::string TagString;

std::string url_safe_tag_name(const TagString & tag);

class Article;


class TagCollection
{
    typedef std::vector<std::shared_ptr<Article>> articlePtrList;
    std::map<TagString, articlePtrList> tagMap;
    
    std::vector<TagString> featuredTags;
    
    std::string sidebarsHTMLCache;
    
public:
    
    void addTag(const TagString & name);
    
    // Does _not_ add the article in the vectors
    // Calls addTag for each tag in the article
    void addAllTagsFromArticle(const Article & article);
    
    // Add all tags before adding articles.
    void addArticle(std::shared_ptr<Article> article);
    
    //const std::map<TagString, std::vector<std::shared_ptr<Article>>> & get() const { return tagMap; }
    
    void forEachTag(std::function<void(const TagString &)>) const;
    
    void forEachFeaturedTag(std::function<void(const TagString &)>) const;
    
    void loadFeaturedTags(const std::string & filePath);
    
    void createTagPages(const std::string& buildDir, const BlogConfig& config);
    
    const std::string & getSidebarsHTMLForTagPage();
    
    std::string getSidebarsHTMLForIndexPage();
};

