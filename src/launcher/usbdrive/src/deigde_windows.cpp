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

#include "common.h"
#include <process.h>


#define PLATFORM_DIR "windows64"

int main(int argc, char **argv){
	try{
		// find the base directory
		std::string workDir(GetWorkingDirectory());
		
		if(argc > 1){
			const std::string path(argv[1]);
			if(path.substr(0, 2) == ".."){
				workDir = RealPath(PathJoin({workDir, path}));
				
			}else{
				workDir = path;
			}
		}
		
		if(*(workDir.cend() - 1) == '\\'){
			workDir.erase(workDir.end() - 1);
		}
		std::cout << "Working Directory = " << workDir << std::endl;
		
		// find launcher binary
		const std::string pathLauncherBin(PathJoin({workDir, PLATFORM_DIR, "deigde", "bin", "deigde.exe"}));
		
		AssertPathExists(pathLauncherBin);
		
		// build environment variables to run
		Environment env;
		
		env.Append("PATH", PathJoin({workDir, PLATFORM_DIR, "deigde", "bin"}));
		env.Append("PATH", PathJoin({workDir, PLATFORM_DIR, "dragengine"}));
		
		const std::string cacheDir(PathJoin({workDir, "user", "cache"}));
		const std::string captureDir(PathJoin({workDir, "user", "capture"}));
		const std::string userConfigDir(PathJoin({workDir, "user", "config", "windows", "deigde"}));
		const std::string projectDir(PathJoin({workDir, "user", "projects"}));
		const std::string logDir(PathJoin({workDir, "logs", "deigde"}));
		
		env.Set("DE_ENGINE_PATH", PathJoin({workDir, PLATFORM_DIR, "dragengine", "data"}));
		env.Set("DE_SHARE_PATH", PathJoin({workDir, PLATFORM_DIR, "dragengine", "share"}));
		env.Set("DE_CONFIG_PATH", PathJoin({workDir, PLATFORM_DIR, "dragengine", "config"}));
		env.Set("DE_CACHE_PATH", cacheDir);
		env.Set("DE_CAPTURE_PATH" , captureDir);
		
		env.Set("DEIGDE_SYS_CONFIG", PathJoin({workDir, PLATFORM_DIR, "deigde", "config"}));
		env.Set("DEIGDE_USER_CONFIG", userConfigDir);
		env.Set("DEIGDE_PROJECTS", projectDir);
		env.Set("DEIGDE_SHARES", PathJoin({workDir, PLATFORM_DIR, "deigde", "share"}));
		env.Set("DEIGDE_LIB", PathJoin({workDir, PLATFORM_DIR, "deigde", "data"}));
		env.Set("DEIGDE_LOGS", logDir);
		
		char ** const exexenv = env.BuildEnv();
		
		// make sure directories exist
		MakeDirs(cacheDir);
		MakeDirs(captureDir);
		MakeDirs(userConfigDir);
		MakeDirs(logDir);
		
		// run launcher
		if(_execle(pathLauncherBin.c_str(), "deigde.exe", (char*)nullptr, exexenv) == -1){
			std::cerr << "Failed running deigde.exe." << std::endl;
			throw std::runtime_error("Failed running deigde.exe.");
		}
		
	}catch(const std::exception &e){
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
