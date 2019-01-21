#pragma once

#include "Articles.hpp"

std::string make_rss(const std::vector<Article> & articles, const BlogConfig & config);
