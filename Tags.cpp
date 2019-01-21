#include "Tags.hpp"
#include "Articles.hpp"
#include <fstream>
#include <algorithm>

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

