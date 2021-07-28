/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLCALCULATEDIRECTORYSIZE_H_
#define _DEGLCALCULATEDIRECTORYSIZE_H_

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deThread.h>

class decPath;



/**
 * Thread calculating the size of a disc directory.
 */
class deglCalculateDirectorySize : public deThread{
private:
	const decString pDirectory;
	const deVirtualFileSystem::Ref pVFS;
	uint64_t pSize;
	bool pAbort;
	bool pFailed;
	deMutex pMutex;
	decStringList pException;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create. */
	deglCalculateDirectorySize( const char *directory );
	
	/** Clean up. */
	virtual ~deglCalculateDirectorySize();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Directory to scan. */
	decString GetDirectory();
	
	/** Directory size. */
	uint64_t GetSize();
	
	/** Calculating size failed. */
	bool GetFailed();
	
	/** Exception if failed. */
	decStringList GetException();
	
	/** Abort thread. */
	void Abort();
	
	
	/** Run function of the thread. */
	virtual void Run();
	
	/** For internal use only. */
	void IncrementSize( int size );
	
	
	
private:
	void pSetSize( int size );
	void pScanDirectory( const decPath &directory );
};

#endif
