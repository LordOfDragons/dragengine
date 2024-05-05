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
