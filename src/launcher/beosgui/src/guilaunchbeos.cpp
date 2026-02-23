/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <signal.h>

#include <Application.h>

#include "guilaunchb.h"
#include "gui/deglbWindowMain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>


// Application class
//////////////////////

class deglbApplication : public BApplication{
private:
	int pArgc;
	char **pArgv;
	
public:
	deglbApplication(int argc, char **argv) :
	BApplication("application/x-vnd.dragengine-launcher-gui.app"),
	pArgc(argc),
	pArgv(argv){
	}
	
	void ReadyToRun() override{
		try{
			deglbWindowMain * const window = new deglbWindowMain(pArgc, pArgv);
			if(window->RunCommandLineActions()){
				window->Show();
				
			}else{
				Quit();
			}
		}catch(const deException &e){
			e.PrintError();
			Quit();
		}
	}
};


// Entry point
////////////////

int main(int argc, char **argv){
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &act, nullptr);
	
	try{
		deglbApplication(argc, argv).Run();
		return 0;
		
	}catch(const deException &e){
		e.PrintError();
		return -1;
	}
}
