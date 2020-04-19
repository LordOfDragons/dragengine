/* 
 * Drag[en]gine Python Script Module
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

#ifndef _SPSCRIPTFILELIST_H_
#define _SPSCRIPTFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

#include "../spCommon.h"

class spScriptFile;
class ScriptingPython;



/**
 * \brief List of python modules loaded from a script files.
 */
class spScriptFileList{
private:
	ScriptingPython *pSP;
	decObjectOrderedSet pFiles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new list. */
	spScriptFileList( ScriptingPython *sp );
	/** \brief Cleans up the list. */
	~spScriptFileList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of files. */
	int GetCount() const;
	/** \brief Retrieves the file at the given position. */
	spScriptFile *GetAt( int index ) const;
	/** \brief Retrieves the file with the given fullname or NULL if not found. */
	spScriptFile *GetWithFullName( const char *fullname ) const;
	/** \brief Retrieves the index of the given file or -1 if not found. */
	int IndexOf( spScriptFile *file ) const;
	/** \brief Retrieves the index of the file with the given fullname or -1 if not found. */
	int IndexOfWithFullName( const char *fullname ) const;
	/** \brief Determines if a file exists. */
	bool Has( spScriptFile *file ) const;
	/** \brief Determines if a file with the given fullname exists. */
	bool HasWithFullName( const char *fullname ) const;
	/** \brief Adds a file. */
	void Add( spScriptFile *file );
	/** \brief Removes a file. */
	void Remove( spScriptFile *file );
	/** \brief Removes all files. */
	void RemoveAll();
	
	/** \brief Call python clean up on all files. */
	void PythonCleanUpAll();
	/*@}*/
};

#endif
