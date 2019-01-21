#include "Articles.hpp"
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <cassert>

EditLogEntry::EditLogEntry() {}

EditLogEntry::EditLogEntry(int year_, int month_, int day_, std::string desc_)
: year(year_), month(month_), day(day_), desc(desc_)
{}

EditLog::EditLog() {}

EditLog::EditLog(std::string articleDirPath)
{
    std::ifstream in(articleDirPath + "editlog.txt");
    
    if(!in.is_open()) {
        // There is no edit log. The std::vector of log entries will be left empty.
        return;
    }
    
    EditLogEntry e;
    e.year = 0; // year == 0 indicates that the time has not been set year
    
    std::string line;
    while (std::getline(in, line))
    {
        if(line.length() == 1 && line[0] == '`') {
            editHistory.push_back(e);
            e.year = 0;
        }
        else {
            if(e.year == 0) {
                e.setTime(line);
            }
            else {
                e.desc += line;
            }
        }
    }
    if(e.year) {
        editHistory.push_back(e);
    }
}

bool EditLogEntry::isValid()
{
    // Basic checks
    
    if(year == 0 || month == 0 || month > 12 || day == 0 || day > 31) {
        return false;        
    }
    
    if(month == 2) {
        if(day > 29) {
            return false;
        }
        
        if(year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) {
            // Not a leap year
            
            if(day > 28) {
                return false;
            }
        }
    }
    
    if((month % 2) == 1 && day == 31) {
        return false;
    }
    
    // Check if time is in the future
    
    // Get current time
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    
    if(year > (unsigned int)now->tm_year + 1900) {
        return false;
    }
    
    if(year == (unsigned int)now->tm_year + 1900) {
        if(month > (unsigned int)now->tm_mon + 1) {
            return false;
        }
        
        if(month == (unsigned int)now->tm_mon + 1) {
            if(day > (unsigned int)now->tm_mday) {
                return false;
            }
        }
    }
    
    return true;
    
}

#ifdef _DEBUG
void test__edit_log_is_valid()
{    
    EditLogEntry e(1942, 12, 31);    
    assert(e.isValid());
    
    e = EditLogEntry(1942, 12, 32);    
    assert(!e.isValid());
    
    e = EditLogEntry(1942, 13, 1);    
    assert(!e.isValid());
    
    // Future dates are not valid
    e = EditLogEntry(3095, 12, 1);    
    assert(!e.isValid());
    
    e = EditLogEntry(1942, 12, 0);    
    assert(!e.isValid());
    
    e = EditLogEntry(1942, 0, 2);    
    assert(!e.isValid());
    
    e = EditLogEntry(0, 1, 1);    
    assert(!e.isValid());
    
    // leap years
    
    e = EditLogEntry(1900, 2, 28);    
    assert(e.isValid());
    
    e = EditLogEntry(1900, 2, 29);    
    assert(!e.isValid());
    
    e = EditLogEntry(1908, 2, 29);    
    assert(e.isValid());
    
    e = EditLogEntry(2000, 2, 29);    
    assert(e.isValid());
}
#endif

void EditLogEntry::setTime(const std::string& time)
{
   std::string invalidMsg = "Invalid date format: " + time + ". Please enter dates as YYYY/MM/DD";
    
    auto sep1 = time.find("/", 0);
    
    if(sep1 == std::string::npos) {
        throw std::runtime_error(invalidMsg);
    }
    
    auto sep2 = time.find("/", sep1+1);
    
    if(sep2 == std::string::npos) {
        throw std::runtime_error(invalidMsg);
    }
    
    // Check that there are not more seperators
    auto extraSep = time.find("/", sep2+1);    
    if(extraSep != std::string::npos) {
        throw std::runtime_error(invalidMsg);
    }
    
    char * endPtr = (char *)(time.c_str() + sep1);
    year = strtol(time.c_str(), &endPtr, 10);
    
    endPtr = (char *)(time.c_str() + sep2);
    month = strtol(time.c_str() + sep1 + 1, &endPtr, 10);
    
    endPtr = (char *)(time.c_str() + time.size());
    day = strtol(time.c_str() + sep2 + 1, &endPtr, 10);
}

#ifdef _DEBUG
void test__edit_log_set_time()
{    
    EditLogEntry e;
    e.setTime("2033613/15477540/431");
    
    assert(e.year == 2033613);
    assert(e.month == 15477540);
    assert(e.day == 431);
    
    bool except = false;
    try {
        e.setTime("2033/613/15477540/431");
    }
    catch(std::exception& e) {
        (void) e;
        except = true;
    }
    assert(except);
}
#endif
    
