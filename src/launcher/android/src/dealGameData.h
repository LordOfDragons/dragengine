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

#ifndef _DEALGAMEDATA_H_
#define _DEALGAMEDATA_H_

#include "dealJniHelper.h"
#include "deObject.h"
#include "game/dealGameList.h"

class dealLauncher;
class dealFDFileReader;



/**
 * \brief Game data stored in a *.delga file.
 * 
 * File conforms to DELGA file format which is a ZIP archive. Opens the DELGA
 * file using the appropriate method storing a Unix type file descriptor.
 * Using this file descriptor the DELGA is read storing all found game
 * definitions as dealGame objects. The file descriptor is then ready to
 * be used as virtual file container for launching games.
 */
class dealGameData : public deObject{
private:
	dealLauncher &pLauncher;
	jniGlobalJObject pObjGameData;
	int pFileDescriptor;
	long pFileOffset;
	long pFileLength;
	dealGameList pGames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	dealGameData( dealLauncher &launcher, jobject objUri );
	
protected:
	/** \brief Clean up game data. */
	virtual ~dealGameData();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game data object mapped to virtual file system. */
	inline jniGlobalJObject &GetObjGameData(){ return pObjGameData; }
	
	/** \brief File descriptor. */
	inline int GetFileDescriptor() const{ return pFileDescriptor; }
	
	/** \brief Offset in bytes where content begins. */
	inline long GetFileOffset() const{ return pFileOffset; }
	
	/** \brief Length of content in bytes. */
	inline long GetFileLength() const{ return pFileLength; }
	
	/** \brief Game definitions found in the game data file. */
	inline const dealGameList &GetGames() const{ return pGames; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pOpenParcelFileDescriptor( jobject objUri );
	void pReadGameDefinitions();
	void pCloseParcelFileDescriptor();
};

#endif
