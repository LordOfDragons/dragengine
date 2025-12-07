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

#ifndef _PROJTESTRUNPROCESS_H_
#define _PROJTESTRUNPROCESS_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#else
#include <unistd.h>
#endif

#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>

#include "projTestRunLauncher.h"
#include "projTestRunEngine.h"

class projTestRunCommandThread;



/**
 * \brief Game engine instance for test running the project.
 * 
 * Instance of game engine runs in a different process for safety reasons. Communication is
 * little used between the processes except quit signal. The process gets at the start the
 * project and profile information send.
 */
class projTestRunProcess{
public:
	/** \brief Module parameter. */
	struct sModuleParameter{
		decString module;
		decString parameter;
		decString value;
	};
	
	/** \brief Run parameters. */
	struct sRunParameters{
		decString pathLogFile;
		decString pathDataDirectory;
		decString pathOverlay;
		decString pathConfig;
		decString pathCapture;
		
		decString scriptDirectory;
		decString scriptVersion;
		decString gameObject;
		decString vfsPathConfig;
		decString vfsPathCapture;
		
		decString identifier;
		int windowSizeX;
		int windowSizeY;
		bool fullScreen;
		decString windowTitle;
		
		int parameterCount;
		sModuleParameter *parameters;
		
		decString runArguments;
		
		decStringSet excludePatterns;
		decStringSet requiredExtensions;
		
		decString moduleScript;
		decString moduleScriptVersion;
		
		decString moduleGraphic;
		decString moduleInput;
		decString modulePhysics;
		decString moduleAnimator;
		decString moduleAI;
		decString moduleCrashRecovery;
		decString moduleAudio;
		decString moduleSynthesizer;
		decString moduleNetwork;
		decString moduleVR;
		
		sRunParameters();
		~sRunParameters();
	};
	
	
	
private:
	#ifdef OS_W32
	HANDLE pPipeIn;
	HANDLE pPipeOut;
	
	#else
	int pPipeIn;
	int pPipeOut;
	#endif
	
	sRunParameters pRunParameters;
	
	deLogger::Ref pLogger;
	
	projTestRunLauncher pLauncher;
	projTestRunEngine pEngine;
	projTestRunCommandThread *pCommandThread;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create process. */
	#ifdef OS_W32
	projTestRunProcess(HANDLE pipeIn, HANDLE pipeOut);
	#else
	projTestRunProcess(int pipeIn, int pipeOut);
	#endif
	
	/** \brief Clean up process. */
	~projTestRunProcess();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief In pipe read end. */
	#ifdef OS_W32
	inline HANDLE GetPipeIn() const{return pPipeIn;}
	#else
	inline int GetPipeIn() const{return pPipeIn;}
	#endif
	
	/** \brief Out pipe write end. */
	#ifdef OS_W32
	inline HANDLE GetPipeOut() const{return pPipeOut;}
	#else
	inline int GetPipeOut() const{return pPipeOut;}
	#endif
	
	/** \brief Write unsigned char to pipe. */
	void WriteUCharToPipe(int value);
	
	/** \brief Write uint16_t to pipe. */
	void WriteUShortToPipe(int value);
	
	/** \brief Write float to pipe. */
	void WriteFloatToPipe(float value);
	
	/** \brief Write string to pipe using uint16_t as length. */
	void WriteString16ToPipe(const char *string);
	
	/** \brief Write data to pipe. */
	void WriteToPipe(const void *data, int length);
	
	/** \brief Read unsigned char from pipe. */
	int ReadUCharFromPipe();
	
	/** \brief Read uint16_t from pipe. */
	int ReadUShortFromPipe();
	
	/** \brief Read float from pipe. */
	float ReadFloatFromPipe();
	
	/** \brief Read string from pipe using uint16_t as length. */
	decString ReadString16FromPipe();
	
	/** \brief Read data from pipe. */
	void ReadFromPipe(void *data, int length);
	
	
	
	/** \brief Run parameters. */
	inline const sRunParameters &GetRunParameters() const{return pRunParameters;}
	
	/** \brief Logger. */
	inline deLogger *GetLogger() const{return pLogger;}
	
	/** \brief Launcher simulation. */
	inline projTestRunLauncher &GetLauncher(){return pLauncher;}
	
	/** \brief Engine. */
	inline projTestRunEngine &GetEngine(){return pEngine;}
	
	
	
	/** \brief Run. */
	void Run();
	
	/** \brief Request game to quit. */
	void RequestQuit();
	/*@}*/
	
	
	
private:
	void pReadRunParameters();
	void pLogConfiguration();
	void pRunGame();
	void pStopEngine();
	void pCreateLogger();
};

#endif
