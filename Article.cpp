#include "Articles.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>
#include "String.hpp"
#include <cassert>
#include "BlogConfig.hpp"

Article::Article(std::string articlesRootDir, std::string articleFolderName_)
    : articleFolderName(articleFolderName_)
{
    const std::string path = articlesRootDir + articleFolderName + "/";

    editLog = EditLog(path);

    {
        std::ifstream titleStream(path + "/title.txt");
        if (!std::getline(titleStream, title)) {
            throw std::runtime_error("Invalid title.txt for article at: " + path);
        }
    }


    {
        std::ifstream metaStream(path + "/meta.txt");
        if (!std::getline(metaStream, meta)) {
            meta.clear();
        }
    }


    {
        std::ifstream rawContentStream(path + "/article.txt");
        std::stringstream buffer;
        buffer << rawContentStream.rdbuf();
        rawContent = buffer.str();
    }

    {
        std::ifstream authorsStream(path + "/authors.txt");
        std::string author;
        while (std::getline(authorsStream, author)) {
            authors.push_back(author);
        }
    }

    {
        std::ifstream tagsStream(path + "/tags.txt");
        std::string tag;
        while (std::getline(tagsStream, tag)) {
            tags.push_back(tag);
        }
    }

}

std::string& Article::build(const BlogConfig & config, std::string articlesRootDir)
{
    if(!rawContent.size()) {
        return rawContent;
    }

    exportedArticle = "<!DOCTYPE html><html lang=\"en-GB\"><head><title>" + title + "</title>" + "<meta charset=\"UTF-8\">"
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"../default.css\">"
    "<link href=\"https://fonts.googleapis.com/css?family=Roboto\" rel=\"stylesheet\">";


    if(meta.size()) {
        exportedArticle = exportedArticle + "<meta name=\"description\" content=\"" + meta + "\">";
    }

    if(authors.size()) {
        exportedArticle += "<meta name=\"author\" content=\"";
        for(auto auth = authors.begin(); auth < authors.end()-1; auth++) {
            exportedArticle = exportedArticle + *auth + ", ";
        }
        exportedArticle = exportedArticle + *(authors.end()-1) + "\"/>";
    }

    exportedArticle = exportedArticle + "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><div id=\"Container\">"
    "<div id=\"TitleBar\"><h1>Blog Title</h1></div>"
    "<div id=\"ArticleContent\">";


    bool parsingTag = false;
    std::string tag;
    std::string tagLowerCase;

    bool bold = false;
    int heading = 0;
    bool italics = false;
    bool strikethrough = false;
    bool raw = false; // html code inserted directly into output

    for(char c : rawContent) {
        if(c == '`') {
            if(parsingTag) {
                parsingTag = false;

                if(tag.size()) {
                    tagLowerCase = tag;
                    make_lowercase(tagLowerCase);

                    if (!raw) {
                        if(tagLowerCase == "bold") {
                            if(!bold) {
                                exportedArticle += "<b>";
                            }

                            bold = true;
                        }
                        else if(tagLowerCase == "/bold") {
                            if(bold) {
                                exportedArticle += "</b>";
                            }

                            bold = false;
                        }
                        else if(tagLowerCase == "heading") {
                            if(!heading) {
                                exportedArticle += "<h1>";
                            }

                            heading = 1;
                        }
                        else if(tagLowerCase == "/heading") {
                            if(heading == 1) {
                                exportedArticle += "</h1>";
                            }
                            else if(heading == 2) {
                                exportedArticle += "</h2>";
                            }

                            heading = 0;
                        }
                        else if(tagLowerCase == "subheading") {
                            if(!heading) {
                                exportedArticle += "<h2>";
                            }

                            heading = 2;
                        }
                        else if(tagLowerCase == "/subheading") {
                            if(heading == 1) {
                                exportedArticle += "</h1>";
                            }
                            else if(heading == 2) {
                                exportedArticle += "</h2>";
                            }

                            heading = 0;
                        }
                        else if(tagLowerCase == "italics") {
                            if(!italics) {
                                exportedArticle += "<i>";
                            }

                            italics = true;
                        }
                        else if(tagLowerCase == "/italics") {
                            if(italics) {
                                exportedArticle += "</i>";
                            }

                            italics = false;
                        }
                        else if(tagLowerCase == "strikethrough") {
                            if(!strikethrough) {
                                exportedArticle += "<del>";
                            }

                            strikethrough = true;
                        }
                        else if(tagLowerCase == "/strikethrough") {
                            if(strikethrough) {
                                exportedArticle += "</del>";
                            }

                            strikethrough = false;
                        }
                        else if(tagLowerCase.substr(0, 4) == "img ") {
                            // TODO: Parse image file headers and set width and height values

                            exportedArticle = exportedArticle + "<img src=\"../res/" + tag.substr(4) + "\">";
                        }
                        else if(tagLowerCase.substr(0, 5) == "html ") {
                            std::string htmlPath = tag.substr(5);

                            std::ifstream stream(articlesRootDir + "articles_src/" + articleFolderName + "/" + htmlPath);

                            if(!stream.is_open()) {

                                throw std::runtime_error("File not found: " + htmlPath);
                            }

                            std::stringstream buffer;
                            buffer << stream.rdbuf();
                            exportedArticle += buffer.str();
                        }
                        else if(tagLowerCase == "html") {
                            raw = true;
                        }
                    }
                    else if(tagLowerCase == "/html") {
                        raw = false;
                    }
                }
                else {
                    exportedArticle += '`';
                }

                tag.clear();
            }
            else {
                parsingTag = true;
            }
        }
        else {
            if(parsingTag) {
                tag += c;
            }
            else {
                if(c == '\n') {
                    exportedArticle += "<br/>";
                }
                else {
                    exportedArticle += c;
                }
            }
        }
    }

    if(config.disqusURL.size()) {

        exportedArticle = exportedArticle +
                          "<div id=\"disqus_thread\"></div>"
                          "<script>"
                          "(function() {"
                          "var d = document, s = d.createElement('script');"
                          "s.src = '" + config.disqusURL + "';"
                          "s.setAttribute('data-timestamp', +new Date());"
                          "(d.head || d.body).appendChild(s);"
                          "})();"
                          "</script>";

    }

    exportedArticle = exportedArticle + "</div><footer><p>[blog name] Copyright © bleh</p></footer></div></body></html>";


    return rawContent;
}

void Article::saveExport(const std::string& articlesExportRootDir)
{
    std::ofstream out(articlesExportRootDir + articleFolderName + "/index.html");
    out.write(exportedArticle.c_str(), exportedArticle.size());
}



std::vector<Article> getArticles(std::string rootDirPath)
{
    // TODO: scan articles_src instead of relying on .txt file

    std::ifstream in(rootDirPath + "articles.txt");
    if(!in.is_open()) {
        throw std::runtime_error("Could not load articles.txt");
    }

    std::vector<Article> articles;
    std::string name;
    while (std::getline(in, name)) {
        articles.emplace_back(Article(rootDirPath + "articles_src/", name));
    }

    return articles;
}



#ifdef _DEBUG
void test__load_test_article()
{

    Article a("testblog/articles_src/", "a2");
    assert(a.title == "article 2");
    assert(a.meta == "this is the second one");
    assert(a.authors.size() == 2);
    assert(a.authors[0] == "Person1");
    assert(a.authors[1] == "Person3");
}
#endif

