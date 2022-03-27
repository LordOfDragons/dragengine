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

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "Launcher.h"
#include "LauncherIni.h"
#include "Exception.h"
#include "String.h"

Launcher::Launcher(int argc, char **argv) :
pLauncherIni(nullptr){
}

Launcher::~Launcher(){
	if(pLauncherIni){
		delete pLauncherIni;
	}
}

int Launcher::Run(){
	char cwd[PATH_MAX];
	if(!getcwd(cwd, sizeof(cwd))){
		throw Exception("Failed get working directory");
	}
	pLauncherDirectory = cwd;
	
	if(pLauncherDirectory[-1] != '/'){
		pLauncherDirectory += '/';
	}
	
	pLauncherIni = new LauncherIni(pLauncherDirectory + "Launcher.ini");
	
	pLaunchDelga();
	return 0;
}

void Launcher::pLaunchDelga(){
	// check if system knows how to launch delga files. we are using this extra
	// check instead of just calling xdg-open and checking the return value because
	// xdg-open can block the process with a user-question because it detects the
	// delga file to launch to be a binary (albeit not an executable one). to avoid
	// potential stalling problems the query solution is used. less elegant but
	// the result will certainly never stall the process
	FILE *cmdcheck = popen("xdg-mime query default application/dragengine-delga", "r");
	int count = 0;
	char buffer[8];
	while(fgets(buffer, sizeof(buffer), cmdcheck)){
		count++;
	}
	pclose(cmdcheck);
	
	// install the game engine if absent
	if(count == 0){
		// try gnome-terminal (ubuntu)
		String cmdline("gnome-terminal -x bash -c \"");
		cmdline += pLauncherDirectory + "install_dragengine.sh";
		cmdline += '"';
		
		int result = system(cmdline.Pointer());
		if(result){
			// try xterm
			cmdline = "xterm -e \"";
			cmdline += pLauncherDirectory + "install_dragengine.sh";
			cmdline += '"';
			
			result = system(cmdline.Pointer());
		}
		
		if(result){
			throw Exception("No terminal found to run installer");
		}
		
		//if(system(pLauncherDirectory + "install_dragengine.sh") != 0){
		/*
		String cmdline("gnome-terminal -x sh -c \"");
		cmdline += pLauncherDirectory + "install_dragengine.sh";
		cmdline += '"';
		*/
// 		if(system(cmdline.Pointer()) != 0){
			// user abort installation or installation failed. an error
			// has been already shown so gracefully exit the launcher
// 			return;
// 		}
	}
	
	// check again if system knows how to launch delga files. this should
	// return success otherwise installer failed or user aborted it
	cmdcheck = popen("xdg-mime query default application/dragengine-delga", "r");
	count = 0;
	while(fgets(buffer, sizeof(buffer), cmdcheck)){
		count++;
	}
	pclose(cmdcheck);
	
	if(count == 0){
// 		throw Exception("Drag[en]gine installation not working (launch broken)");
		return;
	}
	
	// launch delga file. this should work now
	String cmdline("xdg-open \"");
	cmdline += pLauncherDirectory + pLauncherIni->Get("File");
	cmdline += '"';
	
	if(system(cmdline) != 0){
		throw Exception("Failed launching.");
	}
}

// Entry point
int main( int argc, char **argv ){
	try{
		Launcher launcher(argc, argv);
		return launcher.Run();
		
	}catch(const Exception &e){
		String str("xmessage -center \"Failed running launcher:\n");
		str += e.Message();
		str += '"';
		system(str);
		return 1;
	}
}
