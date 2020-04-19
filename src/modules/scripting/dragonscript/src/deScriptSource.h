/* 
 * Drag[en]gine DragonScript Script Module
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

// include only once
#ifndef _DESCRIPTSOURCE_H_
#define _DESCRIPTSOURCE_H_

// includes
#include "dragengine/common/file/decPath.h"
#include <libdscript/dsScriptSource.h>

// predefinitions
class deVirtualFileSystem;
class decBaseFileReader;
class decPath;



// script source class
class deScriptSource : public dsScriptSource{
private:
	const deVirtualFileSystem &pVFS;
	decBaseFileReader *pFileReader;
	const decPath pPath;
	decString pPathString;
	int pPosition;
	int pLength;
	
public:
	// Constructor, destructor
	deScriptSource( const deVirtualFileSystem &vfs, const decPath &path );
	virtual ~deScriptSource();
	
	// Management
	virtual const char *GetName();
	virtual void Open();
	virtual int ReadData(char *Buffer, int Size);
	virtual void Close();
};

// end of include only once
#endif
