#pragma once

#include <vector>
#include <string>

struct BlogConfig;

struct EditLogEntry
{
    unsigned int year = 0; // 1 .. 2019 ....
    unsigned int month = 0; // 1 = jan, .., 12 = dec
    unsigned int day = 0;// 1 .. 31
    
    std::string desc;
    
    EditLogEntry();
    EditLogEntry(int year_, int month_, int day_, std::string desc_ = "");
    
    void setTime(std::string const& time);
    
    bool isValid();
};

class EditLog
{
    std::vector<EditLogEntry> editHistory;
    
public:
    EditLog();
    EditLog(std::string articleDirPath);
};

typedef std::string Author;

class Article 
{
    std::string articleFolderName;
    
    std::string exportedArticle;
public:
    
    std::string rawContent;
    std::string title;
    
    // This gets put in the html meta tag
    // If the string is empty then the meta tag is not added
    std::string meta;
    
    std::vector<Author> authors;
    EditLog editLog;    
    
    std::vector<std::string> tags;
    
    
    Article(std::string articlesRootDir, std::string articleFolderName_);
    
    std::string const& getFolderName() const { return articleFolderName; }
    
    
    // article.txt is parsed, resources are loaded/inspected
    // The result is stored in 'exportedArticle' 
    std::string & build(const BlogConfig & config, std::string articlesRootDir);
    
    std::string const& getBuiltArticle() const { return exportedArticle; }
    
    void saveExport(std::string const& articlesExportRootDir);
};

std::vector<Article> getArticles(std::string rootDirPath);


