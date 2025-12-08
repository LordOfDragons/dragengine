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

#ifndef _DEALASSETUNPACKER_H_
#define _DEALASSETUNPACKER_H_

#include <android/asset_manager.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "unzip.h"
#include "common/string/decString.h"
#include "common/string/decStringList.h"
#include "deLogger.h"

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
	deLogger::Ref pLogger;
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
	dealAssetUnpacker(deLogger *logger);
	
	/** \brief Clean up asset unpacker. */
	~dealAssetUnpacker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Start unpacking. */
	void StartUnpacking(AAssetManager *assetManager, const char *assetFilename,
		const char *targetDirectory, const decStringList &filter);
	
	/**
	 * \brief Get unpack progress.
	 * \param[out] unpacking \em true if still unpacking or \em false if finished.
	 * \param[out] progress Unpack progress in the range from 0 and 1.
	 */
	void UnpackProgress(bool &unpacking, float &progress);
	
	/** \brief Stop unpacking. */
	void StopUnpacking();
	/*@}*/
	
private:
	static void *pThreadRunnerUnpack(void *parameter);
	void pUnpack();
	void pCreateDirectory(const decString &filename);
	decBaseFileWriter *pWriteFile(const decString &filename);
};

#endif
