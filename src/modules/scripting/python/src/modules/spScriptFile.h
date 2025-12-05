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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<spScriptFile> Ref;


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
