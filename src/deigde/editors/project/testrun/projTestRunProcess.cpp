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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/select.h>
#endif

#include "projTestRunConstants.h"
#include "projTestRunProcess.h"
#include "projTestRunCommandThread.h"

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

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/logger/deLoggerConsoleColor.h>



// Definitions
////////////////

#define LOGSOURCE "Test-Run Process"



// Class projTestRunProcess
/////////////////////////////

projTestRunProcess::sRunParameters::sRunParameters() :
parameterCount(0),
parameters(NULL){
}

projTestRunProcess::sRunParameters::~sRunParameters(){
	if(parameters){
		delete [] parameters;
	}
}

// Constructors and Destructors
/////////////////////////////////

#ifdef OS_W32
projTestRunProcess::projTestRunProcess(HANDLE pipeIn, HANDLE pipeOut)
#else
projTestRunProcess::projTestRunProcess(int pipeIn, int pipeOut)
#endif
:
pPipeIn(pipeIn),
pPipeOut(pipeOut),

pLauncher(*this),
pEngine(*this),
pCommandThread(NULL){
}

projTestRunProcess::~projTestRunProcess(){
}



// Management
///////////////

void projTestRunProcess::WriteUCharToPipe(int value){
	if(value < 0 || value > 0xff){
		DETHROW(deeInvalidParam);
	}
	const uint8_t uchar = (uint8_t)value;
	WriteToPipe(&uchar, sizeof(uint8_t));
}

void projTestRunProcess::WriteUShortToPipe(int value){
	if(value < 0 || value > 0xffff){
		DETHROW(deeInvalidParam);
	}
	const uint16_t vushort = (uint16_t)value;
	WriteToPipe(&vushort, sizeof(uint16_t));
}

void projTestRunProcess::WriteFloatToPipe(float value){
	WriteToPipe(&value, sizeof(float));
}

void projTestRunProcess::WriteString16ToPipe(const char *string){
	const int length = (int)strlen(string);
	WriteUShortToPipe(length);
	WriteToPipe(string, length);
}

void projTestRunProcess::WriteToPipe(const void *data, int length){
	if(length == 0){
		return;
	}
	
	#ifdef OS_W32
	DWORD bytesWritten = 0;
	
	if(!WriteFile(pPipeOut, data, length, &bytesWritten, NULL)){
		DETHROW(deeInvalidAction);
	}
	if((int)bytesWritten < length){
		DETHROW(deeInvalidAction);
	}
	
	#else
	if(write(pPipeOut, data, length) < length){
		DETHROW(deeInvalidAction);
	}
	#endif
}

int projTestRunProcess::ReadUCharFromPipe(){
	uint8_t uchar;
	ReadFromPipe(&uchar, sizeof(uint8_t));
	return uchar;
}

int projTestRunProcess::ReadUShortFromPipe(){
	uint16_t vushort;
	ReadFromPipe(&vushort, sizeof(uint16_t));
	return vushort;
}

float projTestRunProcess::ReadFloatFromPipe(){
	float value;
	ReadFromPipe(&value, sizeof(float));
	return value;
}

decString projTestRunProcess::ReadString16FromPipe(){
	const int length = ReadUShortFromPipe();
	decString string;
	string.Set(' ', length);
	ReadFromPipe(string.GetMutableString(), length);
	return string;
}

void projTestRunProcess::ReadFromPipe(void *data, int length){
	if(length == 0){
		return;
	}
	
	#ifdef OS_W32
	DWORD bytesRead = 0;
	
	if(!ReadFile(pPipeIn, data, length, &bytesRead, NULL)){
		pLogger->LogErrorFormat(LOGSOURCE, "ReadFromPipe failed with error %ld:",
			GetLastError());
		DETHROW(deeInvalidAction);
	}
	if((int)bytesRead < length){
		pLogger->LogErrorFormat(LOGSOURCE, "ReadFromPipe read %ld bytes but should"
			" have read %d bytes:", bytesRead, length);
		DETHROW(deeInvalidAction);
	}
	
	#else
	if(read(pPipeIn, data, length) != length){
		DETHROW(deeInvalidAction);
	}
	#endif
}



void projTestRunProcess::Run(){
	try{
		pReadRunParameters();
		
		pLogConfiguration();
		pLauncher.LocatePath();
		pLauncher.CreateVFS();
		
		pEngine.Start();
		pEngine.PutIntoVFS();
		
		pRunGame();
		pStopEngine();
		
	}catch(const deException &e){
		if(pLogger){
			pLogger->LogException(LOGSOURCE, e);
			
		}else{
#ifdef OS_W32
			MessageBoxA(NULL, e.FormatOutput().Join("\n").GetString(),
				"Test-Runner Error", MB_OK | MB_ICONERROR);
#else
			e.PrintError();
#endif
		}
		pStopEngine();
		throw;
	}
	
	if(pLogger){
		pLogger->LogInfo(LOGSOURCE, "Test-Run Finished. Exiting process");
	}
}

void projTestRunProcess::RequestQuit(){
	if(pEngine.GetEngine()){
		pEngine.GetEngine()->Quit();
	}
}



// Private Functions
//////////////////////

void projTestRunProcess::pReadRunParameters(){
	int i, count;
	
	pRunParameters.pathLogFile = ReadString16FromPipe();
	pCreateLogger();
	pLogger->LogInfo(LOGSOURCE, "TestRunner launched. Reading run parameters...");

	pRunParameters.pathDataDirectory = ReadString16FromPipe();
	pRunParameters.pathOverlay = ReadString16FromPipe();
	pRunParameters.pathConfig = ReadString16FromPipe();
	pRunParameters.pathCapture = ReadString16FromPipe();
	
	pRunParameters.scriptDirectory = ReadString16FromPipe();
	pRunParameters.scriptVersion = ReadString16FromPipe();
	pRunParameters.gameObject = ReadString16FromPipe();
	pRunParameters.vfsPathConfig = ReadString16FromPipe();
	pRunParameters.vfsPathCapture = ReadString16FromPipe();
	
	pRunParameters.identifier = ReadString16FromPipe();
	pRunParameters.windowSizeX = ReadUShortFromPipe();
	pRunParameters.windowSizeY = ReadUShortFromPipe();
	pRunParameters.fullScreen = (ReadUCharFromPipe() != 0);
	pRunParameters.windowTitle = ReadString16FromPipe();
	
	pRunParameters.parameterCount = ReadUShortFromPipe();
	if(pRunParameters.parameterCount > 0){
		pRunParameters.parameters = new sModuleParameter[pRunParameters.parameterCount];
		for(i=0; i<pRunParameters.parameterCount; i++){
			pRunParameters.parameters[i].module = ReadString16FromPipe();
			pRunParameters.parameters[i].parameter = ReadString16FromPipe();
			pRunParameters.parameters[i].value = ReadString16FromPipe();
		}
	}
	
	pRunParameters.runArguments = ReadString16FromPipe();
	
	count = ReadUShortFromPipe();
	for(i=0; i<count; i++){
		pRunParameters.excludePatterns.Add(ReadString16FromPipe());
	}
	
	count = ReadUShortFromPipe();
	for(i=0; i<count; i++){
		pRunParameters.requiredExtensions.Add(ReadString16FromPipe());
	}
	
	pRunParameters.moduleScript = ReadString16FromPipe();
	pRunParameters.moduleScriptVersion = ReadString16FromPipe();
	
	pRunParameters.moduleGraphic = ReadString16FromPipe();
	pRunParameters.moduleInput = ReadString16FromPipe();
	pRunParameters.modulePhysics = ReadString16FromPipe();
	pRunParameters.moduleAnimator = ReadString16FromPipe();
	pRunParameters.moduleAI = ReadString16FromPipe();
	pRunParameters.moduleCrashRecovery = ReadString16FromPipe();
	pRunParameters.moduleAudio = ReadString16FromPipe();
	pRunParameters.moduleSynthesizer = ReadString16FromPipe();
	pRunParameters.moduleNetwork = ReadString16FromPipe();
	pRunParameters.moduleVR = ReadString16FromPipe();
	
// 	WriteUCharToPipe( projTestRunConstants::ercSuccess );

	pLogger->LogInfo(LOGSOURCE, "Run parameters read");
}

void projTestRunProcess::pLogConfiguration(){
	int i;
	
	pLogger->LogInfo(LOGSOURCE, "Run-Configuration:");
	
	pLogger->LogInfoFormat(LOGSOURCE, "- Path Log-File: %s",
		pRunParameters.pathLogFile.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Path Data Directory: %s",
		pRunParameters.pathDataDirectory.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Path Overlay Directory: %s",
		pRunParameters.pathOverlay.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Path Config Directory: %s",
		pRunParameters.pathConfig.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Path Capture Directory: %s",
		pRunParameters.pathCapture.GetString());
	
	pLogger->LogInfoFormat(LOGSOURCE, "- Script Directory: %s",
		pRunParameters.scriptDirectory.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Script Version: %s",
		pRunParameters.scriptVersion.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Game Object: %s",
		pRunParameters.gameObject.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- VFS Path Config: %s",
		pRunParameters.vfsPathConfig.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- VFS Path Capture: %s",
		pRunParameters.vfsPathCapture.GetString());
	
	pLogger->LogInfoFormat(LOGSOURCE, "- App-ID: %s",
		pRunParameters.identifier.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Window Size: %dx%d",
		pRunParameters.windowSizeX, pRunParameters.windowSizeY);
	pLogger->LogInfoFormat(LOGSOURCE, "- Full Screen: %s",
		pRunParameters.fullScreen ? "Yes" : "No");
	pLogger->LogInfoFormat(LOGSOURCE, "- Window Title: %s",
		pRunParameters.windowTitle.GetString());
	
	pLogger->LogInfo(LOGSOURCE, "- Module Parameters:");
	for(i=0; i<pRunParameters.parameterCount; i++){
		pLogger->LogInfoFormat(LOGSOURCE, "  - %s:%s = %s",
			pRunParameters.parameters[i].module.GetString(),
			pRunParameters.parameters[i].parameter.GetString(),
			pRunParameters.parameters[i].value.GetString());
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "- Run Arguments: %s",
		pRunParameters.runArguments.GetString());
	
	pLogger->LogInfo(LOGSOURCE, "- Exclude Patterns:");
	for(i=0; i<pRunParameters.excludePatterns.GetCount(); i++){
		pLogger->LogInfoFormat(LOGSOURCE, "  - %s",
			pRunParameters.excludePatterns.GetAt(i).GetString());
	}
	
	pLogger->LogInfo(LOGSOURCE, "- Required Extensions:");
	for(i=0; i<pRunParameters.requiredExtensions.GetCount(); i++){
		pLogger->LogInfoFormat(LOGSOURCE, "  - %s",
			pRunParameters.requiredExtensions.GetAt(i).GetString());
	}
	
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Script: %s (%s)",
		pRunParameters.moduleScript.GetString(),
		pRunParameters.moduleScriptVersion.GetString());
	
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Graphic: %s",
		pRunParameters.moduleGraphic.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Input: %s",
		pRunParameters.moduleInput.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Physics: %s",
		pRunParameters.modulePhysics.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Animator: %s",
		pRunParameters.moduleAnimator.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module AI: %s",
		pRunParameters.moduleAI.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Crash-Recovery: %s",
		pRunParameters.moduleCrashRecovery.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Audio: %s",
		pRunParameters.moduleAudio.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Synthesizer: %s",
		pRunParameters.moduleSynthesizer.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module Network: %s",
		pRunParameters.moduleNetwork.GetString());
	pLogger->LogInfoFormat(LOGSOURCE, "- Module VR: %s",
		pRunParameters.moduleVR.GetString());
}

void projTestRunProcess::pCreateLogger(){
	if(pRunParameters.pathLogFile.IsEmpty()){
		pLogger = deLoggerConsoleColor::Ref::New();
		return;
	}
	
	decPath diskPath(decPath::CreatePathNative(pRunParameters.pathLogFile));
	decPath filePath;
	filePath.AddComponent(diskPath.GetLastComponent());
	diskPath.RemoveLastComponent();
	
	deVFSContainer::Ref container;
	
	container = deVFSDiskDirectory::Ref::New(diskPath);
	
	pLogger = deLoggerFile::Ref::New(container->OpenFileForWriting(filePath));
}

void projTestRunProcess::pRunGame(){
	pEngine.ActivateModules();
	pEngine.SetDataDirectory();
	pEngine.SetRunArguments();
	pEngine.InitVFS();
	pEngine.CreateMainWindow();
	
	pCommandThread = new projTestRunCommandThread(*this);
	pCommandThread->Start();
	
	pEngine.Run();
}

void projTestRunProcess::pStopEngine(){
	if(pCommandThread){
		pCommandThread->Abort();
		delete pCommandThread;
		pCommandThread = NULL;
	}
	
	pEngine.Stop();
}
