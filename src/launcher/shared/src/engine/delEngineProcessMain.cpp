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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "delEngineProcess.h"
#include "delEngineProcessMain.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_UNIX
#	include <dragengine/app/deOSUnix.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_W32
#	include <dragengine/app/deOSWindows.h>
#else
#	error OS not supported!
#endif


delEngineProcessMain::delEngineProcessMain(){
}

#ifdef OS_W32
int delEngineProcessMain::RunMain(int argc, char **args){
	(void)SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	
	HANDLE pipeIn = INVALID_HANDLE_VALUE;
	HANDLE pipeOut = INVALID_HANDLE_VALUE;
	uint16_t logfileLength;
	decString logfile;
	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;
	char sync = 1;
	
//	FILE *w32ConH4xFile = freopen( "OOPS.log", "w", stdout );
//	if( ! w32ConH4xFile ) return -1;
//	setvbuf( w32ConH4xFile, NULL, _IONBF, 0 );
	
//	printf( "GO!\n" );
//	fflush( stdout );
	pipeIn = GetStdHandle(STD_INPUT_HANDLE);
//	printf( "using in pipe %p\n", pipeIn );
//	fflush( stdout );
	
	if(pipeIn == INVALID_HANDLE_VALUE){
		return -1;
	}
	
	try{
//		printf( "reading write pipe\n" );
//		fflush( stdout );
		// read out-pipe handle
		if(!ReadFile(pipeIn, &pipeOut, sizeof(pipeOut), &bytesRead, NULL)){
			DETHROW_INFO(deeInvalidParam, "failed reading out-pipe handle");
		}
		if(bytesRead < sizeof(pipeOut)){
			DETHROW_INFO(deeInvalidParam, "reading out-pipe handle too short");
		}
		
		// read log filename
//		printf( "reading log file length\n" );
//		fflush( stdout );
		if(!ReadFile(pipeIn, &logfileLength, sizeof(logfileLength), &bytesRead, NULL)){
			DETHROW_INFO(deeInvalidParam, "failed reading log filename");
		}
		if(bytesRead < sizeof(logfileLength)){
			DETHROW_INFO(deeInvalidParam, "reading log filename too short");
		}
		
//		printf( "reading log file name\n");
//		fflush( stdout );
		logfile.Set(' ', logfileLength);
		if(!ReadFile(pipeIn, (LPVOID)logfile.GetString(), (int)logfileLength, &bytesRead, NULL)){
			DETHROW_INFO(deeInvalidParam, "failed reading log filename");
		}
		if(bytesRead < (DWORD)logfileLength){
			DETHROW_INFO(deeInvalidParam, "reading log filename too short");
		}
		
		uint8_t flags = 0;
		if(!ReadFile(pipeIn, &flags, 1, &bytesRead, NULL)){
			DETHROW_INFO(deeInvalidParam, "failed reading flags");
		}
		if(bytesRead < 1){
			DETHROW_INFO(deeInvalidParam, "reading flags too short");
		}
		const bool useConsole = (flags & 0x1) == 0x1;
		
		// send sync
//		printf( "sending sync\n" );
//		fflush( stdout );
		if(!WriteFile(pipeOut, &sync, 1, &bytesWritten, NULL)){
			DETHROW_INFO(deeInvalidAction, "failed sending sync");
		}
		if(bytesWritten < 1){
			DETHROW_INFO(deeInvalidAction, "sending sync too short");
		}
		
		// start process
//		printf( "start process\n");
//		fflush( stdout );
		delEngineProcess process(pipeIn, pipeOut, logfile);
		process.SetUseConsole(useConsole);
		
		process.Run();
		
	}catch(const deException &){
//		e.PrintError();
//		printf( "GetLastError %x\n", ( unsigned int )GetLastError() );
		if(pipeIn != INVALID_HANDLE_VALUE){
			CloseHandle(pipeIn);
		}
		if(pipeOut != INVALID_HANDLE_VALUE){
			CloseHandle(pipeOut);
		}
		return -1;
	}
	
	CloseHandle(pipeIn);
	CloseHandle(pipeOut);
	return 0;
}
#endif
