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

#ifndef _SPBASETYPE_H_
#define _SPBASETYPE_H_

#include <dragengine/common/string/decString.h>

#include "../spCommon.h"

class spBaseModule;
class ScriptingPython;



/**
 * \brief Base native python type.
 */
class spBaseType{
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
	int pNativeDataSize;
	bool pPrivateNew;
	
	sFunction **pFunctions;
	int pFunctionCount;
	
	decString pFullTypeName;
	
	PyMethodDef *pPyMethodDefs;
	PyTypeObject *pPyType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new base type. */
	spBaseType( ScriptingPython &sp, const char *name );
	/** \brief Cleans up the base type. */
	virtual ~spBaseType();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the type. */
	inline ScriptingPython &GetSP() const{ return pSP; }
	
	/** \brief Retrieves the type name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Retrieves the type documentation. */
	inline const decString &GetDocumentation() const{ return pDocumentation; }
	/** \brief Sets the type documentation. */
	void SetDocumentation( const char *documentation );
	/** \brief Retrieves the parent module or NULL if located in globals. */
	inline spBaseModule *GetParent() const{ return pParent; }
	/** \brief Sets the parent module or NULL if located in globals. */
	void SetParent( spBaseModule *parent );
	/** \brief Retrieves the size of the native object data. */
	inline int GetNativeDataSize() const{ return pNativeDataSize; }
	/** \brief Sets the size of the native object data. */
	void SetNativeDataSize( int size );
	/** \brief Determines if the object new method is private. */
	inline bool GetPrivateNew() const{ return pPrivateNew; }
	/** \brief Sets if the object new method is private. */
	void SetPrivateNew( bool privateNew );
	
	/** \brief Adds a method. */
	void AddMethod( const char *name, PyCFunction pyFunction, int argumentType, const char *documentation );
	
	/** \brief Retrieves the full type name. */
	inline const decString &GetFullTypeName() const{ return pFullTypeName; }
	
	/** \brief Retrieve owner class of a python object. */
	static spBaseType *GetOwnerClass( PyObject *object );
	/** \brief Retrieve native data of a python object. */
	static void *GetObjectData( PyObject *object );
	/** \brief Create a new python object. */
	PyObject *CreatePyObject();
	
	/** \brief Python create. */
	virtual void PythonCreate();
	/** \brief Python clean up. */
	virtual void PythonCleanUp();
	
	/** Constructor. */
	virtual void Constructor( PyObject *myself, PyObject *args, PyObject *kwds );
	/** Destructor. */
	virtual void Destructor( PyObject *myself );
	/*@}*/
	
private:
	static PyObject *cfNew( PyTypeObject *type, PyObject *args, PyObject *kwds );
	static int cfInit( PyObject *myself, PyObject *args, PyObject *kwds );
	static void cfDealoc( PyObject *myself );
	
	void pUpdateFullName();
	void pCreateMethodsDefinition();
	void pCreateType();
};

#endif
