/*
 * Copyright 2019 AT&T Intellectual Property
 * Copyright 2019 Nokia
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by adi ENZEL on 11/19/19.
//

#ifndef E2_READCONFIGFILE_H
#define E2_READCONFIGFILE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <boost/algorithm/string.hpp>

using namespace std;

class ReadConfigFile {
public:

    explicit ReadConfigFile() = default;

    int openConfigFile(std::string const& configFile) {
        std::ifstream file(configFile.c_str());
        if (!file) {  // file not found
            //perror(configFile.c_str());
            //mdclog_write(MDCLOG_ERR, "File: %s, failed to open", configFile.c_str());
            return -1;
        }
        std::string line;

        while (std::getline(file,line)) {
            if (!line.length()) {  //line empty
                continue;
            } else if (line[0] == '#') {
                continue;
            } else if (line[0] == ';') {
                continue;
            }

            auto leftHand = line.find('=');
            auto name = line.substr(0,leftHand);
            trim(name);
            auto value = line.substr(leftHand+1);
            trim(value);
            //cout << "entry = " << name << " value = " << value  << endl;
            entries[name] = value;
        }
        return 0;
    }

    /**
     * @param key the key we are looking
     * @return string value of the entry and "" if not exists
     */
    string getStringValue(std::string const& key) const {
        auto entry = entries.find(key);
        if (entry == entries.end()) {
            return "";
        }
        return entry->second;
    }

    /**
     * @param key the key we are looking
     * @return int value of the entry and -1 if not exists
     */
    int getIntValue(std::string const& key) const {
        auto entry = entries.find(key);
        if (entry == entries.end()) {
             return -1;
        }
        char *dummy;
        int ret = (int)std::strtol(entry->second.c_str(), &dummy, 10);
        //cout << "entry = " << key << " value = " << entry->second  << " int value = " << ret << endl;
        return ret;
    }

    /**
    * @param key the key we are looking
    * @return double value of the entry and -1.0 if not exists
    */
    double getDoubleValue(std::string const& key) const {
        auto entry = entries.find(key);
        if (entry == entries.end()) {
            return -1.0;
        }
        char *dummy;
        return std::strtod(entry->second.c_str(), &dummy);
    }

private:
    std::unordered_map<string, string> entries;


    inline static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }

    inline static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }

    inline static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
        return ltrim(rtrim(str, chars), chars);
    }
};


#endif //E2_READCONFIGFILE_H