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

#ifndef _SPTYPEMODULELOADER_H_
#define _SPTYPEMODULELOADER_H_

#include "spBaseType.h"

class decPath;



/**
 * \brief Module loader type. Implements loading module scripts through Drag[en]gine VFS.
 */
class spTypeModuleLoader : public spBaseType{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module. */
	spTypeModuleLoader( ScriptingPython &sp );
	/** \brief Cleans up the module. */
	virtual ~spTypeModuleLoader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Parse import path. */
	void ParseImportPath( const char *path, const char *fullname, decPath &vfsPath ) const;
	
	/** \brief Python create. */
	virtual void PythonCreate();
	/*@}*/
	
	/** @name Functions */
	/*@{*/
	static PyObject *cfFindModule( PyObject *myself, PyObject *args );
	static PyObject *cfLoadModule( PyObject *myself, PyObject *args );
	/*@}*/
};

#endif
