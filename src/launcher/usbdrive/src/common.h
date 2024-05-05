/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
