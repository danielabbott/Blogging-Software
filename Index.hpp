#include <string>
#include "Articles.hpp"
#include <vector>
#include "Tags.hpp"

void create_main_page(std::__cxx11::string buildDir, const std::vector< Article >& articles, TagCollection& tagCollection, const BlogConfig& config);
