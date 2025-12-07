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

#include <stdlib.h>

#include "common.h"

#ifdef OS_W32
#include <locale>
#include <codecvt>
#include <string>
#endif

std::string GetWorkingDirectory(){
	char buffer[FILENAME_MAX];
	#ifdef OS_UNIX
	if(! getcwd(buffer, FILENAME_MAX)){
	#elif defined OS_W32
	if(! _getcwd(buffer, FILENAME_MAX)){
	#endif
		std::cerr << "GetWorkingDirectory: getcwd failed." << std::endl;
		throw std::runtime_error("GetWorkingDirectory: getcwd failed");
	}
	return std::string(buffer);
}


std::string PathJoin(const std::list<std::string> &list){
	std::list<std::string>::const_iterator iter(list.cbegin());
	std::stringstream joined;
	joined << *iter;
	while(++iter != list.cend()){
		joined << PATH_SEPARATOR << *iter;
	}
	return joined.str();
}

std::string RealPath(const std::string &filename){
	#ifdef OS_UNIX
	char buffer[FILENAME_MAX];
	if(! realpath(filename.c_str(), buffer)){
		std::cerr << "RealPath: realpath failed." << std::endl;
		throw std::runtime_error("RealPath: realpath failed");
	}
	return std::string(buffer);
	
	#elif defined OS_W32
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wideFilename = converter.from_bytes(filename);
	wchar_t wbuffer[FILENAME_MAX];
	GetFullPathName(wideFilename.c_str(), FILENAME_MAX, (LPWSTR)&wbuffer, nullptr);
	return converter.to_bytes(wbuffer);
	#endif
}

std::string ParentDirectory(const std::string &path){
	std::string::const_iterator iter = std::find_end(path.cbegin(), path.cend(), PATH_SEPARATOR.cbegin(), PATH_SEPARATOR.cend());
	
	if(iter != path.cend()){
		std::string result;
		std::copy(path.cbegin(), iter, std::back_inserter(result));
		return result;
		
	}else{
		return path;
	}
}

std::string PathFilename(const std::string &path){
	std::string::const_iterator iter = std::find_end(path.cbegin(), path.cend(), PATH_SEPARATOR.cbegin(), PATH_SEPARATOR.cend());
	
	if(iter != path.cend()){
		std::string result;
		std::copy(iter, path.cend(), std::back_inserter(result));
		return result;
		
	}else{
		return path;
	}
}


bool PathExists(const std::string &path){
	struct stat filestat;
	return stat(path.c_str(), &filestat) == 0;
}

void AssertPathExists(const std::string &path){
	struct stat filestat;
	const int result = stat(path.c_str(), &filestat);
	if(result || ! S_ISREG(filestat.st_mode)){
		std::cerr << "File not found: '" << path << "'" << std::endl;
		std::stringstream s;
		s << "File not found: '" << path << "'" << std::endl;
		throw std::runtime_error(s.str());
	}
}


// extern char **environ;
Environment::Environment() : buildenv(nullptr){
	char **iterenv = environ;
	while(*iterenv){
		const std::string keyvalue(*iterenv);
		const std::string::const_iterator deli(std::find(keyvalue.cbegin(), keyvalue.cend(), '='));
		if(deli != keyvalue.cend()){
			std::string key, value;
			std::copy(keyvalue.cbegin(), deli, std::back_inserter(key));
			std::copy(deli + 1, keyvalue.cend(), std::back_inserter(value));
			entries[key] = value;
		}
		iterenv++;
	}
}

Environment::~Environment(){
	if(buildenv){
		delete [] buildenv;
	}
}

void Environment::Set(const std::string &key, const std::string &value){
	std::cout << "Environment.Set: " << key << " = " << value << std::endl;
	entries[key] = value;
}

void Environment::Append(const std::string &key, const std::string &value){
	std::cout << "Environment.Append: " << key << " = " << value << std::endl;
	
	const std::map<std::string,std::string>::iterator iter = entries.find(key);
	if(iter != entries.cend()){
		std::stringstream combined;
		combined << value << ENV_SEPARATOR << iter->second;
		entries[key] = combined.str();
		
	}else{
		entries[key] = value;
	}
}

char **Environment::BuildEnv(){
	if(buildenv){
		delete [] buildenv;
		buildenv = nullptr;
	}
	
	tempenv.clear();
	std::map<std::string,std::string>::const_iterator iter;
	for(iter = entries.cbegin(); iter != entries.cend(); iter++){
		std::stringstream s;
		s << iter->first << "=" << iter->second;
		tempenv.push_back(s.str());
	}
	
	buildenv = new char*[tempenv.size() + 1];
	char ** out = buildenv;
	std::list<std::string>::const_iterator iter2;
	for(iter2 = tempenv.cbegin(); iter2 != tempenv.cend(); iter2++){
		*( out++ ) = ( char* )iter2->c_str();
	}
	*out = nullptr;
	
	return buildenv;
}



void SetEnvValue(const std::string &key, const std::string &value){
	std::stringstream result;
	result << key << "=" << value;
	
	const std::string sresult(result.str());
	if(putenv((char*)sresult.c_str())){
		std::cerr << "SetEnvValue: putenv failed";
		throw std::runtime_error("SetEnvValue: putenv failed");
	}
	
	std::cout << "SetEnvValue: " << result.str() << std::endl;
}

void AppendEnvValue(const std::string &key, const std::string &value){
	std::stringstream result;
	result << key << "=" << value;
	
	const char * const prevValue = getenv(key.c_str());
	if(prevValue){
		result << ENV_SEPARATOR << prevValue;
	}
	
	const std::string sresult(result.str());
	if(putenv((char*)sresult.c_str())){
		std::cerr << "AppendEnvValue: putenv failed";
		throw std::runtime_error("AppendEnvValue: putenv failed");
	}
	
	std::cout << "AppendEnvValue: " << result.str() << std::endl;
}

void AppendEnvValue(const std::string &key, const std::string &value1, const std::string &value2){
	std::stringstream combinedValue;
	combinedValue << value1 << ENV_SEPARATOR << value2;
	AppendEnvValue(key, combinedValue.str());
}


void MakeDirs(const std::string &path){
	std::string::const_iterator iter = path.cbegin();
	struct stat dirstat;
	
	while(iter != path.cend()){
		std::string::const_iterator next = std::find(iter, path.cend(), PATH_SEPARATOR[0]);
		std::string dirname;
		std::copy(path.cbegin(), iter, std::back_inserter(dirname));
		if(stat(dirname.c_str(), &dirstat)){
			return;
		}
		
		#ifdef OS_UNIX
		if(! mkdir(dirname.c_str(), 0777)){
		#elif defined OS_W32
		if(! mkdir(dirname.c_str())){
		#endif
			std::cerr << "MakeDirs: mkdir failed for '" << dirname << "'" << std::endl;
			throw std::runtime_error("MakeDirs: mkdir failed");
		}
		
		if(iter == path.cend()){
			break;
		}
		
		iter = next + 1;
	}
}
