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

#ifndef _DELENGINEPROCESS_H_
#define _DELENGINEPROCESS_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

class deEngine;
class delEngineProcessRunGame;


/**
 * \brief Game engine process.
 * 
 * Process handling the game engine instance. This class is used to keep the engine instance
 * properties outside the engine class. This way the engine class only features variables
 * and methods usable from the main process. This class contains anything usable only from
 * inside the separate process.
 */
class DE_DLL_EXPORT delEngineProcess{
public:
	/** \brief Command codes. */
	enum eCommandCodes{
		/** \brief Stop the process. */
		eccStopProcess,
		
		/** \brief Get engine property value. */
		eccGetProperty,
		
		/** \brief Load modules. */
		eccLoadModules,
		
		/** \brief Get module status. */
		eccGetModuleStatus,
		
		/** \brief Get module parameter list. */
		eccGetModuleParamList,
		
		/** \brief Set module parameter. */
		eccSetModuleParameter,
		
		/** \brief Activate module. */
		eccActivateModule,
		
		/** \brief Enable or disable a module. */
		eccEnableModule,
		
		/** \brief Set data directory. */
		eccSetDataDir,
		
		/** \brief Set cache application identifier. */
		eccSetCacheAppID,
		
		/** \brief Add disk directory to virtual file system. */
		eccVFSAddDiskDir,
		
		/** \brief Add virtual file system container for module shared data. */
		eccVFSAddScriptSharedDataDir,
		
		/** \brief Add delga file to virtual file system as root container. */
		eccVFSAddDelgaFile,
		
		/** \brief Set command line arguments. */
		eccSetCmdLineArgs,
		
		/** \brief Create render window. */
		eccCreateRenderWindow,
		
		/** \brief Start game. */
		eccStartGame,
		
		/** \brief Stop game. */
		eccStopGame,
		
		/** \brief Current display resolutions. */
		eccGetDisplayCurrentResolution,
		
		/** \brief Display resolutions. */
		eccGetDisplayResolutions,
		
		/** \brief Command reading game definitions from DELGA file. */
		eccReadDelgaGameDefs,
		
		/** \brief Command reading game patch definitions from DELGA file. */
		eccReadDelgaPatchDefs,
		
		/** \brief Command reading files from delga. */
		eccReadDelgaFiles,
		
		/**
		 * \brief Set overlay directory.
		 * \version 1.7
		 */
		eccSetPathOverlay,
		
		/**
		 * \brief Set capture directory.
		 * \version 1.7
		 */
		eccSetPathCapture,
		
		/**
		 * \brief Set config directory.
		 * \version 1.7
		 */
		eccSetPathConfig
	};
	
	/** \brief Result Codes. */
	enum eResultCodes{
		/** \brief Command successful. */
		ercSuccess,
		
		/** \brief Command failed. */
		ercFailed,
		
		/** \brief Game exited. */
		ercGameExited,
	};
	
	/** \brief Properties. */
	enum eProperties{
		/** \brief Engine Config Path. */
		epPathEngineConfig,
		
		/** \brief Engine Share Path. */
		epPathEngineShare,
		
		/** \brief Engine Lib path. */
		epPathEngineLib,
		
		/** \brief Engine cache path. */
		epPathEngineCache
	};
	
	/** \brief Log types. */
	enum eLogTypes{
		/** \brief Information. */
		eltInfo,
		
		/** \brief Warning. */
		eltWarn,
		
		/** \brief Error. */
		eltError
	};
	
	
	
private:
	#ifdef OS_W32
	HANDLE pPipeIn;
	HANDLE pPipeOut;
	
	#else
	int pPipeIn;
	int pPipeOut;
	#endif
	
	deEngine *pEngine;
	bool pEngineRunning;
	bool pStopProcess;
	delEngineProcessRunGame *pRunGame;
	
	deLogger::Ref pLogger;
	decString pLogSource;
	bool pUseConsole;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance process. */
	#ifdef OS_W32
	delEngineProcess( HANDLE pipeIn, HANDLE pipeOut, const char *logfile,
		const char *logSource = "Engine Process" );
	#else
	delEngineProcess( int pipeIn, int pipeOut, const char *logfile,
		const char *logSource = "Engine Process" );
	#endif
	
	/** \brief Clean up engine instance process. */
	~delEngineProcess();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief In pipe read end. */
	#ifdef OS_W32
	inline HANDLE GetPipeIn() const{ return pPipeIn; }
	#else
	inline int GetPipeIn() const{ return pPipeIn; }
	#endif
	
	/** \brief Out pipe write end. */
	#ifdef OS_W32
	inline HANDLE GetPipeOut() const{ return pPipeOut; }
	#else
	inline int GetPipeOut() const{ return pPipeOut; }
	#endif
	
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	/** \brief Use console. */
	inline bool GetUseConsole() const{ return pUseConsole; }
	
	/** \brief Set use console. */
	void SetUseConsole( bool useConsole );
	
	
	
	/** \brief Run the process. */
	void Run();
	
	/** \brief Start engine. */
	void StartEngine();
	
	/** \brief Stop engine. */
	void StopEngine();
	
	/** \brief Read commands from the in pipe until the process is told to stop. */
	void ReadCommandsFromInPipe();
	
	/** \brief Write unsigned char to pipe. */
	void WriteUCharToPipe( int value );
	
	/** \brief Write uint16_t to pipe. */
	void WriteUShortToPipe( int value );
	
	/** \brief Write int32_t to pipe. */
	void WriteIntToPipe( int value );
	
	/** \brief Write float to pipe. */
	void WriteFloatToPipe( float value );
	
	/** \brief Write string to pipe using uint16_t as length. */
	void WriteString16ToPipe( const char *string );
	
	/** \brief Write data to pipe. */
	void WriteToPipe( const void *data, int length );
	
	/** \brief Read unsigned char from pipe. */
	int ReadUCharFromPipe();
	
	/** \brief Read uint16_t from pipe. */
	int ReadUShortFromPipe();
	
	/** \brief Read float from pipe. */
	float ReadFloatFromPipe();
	
	/** \brief Read string from pipe using uint16_t as length. */
	void ReadString16FromPipe( decString &string );
	
	/** \brief Read data from pipe. */
	void ReadFromPipe( void *data, int length );
	
	
	
	/** \brief Command get property. */
	void CommandGetProperty();
	
	/** \brief Command load modules. */
	void CommandLoadModules();
	
	/** \brief Command get module status. */
	void CommandGetModuleStatus();
	
	/** \brief Command get module parameter list. */
	void CommandGetModuleParamList();
	
	/** \brief Command set module parameter. */
	void CommandSetModuleParameter();
	
	/** \brief Command activate module. */
	void CommandActivateModule();
	
	/** \brief Command enable module. */
	void CommandEnableModule();
	
	/** \brief Command set data directory. */
	void CommandSetDataDir();
	
	/** \brief Command set cache application identifier. */
	void CommandSetCacheAppID();
	
	/**
	 * \brief Command set path overlay.
	 * \version 1.7
	 */
	void CommandSetPathOverlay();
	
	/**
	 * \brief Command set path capture.
	 * \version 1.7
	 */
	void CommandSetPathCapture();
	
	/**
	 * \brief Command set path config.
	 * \version 1.7
	 */
	void CommandSetPathConfig();
	
	/** \brief Command add disk directory to virtual file system. */
	void CommandVFSAddDiskDir();
	
	/** \brief Command add virtual file system container for module shared data. */
	void CommandVFSAddScriptSharedDataDir();
	
	/** \brief Command add delga file to virtual file system as root container. */
	void CommandVFSAddDelgaFile();
	
	/** \brief Command set command line arguments. */
	void CommandSetCmdLineArgs();
	
	/** \brief Command create render window. */
	void CommandCreateRenderWindow();
	
	/** \brief Command start game. */
	void CommandStartGame();
	
	/** \brief Command get current display resolutions. */
	void CommandGetDisplayCurrentResolution();
	
	/** \brief Command get display resolutions. */
	void CommandGetDisplayResolutions();
	
	/** \brief Command reading game definitions from DELGA file. */
	void CommandDelgaReadGameDefs();
	
	/** \brief Command reading game patch definitions from DELGA file. */
	void CommandDelgaReadPatchDefs();
	
	/** \brief Command reading files from DELGA file. */
	void CommandDelgaReadFiles();
	/*@}*/
	
	
	
private:
	void pCreateLogger( const char *logfile );
};

#endif
