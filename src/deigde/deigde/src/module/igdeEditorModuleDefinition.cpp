/* 
 * Drag[en]gine IGDE
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

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "igdeEditorModuleDefinition.h"
#include "igdeEditorModuleManager.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../gui/igdeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Definitions
////////////////

typedef igdeEditorModule* ( *FUNC_CREATEMODULE )( igdeEnvironment *environment );

#define LOGSOURCE "IGDE"



// Class igdeEditorModuleDefinition
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditorModuleDefinition::igdeEditorModuleDefinition(
igdeEditorModuleManager &moduleManager, const char *filePath ) :
pModuleManager( moduleManager ),
pFilePath( filePath ),
pLibSize( 0 ),
pLibHandle( NULL ),
pCanLoad( false ),
pErrorCode( eecSuccess ),
pModule( NULL )
{
	pLoadFile();
	pVerify();
	
	pLoggingName.Format( "Editor %s", pName.GetString() );
}

igdeEditorModuleDefinition::~igdeEditorModuleDefinition(){
	if( IsModuleRunning() ) UnloadModule();
	pUnloadLibrary();
}



// Management
///////////////

void igdeEditorModuleDefinition::SetDirectoryName( const char *directoryName ){
	pDirectoryName = directoryName;
}

bool igdeEditorModuleDefinition::IsModuleRunning() const{
	return pModule != NULL;
}

bool igdeEditorModuleDefinition::LoadModule( igdeEnvironment *environment ){
	if( ! environment ){
		DETHROW( deeInvalidParam );
	}
	
	if( pModule || ! pCanLoad ){
		return false;
	}
	
	const decPath basePathEditorLib( decPath::CreatePathNative( pModuleManager.GetPathModules() ) );
	
	try{
		if( ! pLoadLibrary() ){
			return false;
		}
		if( ! pCreateModule( environment ) ){
			return false;
		}
		
		pModule->SetLoggingName( pLoggingName );
		pModule->SetEditorDirectory( pDirectoryName );
		
		decPath pathEditorLib( basePathEditorLib );
		pathEditorLib.AddComponent( pDirectoryName );
		pModule->SetEditorPathLib( pathEditorLib.GetPathNative() );
		
		pModule->Start();
		
		if( environment->GetEngineController()->GetRunning() ){
			pModule->OnBeforeEngineStart();
			pModule->OnAfterEngineStart();
		}
		
	}catch( const deException &e ){
		pModuleManager.GetWindowMain().GetLogger()->LogException( LOGSOURCE, e );
		if( pModule ){
			delete pModule;
			pModule = NULL;
		}
		
		return false;
	}
	
	return true;
}

void igdeEditorModuleDefinition::UnloadModule(){
	if( ! pModule ){
		return;
	}
	
	pModule->Stop();
	delete pModule;
	pModule = NULL;
}



// Private Functions
//////////////////////

void igdeEditorModuleDefinition::pLoadFile(){
	decBaseFileReaderReference reader;
	reader.TakeOver( new decDiskFileReader( pFilePath ) );
	pParseFile( reader );
}

void igdeEditorModuleDefinition::pParseFile( decBaseFileReader& reader ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	int i, j;
	
	// set base path
	decPath basePath;
	basePath.SetFromNative( pFilePath );
	basePath.RemoveLastComponent();
	pID = basePath.GetLastComponent();
	pDirectoryName = basePath.GetLastComponent();
	
	// parse xml file
	decXmlParser parser( pModuleManager.GetWindowMain().GetLogger() );
	parser.ParseXml( &reader, xmlDoc );
	
	// some cleanup visiting
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	// reset parameters so we can test later on
	pName = "";
	pDescription = "";
	pAuthor = "";
	pVersion = "";
	
	// parse xml tags
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "module" ){
		DETHROW( deeInvalidParam );
	}
	
	for( i=0; i<root->GetElementCount(); i++ ){
		decXmlElement * const element = root->GetElementAt( i );
		if( ! element->CanCastToElementTag() ){
			continue;
		}
		
		const decXmlElementTag &tag = *element->CastToElementTag();
		
		if( tag.GetName() == "name" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pName = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "description" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pDescription = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "author" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pAuthor = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "version" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pVersion = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "iconSmall" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pIconSmall = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "iconLarge" ){
			if( ! tag.GetFirstData() ){
				DETHROW( deeInvalidParam );
			}
			pIconLarge = tag.GetFirstData()->GetData();
			
		}else if( tag.GetName() == "library" ){
			for( j=0; j<tag.GetElementCount(); j++ ){
				decXmlElement * const element2 = tag.GetElementAt( j );
				if( ! element2->CanCastToElementTag() ){
					continue;
				}
				
				const decXmlElementTag &tag2 = *element2->CastToElementTag();
				
				if( tag2.GetName() == "path" ){
					if( ! tag2.GetFirstData() ){
						DETHROW( deeInvalidParam );
					}
					const decString &libFilePath = tag2.GetFirstData()->GetData();
					
					if( decPath::IsNativePathAbsolute( libFilePath ) ){
						pLibPath = libFilePath;
						
					}else{
						decPath libPath( basePath );
						libPath.AddNativePath( libFilePath );
						pLibPath = libPath.GetPathNative();
					}
					
				}else if( tag2.GetName() == "size" ){
					if( ! tag2.GetFirstData() ){
						DETHROW( deeInvalidParam );
					}
					pLibSize = tag2.GetFirstData()->GetData().ToInt();
					
				}else if( tag2.GetName() == "sha1" ){
					if( ! tag2.GetFirstData() ){
						DETHROW( deeInvalidParam );
					}
					pLibHash = tag2.GetFirstData()->GetData();
					
				}else if( tag2.GetName() == "entrypoint" ){
					if( ! tag2.GetFirstData() ){
						DETHROW( deeInvalidParam );
					}
					pEntryPoint = tag2.GetFirstData()->GetData();
				}
			}
		}
	}
}

bool igdeEditorModuleDefinition::pLoadLibrary(){
	deLogger &logger = *pModuleManager.GetWindowMain().GetLogger();
	
#if defined( HAS_LIB_DL )
	pLibHandle = dlopen( pLibPath.GetString(), RTLD_NOW );
	if( ! pLibHandle ){
		logger.LogErrorFormat( LOGSOURCE, "dlopen: %s.", dlerror() );
	}
#elif defined( OS_W32 )
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( pLibPath, widePath, MAX_PATH );
	pLibHandle = LoadLibrary( widePath );
	if( ! pLibHandle ){
		int err = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
			( LPTSTR ) &lpMsgBuf, 0, NULL );
		
		// Display the string.
		logger.LogErrorFormat( LOGSOURCE, "LoadLibrary(%i): %s.", err, (char*)lpMsgBuf );
		
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
#endif
	if( ! pLibHandle ){
		pErrorCode = eecLibFileOpenFailed;
		return false;
	}
	
	return true;
}

bool igdeEditorModuleDefinition::pCreateModule( igdeEnvironment *environment ){
	FUNC_CREATEMODULE funcCreateModule;
	
	// look for the entry point function
#if defined( HAS_LIB_DL )
	funcCreateModule = ( FUNC_CREATEMODULE )dlsym( pLibHandle, pEntryPoint.GetString() );
#elif defined( OS_W32 )
	funcCreateModule = ( FUNC_CREATEMODULE )GetProcAddress( pLibHandle, pEntryPoint.GetString() );
#endif
	if( ! funcCreateModule ){
		pErrorCode = eecLibFileEntryPointNotFound;
		return false;
	}
	
	// create module
	pModule = funcCreateModule( environment );
	if( ! pModule ){
		pErrorCode = eecLibFileCreateModuleFailed;
		return false;
	}
	
	// success
	return true;
}

void igdeEditorModuleDefinition::pUnloadLibrary(){
	if( pLibHandle ){
#if defined( HAS_LIB_DL )
		dlclose( pLibHandle );
#elif defined( OS_W32 )
		FreeLibrary( pLibHandle );
#endif
	}
}

void igdeEditorModuleDefinition::pVerify(){
	struct stat fs;
	
	// get file attributes
	if( stat( pLibPath, &fs ) != 0){ // not found
		pErrorCode = eecLibFileNotFound;
		pCanLoad = false;
		return;
	}
	
	// check that this is really a file
	if( ! S_ISREG( fs.st_mode ) ){
		pErrorCode = eecLibFileNotRegularFile;
		pCanLoad = false;
		return;
	}
	
	// check that the file size matches the one specified
	if( fs.st_size != pLibSize ){
		pErrorCode = eecLibFileSizeMismatch;
		pCanLoad = false;
		return;
	}
	
	// check that the sha1 sum is the same
	
	// checks passed
	pCanLoad = true;
}
