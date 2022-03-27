/* 
 * Drag[en]gine Game Engine Distribution Launcher
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

#include <stdio.h>
#include "LauncherIni.h"
#include "File.h"
#include "Exception.h"

LauncherIni::LauncherIni(const String& filename){
	File stream(filename);
	
	const String line(stream.ReadLine());
	if(line != "[Launch]"){
		throw Exception("INI file does not begin with [Launch]");
	}
	
	while(!stream.Eof()){
		const String line(stream.ReadLine());
		if(line.Length() == 0){
			continue;
		}
		
		const int index = line.Find('=');
		if(index == -1){
			throw Exception("INI file has invalid format (key=value)");
		}
		if(line[0] == '['){
			throw Exception("INI file has multiple sections");
		}
		
		const String key(line.SubString(0, index));
		const String value(line.SubString(index + 1));
		
		if(key == "File"){
			pFile = value;
		}
	}
}

LauncherIni::~LauncherIni(){
}

const String &LauncherIni::Get(const String &key){
	if(key == "File"){
		return pFile;
	}
	throw Exception(String("Key not found: ") + key);
}
