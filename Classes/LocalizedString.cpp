//
//  LocalizedString.cpp
//  JumpingNingjya2
//
//  Created by yoshidayuki on 2014/06/09.
//
//

#include "LocalizedString.h"
#include <map>
#include <list>
#include <string>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

string trim(const string& str, const char* trimCharacterList = " \t\v\r\n\"\'")
{
    string result;
    std::string::size_type left = str.find_first_not_of(trimCharacterList);
    
    if (left != std::string::npos)
    {
        std::string::size_type right = str.find_last_not_of(trimCharacterList);
        result = str.substr(left, right - left + 1);
    }
    
    return result;
}

list<string> split(string str, string delim)
{
    list<string> result;
    int cutAt;
    

    while( (cutAt = str.find_first_of(delim)) != str.npos )
    {
        if(cutAt > 0)
        {
            result.push_back(trim(str.substr(0, cutAt)));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        result.push_back(trim(str));
    }
    
    return result;
}

const char* LocalizedString(const char* searchKey, const char* comment){
    const char* ret = comment;
    
    static map<string, string> localizable;
    if(localizable.empty()){
        FileUtils* fileUtil = FileUtils::getInstance();
        LanguageType language = Application::getInstance()->getCurrentLanguage();
        if(language == LanguageType::JAPANESE){
            fileUtil->addSearchPath("ja.lproj");
        }
        else{
            fileUtil->addSearchPath("en.lproj");
        }
        
        string fullpath = fileUtil->fullPathForFilename("Localizable.strings");
        string strings = fileUtil->getStringFromFile(fullpath);
        
        if(strings == ""){
            return ret;
        }
        
        list<string> stringLines = split(strings, ";");
        
        for(list<string>::iterator begin = stringLines.begin(), end = stringLines.end(); begin != end; ++begin){
            list<string> stringKeyValue = split(*begin, "=");
            if(stringKeyValue.size() >= 2){
                list<string>::iterator keyv = stringKeyValue.begin();
                localizable.insert(pair<string, string>(*keyv, *(++keyv)));
            }
        }
    }
    
    map<string, string>::iterator iterator = localizable.find(searchKey);
    if(iterator != localizable.end()){
        ret = iterator->second.c_str();
    }
    
    return ret;
}