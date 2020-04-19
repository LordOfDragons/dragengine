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

#ifndef _SPBASEMODULE_H_
#define _SPBASEMODULE_H_

#include <dragengine/common/string/decString.h>

#include "../spCommon.h"

class ScriptingPython;



/**
 * \brief Base class managing a python module.
 */
class spBaseModule{
private:
	struct sFunction{
		decString name;
		PyCFunction pyFunction;
		int argumentType;
		decString documentation;
	};
	
private:
	ScriptingPython &pSP;
	
	spBaseModule *pParent;
	decString pName;
	decString pDocumentation;
	
	sFunction **pFunctions;
	int pFunctionCount;
	
	decString pFullModuleName;
	
	PyModuleDef *pPyModuleDefinition;
	PyMethodDef *pPyMethodDefs;
	PyObject *pPyModule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new base module. */
	spBaseModule( ScriptingPython &sp, const char *name );
	/** \brief Cleans up the base module. */
	virtual ~spBaseModule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline ScriptingPython &GetSP() const{ return pSP; }
	
	/** \brief Retrieves the module name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Retrieves the module documentation. */
	inline const decString &GetDocumentation() const{ return pDocumentation; }
	/** \brief Sets the module documentation. */
	void SetDocumentation( const char *documentation );
	/** \brief Retrieves the parent or NULL if located in globals. */
	inline spBaseModule *GetParent() const{ return pParent; }
	/** \brief Sets the parent or NULL if located in globals. */
	void SetParent( spBaseModule *parent );
	
	/** \brief Adds a method. */
	void AddMethod( const char *name, PyCFunction pyFunction, int argumentType, const char *documentation );
	
	/** \brief Retrieves the full module name. */
	inline const decString &GetFullModuleName() const{ return pFullModuleName; }
	
	/** \brief Retrieve owner class of a python object. */
	static spBaseModule *GetOwnerClass( PyObject *object );
	
	/** \brief Retrieves the python module. */
	inline PyObject *GetPyModule() const{ return pPyModule; }
	
	/** \brief Python create module. */
	virtual void PythonCreate();
	/** \brief Python clean up. */
	virtual void PythonCleanUp();
	/*@}*/
	
private:
	void pUpdateFullName();
	void pCreateMethodsDefinition();
	void pCreateModuleDefinition();
	void pCreateModule();
};

#endif
