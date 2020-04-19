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

#include "common.h"
#include <process.h>


#define PLATFORM_DIR "windows64"

int main( int argc, char **argv ){
	try{
		// find the base directory
		std::string workDir( GetWorkingDirectory() );
		
		if( argc > 1 ){
			workDir = argv[ 1 ];
		}
		
		if( *( workDir.cend() - 1 ) == '\\' ){
			workDir.erase( workDir.end() - 1 );
		}
		std::cout << "Working Directory = " << workDir << std::endl;
		
		// find launcher binary
		const std::string pathLauncherBin( PathJoin( { workDir, PLATFORM_DIR, "delaunchers", "bin", "delauncher-gui.exe" } ) );
		
		AssertPathExists( pathLauncherBin );
		
		// build environment variables to run
		Environment env;
		
		env.Append( "PATH", PathJoin( { workDir, PLATFORM_DIR, "delaunchers", "bin" } ) );
		env.Append( "PATH", PathJoin( { workDir, PLATFORM_DIR, "dragengine" } ) );
		
		const std::string cacheDir( PathJoin( { workDir, "user", "cache" } ) );
		const std::string captureDir( PathJoin( { workDir, "user", "capture" } ) );
		const std::string userConfigDir( PathJoin( { workDir, "user", "config", "windows", "delauncher" } ) );
		const std::string sharedDir( PathJoin( { workDir, "user", "delaunchshare" } ) );
		const std::string gameDir( PathJoin( { workDir, "games" } ) );
		const std::string logDir( PathJoin( { workDir, "logs", "launcher" } ) );
		
		env.Set( "DE_ENGINE_PATH", PathJoin( { workDir, PLATFORM_DIR, "dragengine", "data" } ) );
		env.Set( "DE_SHARE_PATH", PathJoin( { workDir, PLATFORM_DIR, "dragengine", "share" } ) );
		env.Set( "DE_CONFIG_PATH", PathJoin( { workDir, PLATFORM_DIR, "dragengine", "config" } ) );
		env.Set( "DE_CACHE_PATH", cacheDir );
		env.Set( "DE_CAPTURE_PATH" , captureDir );
		
		env.Set( "DELAUNCHER_SYS_CONFIG", PathJoin( { workDir, PLATFORM_DIR, "delaunchers", "config" } ) );
		env.Set( "DELAUNCHER_USER_CONFIG", userConfigDir );
		env.Set( "DELAUNCHER_SHARES", sharedDir );
		env.Set( "DELAUNCHER_GAMES", gameDir );
		env.Set( "DELAUNCHER_LOGS", logDir );
		
		char ** const exexenv = env.BuildEnv();
		
		// make sure directories exist
		MakeDirs( cacheDir );
		MakeDirs( captureDir );
		MakeDirs( userConfigDir );
		MakeDirs( logDir );
		
		// run launcher
		if( _execle( pathLauncherBin.c_str(), "delauncher-gui.exe", ( char* )nullptr, exexenv ) == -1 ){
			std::cerr << "Failed running delauncher-gui.exe." << std::endl;
			throw std::runtime_error( "Failed running delauncher-gui.exe." );
		}
		
	}catch( const std::exception &e ){
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
