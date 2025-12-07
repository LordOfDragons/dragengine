#include <stdlib.h>
#include <stdio.h>

#include "projTestRunProcess.h"

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#include <dragengine/common/exceptions.h>
#endif




#ifdef OS_W32
// entry point for windows32 helper binary
#ifdef OS_W32_VS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	(void)SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	const HANDLE pipeIn = GetStdHandle(STD_INPUT_HANDLE);
	if(pipeIn == INVALID_HANDLE_VALUE){
		return -1;
	}
	
	HANDLE pipeOut = INVALID_HANDLE_VALUE;
	DWORD bytesRead = 0;
	
	try{
		// read out-pipe handle
		if(!ReadFile(pipeIn, &pipeOut, sizeof(pipeOut), &bytesRead, NULL)){
			DETHROW(deeInvalidParam);
		}
		if(bytesRead < sizeof(pipeOut)){
			DETHROW(deeInvalidParam);
		}
		
		// start process
		projTestRunProcess process(pipeIn, pipeOut);
		process.Run();
		
	}catch(const deException &){
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

#else
int main(int argc, char **argv){
	int pipeIn, pipeOut;
	
	/*
	printf("ENTERING1\n");
	if(read(STDIN_FILENO, &pipeIn, sizeof(pipeIn)) != sizeof(pipeIn)){
		return 1;
	}
	printf("ENTERING2\n");
	if(read(STDIN_FILENO, &pipeOut, sizeof(pipeOut)) != sizeof(pipeOut)){
		return 1;
	}
	*/
	
	if(argc != 3){
		return 1;
	}
	
	pipeIn = (int)strtol(argv[1], NULL, 10);
	pipeOut = (int)strtol(argv[2], NULL, 10);
	
	projTestRunProcess process(pipeIn, pipeOut);
	process.Run();
	return 0;
}

#endif
