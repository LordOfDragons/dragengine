/* 
 * Drag[en]gine USB Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <algorithm>
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef OS_UNIX
#include <unistd.h>
#elif defined OS_W32
#include <direct.h>
#include <windows.h>
#endif

#ifdef OS_UNIX
#define PATH_SEPARATOR std::string( "/" )
#define ENV_SEPARATOR std::string( ":" )
#elif defined OS_W32
#define PATH_SEPARATOR std::string( "\\" )
#define ENV_SEPARATOR std::string( ";" )
#endif


extern std::string GetWorkingDirectory();
extern std::string PathJoin( const std::list<std::string> &list );
extern std::string RealPath( const std::string &filename );
extern std::string ParentDirectory( const std::string &path );
extern std::string PathFilename( const std::string &path );

extern bool PathExists( const std::string &path );

extern void AssertPathExists( const std::string &path );

class Environment{
public:
	Environment();
	~Environment();
	
	void Set( const std::string &key, const std::string &value );
	void Append( const std::string &key, const std::string &value );
	char **BuildEnv();
	
private:
	std::map<std::string,std::string> entries;
	std::list<std::string> tempenv;
	char **buildenv;
};

extern void SetEnvValue( const std::string &key, const std::string &value );
extern void AppendEnvValue( const std::string &key, const std::string &value );
extern void AppendEnvValue( const std::string &key, const std::string &value1, const std::string &value2 );

extern void MakeDirs( const std::string &path );

#endif
