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


#define PLATFORM_DIR "linux64"

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
		const std::string pathLauncherBin(PathJoin({workDir, PLATFORM_DIR, "bin", "delauncher-gui"}));
		
		AssertPathExists(pathLauncherBin);
		
		// build environment variables to run
		Environment env;
		
		//env.Append( "PATH", PathJoin( { workDir, PLATFORM_DIR, "bin" } ) );
		env.Append("PATH", "bin");
		env.Append("LD_LIBRARY_PATH", PathJoin({workDir, PLATFORM_DIR, "bin"}));
		env.Append("LD_LIBRARY_PATH", PathJoin({workDir, PLATFORM_DIR, "lib"}));
		
		const std::string cacheDir(PathJoin({workDir, "user", "cache"}));
		const std::string captureDir(PathJoin({workDir, "user", "capture"}));
		const std::string userConfigDir(PathJoin({workDir, "user", "config", "linux", "delauncher"}));
		const std::string sharedDir(PathJoin({workDir, "user", "delaunchshare"}));
		const std::string gameDir(PathJoin({workDir, "games"}));
		const std::string logDir(PathJoin({workDir, "logs", "launcher"}));
		
		env.Set("DE_ENGINE_PATH", PathJoin({workDir, PLATFORM_DIR, "lib", "dragengine"}));
		env.Set("DE_SHARE_PATH", PathJoin({workDir, PLATFORM_DIR, "share", "dragengine"}));
		env.Set("DE_CONFIG_PATH", PathJoin({workDir, PLATFORM_DIR, "etc", "dragengine"}));
		env.Set("DE_CACHE_PATH", cacheDir);
		env.Set("DE_CAPTURE_PATH" , captureDir);
		
		env.Set("DELAUNCHER_SYS_CONFIG", PathJoin({workDir, PLATFORM_DIR, "etc", "delauncher"}));
		env.Set("DELAUNCHER_USER_CONFIG", userConfigDir);
		env.Set("DELAUNCHER_SHARES", sharedDir);
		env.Set("DELAUNCHER_GAMES", gameDir);
		env.Set("DELAUNCHER_LOGS", logDir);
		
		char ** const exexenv = env.BuildEnv();
		
		// make sure directories exist
		MakeDirs(cacheDir);
		MakeDirs(captureDir);
		MakeDirs(userConfigDir);
		MakeDirs(logDir);
		
		// run launcher
		std::string ldrunner;
		
		if(PathExists("/lib64/ld-linux-x86-64.so.2")){
			ldrunner = "/lib64/ld-linux-x86-64.so.2";
			
		}else if(PathExists("/lib/ld-linux-x86-64.so.2")){
			ldrunner = "/lib/ld-linux-x86-64.so.2";
		}
		
		if(ldrunner.empty()){
			std::cerr << "Can not find 64-bit ld binary to run application without exec-bit set." << std::endl;
			throw std::runtime_error("Can not find 64-bit ld binary to run application without exec-bit set.");
		}
		
		const std::string ldrunnerFilename(PathFilename(ldrunner));
		if(execle(ldrunner.c_str(), ldrunnerFilename.c_str(), pathLauncherBin.c_str(), (char*)nullptr, exexenv) == -1){
			std::cerr << "Failed running delauncher-gui." << std::endl;
			throw std::runtime_error("Failed running delauncher-gui.");
		}
		
	}catch(const std::exception &e){
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
