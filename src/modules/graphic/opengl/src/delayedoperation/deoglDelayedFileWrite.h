/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDELAYEDFILEWRITE_H_
#define _DEOGLDELAYEDFILEWRITE_H_

#include <dragengine/common/file/decPath.h>

class decMemoryFile;
class deVirtualFileSystem;
class decBaseFileWriter;



/**
 * Delayed file write.
 */
class deoglDelayedFileWrite{
private:
	decMemoryFile *pMemoryFile;
	const decPath pPath;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create delayed file write object. */
	deoglDelayedFileWrite( const decPath &path );
	
	/** Clean up delayed file write object. */
	~deoglDelayedFileWrite();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create file writer for the memory file. */
	decBaseFileWriter *GetFileWriter() const;
	
	/**
	 * Write file to virtual file system.
	 * \details This has to be called during main thread.
	 */
	void SaveFile( deVirtualFileSystem &vfs );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
