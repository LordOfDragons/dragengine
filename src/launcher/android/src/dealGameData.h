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
