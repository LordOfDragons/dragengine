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
