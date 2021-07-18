/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
class delEngineProcess{
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
		eccReadDelgaFiles
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
	int ReadFloatFromPipe();
	
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
