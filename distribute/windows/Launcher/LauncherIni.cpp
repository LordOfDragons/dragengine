/* 
 * Drag[en]gine Windows Launcher
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <sstream>

#include "LauncherIni.h"

LauncherIni::LauncherIni(const std::wstring& filename){
    auto stream = std::ifstream(filename);

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(stream, line)){
        lines.push_back(line);
    }

    std::vector<std::string>::const_iterator iter = lines.cbegin();
    if(iter == lines.cend()){
        throw std::runtime_error("INI file has no content");
    }

    if(*iter != "[Launch]"){
        throw std::runtime_error("INI file does not begin with [Launch]");
    }

    std::string::size_type index;
    for(iter++; iter!=lines.cend(); iter++){
        index = iter->find('=');
        if(index == std::string::npos){
            throw std::runtime_error("INI file has invalid format (key=value)");
        }
        if(iter->at(0) == '['){
            throw std::runtime_error("INI file has multiple sections");
        }

        pEntries[iter->substr(0, index)] = iter->substr(index + 1);
    }
}

LauncherIni::~LauncherIni(){
}

const std::string& LauncherIni::Get(const std::string& key){
    const MapEntries::const_iterator iter = pEntries.find(key);
    if(iter == pEntries.cend()){
        throw std::runtime_error(std::string("Key not found: ") + key);
    }
    return iter->second;
}
