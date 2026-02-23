/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>

#include "deglbLauncher.h"
#include "deglbSignalHandler.h"

#include <dragengine/common/exceptions.h>



// Signal Handlers
////////////////////

static deglbSignalHandler *pSignalHandler = nullptr;

static void signalSegV(int number, siginfo_t *infos, void *ptrContext){
	const char *logSource = "?";
	deLogger *logger = nullptr;
	
	if(pSignalHandler){
		logger = pSignalHandler->GetLauncher().GetLogger();
		logSource = pSignalHandler->GetLauncher().GetLogSource();
	}
	
	if(infos->si_code == SEGV_MAPERR){
		if(logger){
			logger->LogErrorFormat(logSource,
				"Segmentation Fault! Tried to access not allocated memory at %p.",
				infos->si_addr);
		}else{
			printf("Segmentation Fault! Tried to access not allocated memory at %p.\n",
				infos->si_addr);
		}
	}else if(infos->si_code == SEGV_ACCERR){
		if(logger){
			logger->LogErrorFormat(logSource,
				"Segmentation Fault! Permission denied accessing memory at %p.",
				infos->si_addr);
		}else{
			printf("Segmentation Fault! Permission denied accessing memory at %p.\n",
				infos->si_addr);
		}
	}else{
		if(logger){
			logger->LogErrorFormat(logSource,
				"Segmentation Fault! Unknown memory error at %p.", infos->si_addr);
		}else{
			printf("Segmentation Fault! Unknown memory error at %p.\n", infos->si_addr);
		}
	}
	
	void *btentries[50];
	size_t btentryCount = backtrace(btentries, 50);
	
	if(logger){
		logger->LogError(logSource, "Backtrace:");
		char ** const btStrings = backtrace_symbols(btentries, btentryCount);
		for(size_t i=0; i<btentryCount; i++){
			logger->LogError(logSource, btStrings[i]);
		}
		free(btStrings);
	}else{
		printf("Backtrace:\n");
		backtrace_symbols_fd(btentries, btentryCount, fileno(stdout));
	}
	
	if(logger){
		logger->LogError(logSource, "Done, exiting.");
	}else{
		printf("Done, exiting.\n");
	}
	
	exit(-1);
}

static void signalAbort(int number, siginfo_t *infos, void *ptrContext){
	const char *logSource = "?";
	deLogger *logger = nullptr;
	
	if(pSignalHandler){
		logger = pSignalHandler->GetLauncher().GetLogger();
		logSource = pSignalHandler->GetLauncher().GetLogSource();
	}
	
	if(logger){
		logger->LogError(logSource, "Unhandled Exception!");
	}else{
		printf("Unhandled Exception\n");
	}
	
	void *btentries[50];
	size_t btentryCount = backtrace(btentries, 50);
	
	if(logger){
		logger->LogError(logSource, "Backtrace:");
		char **btStrings = backtrace_symbols(btentries, btentryCount);
		for(size_t i=0; i<btentryCount; i++){
			logger->LogError(logSource, btStrings[i]);
		}
		free(btStrings);
	}else{
		printf("Backtrace:\n");
		backtrace_symbols_fd(btentries, btentryCount, fileno(stdout));
	}
	
	if(logger){
		logger->LogError(logSource, "Done, exiting.");
	}else{
		printf("Done, exiting.\n");
	}
	
	exit(-1);
}



// Class deglbSignalHandler
////////////////////////////

// Constructor, destructor
////////////////////////////

deglbSignalHandler::deglbSignalHandler(deglbLauncher &launcher) :
pLauncher(launcher){
	pRegisterSignals();
}

deglbSignalHandler::~deglbSignalHandler(){
	pSignalHandler = nullptr;
}



// Private Functions
//////////////////////

void deglbSignalHandler::pRegisterSignals(){
	struct sigaction action;
	
	pSignalHandler = this;
	
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = signalSegV;
	action.sa_flags = SA_SIGINFO;
	if(sigaction(SIGSEGV, &action, nullptr)){
		DETHROW(deeInvalidAction);
	}
	
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = signalAbort;
	action.sa_flags = SA_SIGINFO;
	if(sigaction(SIGABRT, &action, nullptr)){
		DETHROW(deeInvalidAction);
	}
	
	memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_IGN;
	action.sa_flags = 0;
	if(sigaction(SIGPIPE, &action, nullptr)){
		DETHROW(deeInvalidAction);
	}
}
