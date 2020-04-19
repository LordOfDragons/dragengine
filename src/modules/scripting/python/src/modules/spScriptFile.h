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

#ifndef _SPSCRIPTFILE_H_
#define _SPSCRIPTFILE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

#include "../spCommon.h"

class decPath;
class ScriptingPython;



/**
 * \brief Python module loaded from a script file.
 */
class spScriptFile : public deObject{
private:
	ScriptingPython &pSP;
	
	decString pPath;
	decString pFullModuleName;
	decString pModuleName;
	
	PyModuleDef *pPyModuleDefinition;
	PyObject *pPyModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module. */
	spScriptFile( ScriptingPython &sp, const char *fullname, const char *path );
	/** \brief Cleans up the module. */
	virtual ~spScriptFile();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline ScriptingPython &GetSP() const{ return pSP; }
	
	/** \brief Retrieves the path of the script file. */
	inline const decString &GetPath() const{ return pPath; }
	/** \brief Retrieves the full module name. */
	inline const decString &GetFullModuleName() const{ return pFullModuleName; }
	/** \brief Retrieves the module name. */
	inline const decString &GetModuleName() const{ return pModuleName; }
	
	/** \brief Retrieve owner class of a python object. */
	static spScriptFile *GetOwnerClass( PyObject *object );
	
	/** \brief Retrieves the python module. */
	inline PyObject *GetPyModule() const{ return pPyModule; }
	
	/** \brief Python create module. */
	virtual void PythonCreate();
	/** \brief Python clean up. */
	virtual void PythonCleanUp();
	/*@}*/
	
private:
	void pCreateModuleDefinition();
	void pCreateModule();
	void pLoadFile();
	void pParseImportPath( decPath &vfsPath ) const;
};

#endif
