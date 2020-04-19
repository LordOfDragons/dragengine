/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALASSETUNPACKER_H_
#define _DEALASSETUNPACKER_H_

#include <android/asset_manager.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "unzip.h"
#include "common/string/decString.h"
#include "common/string/decStringList.h"

class deLogger;
class decBaseFileWriter;



/**
 * \brief Asset unpacker.
 * 
 * Unpack zip archive from android asset into application directory.
 */
class dealAssetUnpacker{
public:
	/** \brief Status code. */
	enum eStatusCodes{
		/** \brief Success. */
		escSuccess,
		
		/** \brief Generic failure. */
		escFailed,
		
		/** \brief Mutex failure. */
		escMutex,
		
		/** \brief Thread failure. */
		escThread,
		
		/** \brief Already unpacking. */
		escUnpacking,
		
		/** \brief Failed reading asset. */
		escReadAsset,
		
		/** \brief Write file failed. */
		escWriteFile
	};
	
private:
	deLogger *pLogger;
	AAsset *pAsset;
	decString pTargetDirectory;
	bool pUnpacking;
	bool pStopUnpacking;
	float pUnpackProgress;
	bool pUnpackFailed;
	bool pEnableLogging;
	pthread_t pThreadUnpack;
	pthread_mutex_t pMutex;
	decStringList pFilter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create asset unpacker. */
	dealAssetUnpacker( deLogger *logger );
	
	/** \brief Clean up asset unpacker. */
	~dealAssetUnpacker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Start unpacking. */
	void StartUnpacking( AAssetManager *assetManager, const char *assetFilename,
		const char *targetDirectory, const decStringList &filter );
	
	/**
	 * \brief Get unpack progress.
	 * \param[out] unpacking \em true if still unpacking or \em false if finished.
	 * \param[out] progress Unpack progress in the range from 0 and 1.
	 */
	void UnpackProgress( bool &unpacking, float &progress );
	
	/** \brief Stop unpacking. */
	void StopUnpacking();
	/*@}*/
	
private:
	static void *pThreadRunnerUnpack( void *parameter );
	void pUnpack();
	void pCreateDirectory( const decString &filename );
	decBaseFileWriter *pWriteFile( const decString &filename );
};

#endif
