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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spTypeModuleLoader.h"
#include "spScriptFile.h"
#include "spScriptFileList.h"
#include "../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class spTypeModuleLoader
/////////////////////////////

// Constructor, destructor
////////////////////////////

spTypeModuleLoader::spTypeModuleLoader(ScriptingPython &sp) : spBaseType(sp, "ModuleLoader"){
	SetDocumentation("Drag[en]gine module loader");
	SetParent(sp.GetNamespaceDragengine());
	
	AddMethod("find_module", (PyCFunction)cfFindModule, METH_VARARGS, "Retrieve finder for a path.");
	AddMethod("load_module", (PyCFunction)cfLoadModule, METH_VARARGS, "Load module for a path.");
}

spTypeModuleLoader::~spTypeModuleLoader(){
}



// Management
///////////////

void spTypeModuleLoader::ParseImportPath(const char *path, const char *fullname,
decPath &vfsPath) const{
	decStringList components;
	decString scriptfile;
	int componentCount;
	int i;
	
	vfsPath.SetFromUnix("/");
	
	if(path){
		components = decString(path).Split('.');
		componentCount = components.GetCount();
		
		for(i=0; i<componentCount; i++){
			vfsPath.AddComponent(components.GetAt(i).GetString());
		}
	}
	
	components = decString(fullname).Split('.');
	componentCount = components.GetCount();
	
	for(i=0; i<componentCount; i++){
		vfsPath.AddComponent(components.GetAt(i).GetString());
	}
	
	scriptfile = vfsPath.GetLastComponent();
	scriptfile.Append(".py");
	vfsPath.RemoveLastComponent();
	vfsPath.AddComponent(scriptfile.GetString());
}



void spTypeModuleLoader::PythonCreate(){
	spBaseType::PythonCreate();
	
	// add loader to the list of known loaders
	PyRun_SimpleString(
		"import sys\n"
		"import Dragengine\n"
		"sys.meta_path.append(Dragengine.ModuleLoader())\n");
}



// Functions
//////////////

PyObject *spTypeModuleLoader::cfFindModule(PyObject *myself, PyObject *args){
	const spTypeModuleLoader &clsModuleLoader = *((spTypeModuleLoader*)GetOwnerClass(myself));
	ScriptingPython &sp = clsModuleLoader.GetSP();
	deVirtualFileSystem &vfsGame = *sp.GetGameEngine()->GetVirtualFileSystem();
	deVirtualFileSystem &vfsShared = sp.GetVFS();
	const char *fullname = NULL;
	const char *path = NULL;
	decPath lookupPath;
	decPath vfsPath;
	bool found;
	
	if(!PyArg_ParseTuple(args, "sz", &fullname, &path)){
		return NULL;
	}
	
	sp.LogInfoFormat("Module %s cfFindModule fullname='%s' path='%s'", clsModuleLoader.GetFullTypeName().GetString(), fullname, path);
	
	// determine the look up path. this one is tested against the possible virtual file systems
	clsModuleLoader.ParseImportPath(path, fullname, lookupPath);
	
	// try to find the path in the game scripts
	vfsPath.SetFromUnix(sp.GetScriptPath());
	vfsPath.Add(lookupPath);
	sp.LogInfoFormat("Looking up '%s' in game scripts", vfsPath.GetPathUnix().GetString());
	found = vfsGame.ExistsFile(vfsPath);
	
	// if not found try to find the path in the shared scripts
	if(!found){
		vfsPath.SetFromUnix("/share/scripts");
		vfsPath.Add(lookupPath);
		sp.LogInfoFormat("Looking up '%s' in engine scripts", vfsPath.GetPathUnix().GetString());
		found = vfsShared.ExistsFile(vfsPath);
	}
	
	// if the file exists we can load it
	if(found){
		Py_INCREF(myself);
		return myself;
		
	}else{
		Py_INCREF(Py_None);
		return Py_None;
	}
}

PyObject *spTypeModuleLoader::cfLoadModule(PyObject *myself, PyObject *args){
	const spTypeModuleLoader &clsModuleLoader = *((spTypeModuleLoader*)GetOwnerClass(myself));
	ScriptingPython &sp = clsModuleLoader.GetSP();
	spScriptFile *scriptFile = NULL;
	PyObject *loadedModule = NULL;
	const char *fullname = NULL;
	decPath lookupPath;
	
	if(!PyArg_ParseTuple(args, "s", &fullname)){
		return NULL;
	}
	
	// determine the look up path. this one is tested against the possible virtual file systems
	clsModuleLoader.ParseImportPath(NULL, fullname, lookupPath);
	
	sp.LogInfoFormat("Module %s cfLoadModule fullname='%s'", clsModuleLoader.GetFullTypeName().GetString(), fullname);
	
	// load script file
	try{
		// load script file
		scriptFile = new spScriptFile(clsModuleLoader.GetSP(), fullname, lookupPath.GetPathUnix());
		scriptFile->PythonCreate();
		
		loadedModule = scriptFile->GetPyModule();
		
		// register loaded module
		sp.GetScriptFileList().Add(scriptFile);
		scriptFile->FreeReference();
		
	}catch(const deException &e){
		sp.LogException(e);
		if(scriptFile){
			scriptFile->PythonCleanUp();
			scriptFile->FreeReference();
		}
		
		PyErr_SetString(PyExc_RuntimeError, "Failed to load module!");
		return NULL;
	}
	
	if(loadedModule){
		Py_INCREF(loadedModule);
	}
	return loadedModule;
}
