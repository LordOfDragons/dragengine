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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spScriptFile.h"
#include "spTypeModuleLoader.h"
#include "../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Initializers
/////////////////

static const PyModuleDef vModuleDefinitionInitializer = {
	PyModuleDef_HEAD_INIT,
	NULL,
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};



// Native Data
////////////////

struct sModuleData{
	spScriptFile *ownerClass;
};



// Class spScriptFile
///////////////////////

// Constructor, destructor
////////////////////////////

spScriptFile::spScriptFile( ScriptingPython &sp, const char *fullname, const char *path ) :
pSP( sp )
{
	if( ! fullname || ! path ){
		DETHROW( deeInvalidParam );
	}
	
	pFullModuleName = fullname;
	pPath = path;
	
	pPyModuleDefinition = NULL;
	pPyModule = NULL;
	
	const decStringList parts = pFullModuleName.Split( '.' );
	pModuleName = parts.GetAt( parts.GetCount() - 1 );
}

spScriptFile::~spScriptFile(){
	if( pPyModuleDefinition ){
		delete pPyModuleDefinition;
		pPyModuleDefinition = NULL;
	}
}



// Management
///////////////



spScriptFile *spScriptFile::GetOwnerClass( PyObject *object ){
	if( ! object ){
		DETHROW( deeNullPointer );
	}
	
	return ( ( sModuleData* )PyModule_GetState( object ) )->ownerClass;
}



void spScriptFile::PythonCreate(){
	pSP.LogInfoFormat( "Script-File %s PythonCreate", pFullModuleName.GetString() );
	
	pCreateModuleDefinition();
	pCreateModule();
	pLoadFile();
}

void spScriptFile::PythonCleanUp(){
	pSP.LogInfoFormat( "Script-File %s PythonCleanUp", pFullModuleName.GetString() );
	
	if( pPyModule ){
		Py_DECREF( pPyModule );
		pPyModule = NULL;
	}
}



// Private Functions
//////////////////////

void spScriptFile::pCreateModuleDefinition(){
	decString fullModuleName;
	
	if( pPyModuleDefinition ){
		delete pPyModuleDefinition;
		pPyModuleDefinition = NULL;
	}
	
	pPyModuleDefinition = new PyModuleDef;
	memcpy( pPyModuleDefinition, &vModuleDefinitionInitializer, sizeof( PyMethodDef ) );
	
	pPyModuleDefinition->m_name = pFullModuleName.GetString();
	pPyModuleDefinition->m_doc = "Script file module";
	pPyModuleDefinition->m_size = sizeof( sModuleData );
	pPyModuleDefinition->m_methods = NULL;
	pPyModuleDefinition->m_reload = NULL;
	pPyModuleDefinition->m_traverse = NULL;
	pPyModuleDefinition->m_clear = NULL;
	pPyModuleDefinition->m_free = NULL;
}

void spScriptFile::pCreateModule(){
	if( pPyModule ){
		Py_DECREF( pPyModule );
		pPyModule = NULL;
	}
	
	pSP.LogInfoFormat( "Script-File %s create module", pFullModuleName.GetString() );
	
	pPyModule = PyModule_Create( pPyModuleDefinition );
	if( ! pPyModule ){
		DETHROW( deeInvalidAction );
	}
	
	( ( sModuleData* )PyModule_GetState( pPyModule ) )->ownerClass = this;
	
	PyDict_SetItemString( PyImport_GetModuleDict(), pModuleName.GetString(), pPyModule );
	
	decString TEST;
	TEST.Format( "m=sys.modules['%s']\nfor x in dir(m):\n\tprint( (x,m.__dict__[x]) )\n", pModuleName.GetString() );
	PyRun_SimpleString( TEST.GetString() );
}

void spScriptFile::pLoadFile(){
	deVirtualFileSystem &vfsGame = *pSP.GetGameEngine()->GetVirtualFileSystem();
	deVirtualFileSystem &vfsShared = pSP.GetVFS();
	decBaseFileReader *reader = NULL;
	decPath lookupPath;
	decPath vfsPath;
	decString content;
	int size;
	
	// determine the look up path. this one is tested against the possible virtual file systems
	pSP.GetTypeModuleLoader()->ParseImportPath( NULL, pFullModuleName.GetString(), lookupPath );
	
	try{
		// try to load the script file from the matching virtual file system
		vfsPath.SetFromUnix( pSP.GetScriptPath() );
		vfsPath.Add( lookupPath );
		
		if( vfsGame.ExistsFile( vfsPath ) ){
			pSP.LogInfoFormat( "Loading '%s' in game scripts", vfsPath.GetPathUnix().GetString() );
			reader = vfsGame.OpenFileForReading( vfsPath );
			
		}else{
			vfsPath.SetFromUnix( "/shared" );
			vfsPath.Add( lookupPath );
			pSP.LogInfoFormat( "Looking up '%s' in engine scripts", vfsPath.GetPathUnix().GetString() );
			reader = vfsShared.OpenFileForReading( vfsPath );
		}
		
		size = reader->GetLength();
		reader->SetPosition( 0 );
		
		content.Set( ' ', size );
		reader->Read( (char*)content.GetString(), size );
		
		reader->FreeReference();
		
	}catch( const deException &e ){
		if( reader ){
			reader->FreeReference();
		}
		throw;
	}
	
	// compiling the code is a bit tricky. python is particular in how namespaces are set up and
	// how modules are loaded. in particular it is necessary to add the builtins to the module
	// dictionary before compiling the code. then the module dictionary has to be used as global
	// and local dictionary. the global dictionary is used for the lookup while the compiled
	// content is added to the local dictionary. hence to get the namespace working as expected
	// both global and local dictionary has to point to the prepared module dictionary
	PyObject * const moduleDict = PyModule_GetDict( pPyModule ); // borrowed reference
	PyDict_SetItemString( moduleDict, "__builtins__", PyEval_GetBuiltins() );
	
	// set module parameters
	PyObject *pyoString;
	
	//pyoString = PyUnicode_FromString( pPath.GetString() );
	//PyDict_SetItemString( moduleDict, "__path__", pyoString );
	//PyDict_SetItemString( moduleDict, "__path__", Py_None );
	//Py_DECREF( pyoString );
	
	pyoString = PyUnicode_FromFormat( "<%s>", vfsPath.GetPathUnix().GetString() );
	PyDict_SetItemString( moduleDict, "__file__", pyoString );
	Py_DECREF( pyoString );
	
	//pyoString = PyUnicode_FromString( "" );
	//PyDict_SetItemString( moduleDict, "__package__", pyoString );
	//Py_DECREF( pyoString );
	
	//PyDict_SetItemString( moduleDict, "__loader__", myself );
	
	if( ! PyRun_StringFlags( content.GetString(), Py_file_input, moduleDict, moduleDict, NULL ) ){
		DETHROW( deeInvalidAction );
	}
}

#if 0
	/*
The loader should set several attributes on the module. (Note that some of these attributes can change when a module is reloaded):

    __name__

        The name of the module.

    __file__

        The path to where the module data is stored (not set for built-in modules).

    __cached__

        The path to where a compiled version of the module is/should be stored (not set when the attribute would be inappropriate).

    __path__

        A list of strings specifying the search path within a package. This attribute is not set on modules.

    __package__

        The parent package for the module/package. If the module is top-level then it has a value of the empty string. The importlib.util.module_for_loader() decorator can handle the details for __package__.

    __loader__

        The loader used to load the module. The importlib.util.module_for_loader() decorator can handle the details for __package__.

	*/
	
	/*
	Here is a minimal pattern for a load_module() method:

	# Consider using importlib.util.module_for_loader() to handle
	# most of these details for you.
	def load_module(self, fullname):
		code = self.get_code(fullname)
		ispkg = self.is_package(fullname)
		mod = sys.modules.setdefault(fullname, imp.new_module(fullname))
		mod.__file__ = "<%s>" % self.__class__.__name__
		mod.__loader__ = self
		if ispkg:
			mod.__path__ = []
			mod.__package__ = fullname
		else:
			mod.__package__ = fullname.rpartition('.')[0]
		exec(code, mod.__dict__)
		return mod
	*/
	
	/*
	('__doc__', 'Loaded module')
	('__file__', '</scripts/Game.py>')
	('__loader__', <Dragengine.ModuleLoader object at 0x7fd88c087070>)
	('__name__', 'Game')
	('__package__', '')
	('__spec__', None)
	*/
	
	// https://docs.python.org/3/library/importlib.html?highlight=load%20module#importlib.abc.Loader.exec_module
#endif
