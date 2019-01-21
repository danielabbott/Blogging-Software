#include "RSS.hpp"
#include "BlogConfig.hpp"
#include <time.h>

std::string make_rss(const std::vector<Article> & articles, const BlogConfig & config)
{
    std::string rss =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
        "<rss version=\"2.0\">"
        "<channel>"
        "<title>";

    rss = rss + config.blogName + "</title>"
          "<lastBuildDate>";

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    rss = rss + asctime (timeinfo) + "</lastBuildDate>";

    for(const auto & article : articles) {
        rss = rss + "<item><title>" + article.getTitle() + "</title><description>" + article.getMeta() + "</description><link>http://" 
            + config.blogURL + article.getFolderName() + "/index.html</link></item>";
    }

    rss += "</channel></rss>";

    return rss;
}
