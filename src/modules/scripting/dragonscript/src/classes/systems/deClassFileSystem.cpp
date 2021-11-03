/* 
 * Drag[en]gine DragonScript Script Module
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

#include "deClassFileSystem.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#ifdef OS_UNIX
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassBlock.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Functions
/////////////////////

// public static func byte getSeparator()
deClassFileSystem::nfGetSeparator::nfGetSeparator( const sInitData &init ) : dsFunction( init.clsFileSys,
"getSeparator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsByte ){
}
void deClassFileSystem::nfGetSeparator::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushByte( '/' );
}

// public static func byte getPathSeparator()
deClassFileSystem::nfGetPathSeparator::nfGetPathSeparator( const sInitData &init ) : dsFunction( init.clsFileSys,
"getPathSeparator", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsByte ){
}
void deClassFileSystem::nfGetPathSeparator::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushByte( ':' );
}

// public static func String getCurrentPrefix()
deClassFileSystem::nfGetCurrentPrefix::nfGetCurrentPrefix( const sInitData &init ) : dsFunction( init.clsFileSys,
"getCurrentPrefix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr ){
}
void deClassFileSystem::nfGetCurrentPrefix::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushString( "/" );
}

// public static func String getGamePath()
deClassFileSystem::nfGetGamePath::nfGetGamePath( const sInitData &init ) : dsFunction( init.clsFileSys,
"getGamePath", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsStr ){
}
void deClassFileSystem::nfGetGamePath::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	
	rt->PushString( clsFileSys.GetDS()->GetGameEngine()->GetDataDir() );
}




// public static func bool existsFile( String filename )
deClassFileSystem::nfExistsFile::nfExistsFile( const sInitData &init ) : dsFunction( init.clsFileSys,
"existsFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsStr ); // filename
}
void deClassFileSystem::nfExistsFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	const char *filename = rt->GetValue( 0 )->GetString();
	decPath path;
	
	path.SetFromUnix( filename );
	
	rt->PushBool( vfs.ExistsFile( path ) );
}

// public static func bool canReadFile( String path )
deClassFileSystem::nfCanReadFile::nfCanReadFile( const sInitData &init ) : dsFunction( init.clsFileSys,
"canReadFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfCanReadFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	
	rt->PushBool( vfs.CanReadFile( path ) );
}

// public static func bool canWriteFile( String path )
deClassFileSystem::nfCanWriteFile::nfCanWriteFile( const sInitData &init ) : dsFunction( init.clsFileSys,
"canWriteFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfCanWriteFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	
	rt->PushBool( vfs.CanWriteFile( path ) );
}

// public static func bool canDeleteFile( String path )
deClassFileSystem::nfCanDeleteFile::nfCanDeleteFile( const sInitData &init ) : dsFunction( init.clsFileSys,
"canDeleteFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfCanDeleteFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	
	rt->PushBool( vfs.CanDeleteFile( path ) );
}

// public static func void deleteFile( String path )
deClassFileSystem::nfDeleteFile::nfDeleteFile( const sInitData &init ) : dsFunction( init.clsFileSys,
"deleteFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfDeleteFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	
	vfs.DeleteFile( path );
}

class deClassFileSystemSearch : public deFileSearchVisitor{
private:
	deClassFileSystem &pClassFileSystem;
	dsRunTime &pRT;
	const bool pRecursive;
	dsValue * const pBlock;
	const int pFuncIndexRun;
	dsClass *pClassBool;
	
public:
	deClassFileSystemSearch( deClassFileSystem &classFileSystem, dsRunTime &rt, bool recursive, dsValue *block ) :
	pClassFileSystem( classFileSystem ), pRT( rt ), pRecursive( recursive ), pBlock( block ),
	pFuncIndexRun( ( ( dsClassBlock* )rt.GetEngine()->GetClassBlock() )->GetFuncIndexRun2() ),
	pClassBool( rt.GetEngine()->GetClassBool() )
	{
		classFileSystem.PrepareTypes();
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		return Run( path, pClassFileSystem.GetTypeFile() );
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		if( ! Run( path, pClassFileSystem.GetTypeDirectory() ) ){
			return false;
		}
		if( pRecursive ){
			vfs.SearchFiles( path, *this );
		}
		return true;
	}
	
	virtual bool VisitSpecial( const deVirtualFileSystem &vfs, const decPath &path ){
		return Run( path, pClassFileSystem.GetTypeSpecial() );
	}
	
	bool Run( const decPath &path, dsValue *valueType ){
		pRT.PushValue( valueType );
		pRT.PushString( path.GetPathUnix() );
		pRT.RunFunctionFast( pBlock, pFuncIndexRun ); // bool run(path, type)
		
		if( pRT.GetReturnValue()->GetType() != pClassBool ){
			DSTHROW_INFO( dseInvalidCast, ErrorCastInfo( pRT.GetReturnValue(), pClassBool ) );
		}
		
		return pRT.GetReturnBool();
	}
};

// public static func void searchFiles( String directory, bool recursive, Block ablock )
deClassFileSystem::nfSearchFiles::nfSearchFiles( const sInitData &init ) : dsFunction( init.clsFileSys,
"searchFiles", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // directory
	p_AddParameter( init.clsBool ); // recursive
	p_AddParameter( init.clsBlock ); // ablock
}
void deClassFileSystem::nfSearchFiles::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	const char * const directory = rt->GetValue( 0 )->GetString();
	const bool recursive = rt->GetValue( 1 )->GetBool();
	dsValue * const block = rt->GetValue( 2 );
	
	if( ! block->GetRealObject() ){
		DSTHROW_INFO( dueNullPointer, "block" );
	}
	
	deClassFileSystemSearch visitor( clsFileSys, *rt, recursive, block );
	vfs.SearchFiles( decPath::CreatePathUnix( directory ), visitor );
}

// public static func FileType getTypeOfFile( String path )
deClassFileSystem::nfGetFileType::nfGetFileType( const sInitData &init ) : dsFunction( init.clsFileSys,
"getTypeOfFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFileType ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfGetFileType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	decPath path;
	
	path.SetFromUnix( rt->GetValue( 0 )->GetString() );
	
	rt->PushValue( ( ( deClassFileSystem* )GetOwnerClass() )->GetClassFileType()
		->GetVariable( vfs.GetFileType( path ) )->GetStaticValue() );
}

// public static func bool pathMatchesPattern( String path, String pattern )
deClassFileSystem::nfPathMatchesPattern::nfPathMatchesPattern( const sInitData &init ) : dsFunction( init.clsFileSys,
"pathMatchesPattern", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsStr ); // path
	p_AddParameter( init.clsStr ); // pattern
}
void deClassFileSystem::nfPathMatchesPattern::RunFunction( dsRunTime *rt, dsValue *myself ){
	const char *path = rt->GetValue( 0 )->GetString();
	const char *pattern = rt->GetValue( 1 )->GetString();
	
	rt->PushBool( decString::StringMatchesPattern( path, pattern ) );
}

// public static func void browseOverlay(String path)
deClassFileSystem::nfBrowseOverlay::nfBrowseOverlay( const sInitData &init ) :
dsFunction( init.clsFileSys, "browseOverlay", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfBrowseOverlay::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	const char * const path = rt->GetValue( 0 )->GetString();
	
	// ensure directory exists
	decPath ensurePath;
	ensurePath.SetFromUnix( path );
	ensurePath.AddComponent( "__ds_overlay_delete_me__" );
	
	if( ! vfs.ExistsFile( ensurePath ) && vfs.CanWriteFile( ensurePath ) ){
		decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( ensurePath ) );
		vfs.DeleteFile( ensurePath );
	}
	
	// browse directory
	decPath realPath;
	realPath.SetFromNative( clsFileSys.GetDS()->GetGameEngine()->GetPathOverlay() );
	realPath.AddUnixPath( path );
	clsFileSys.BrowseNativeDirectory( realPath );
}

// public static func void browseCapture(String path)
deClassFileSystem::nfBrowseCapture::nfBrowseCapture( const sInitData &init ) :
dsFunction( init.clsFileSys, "browseCapture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfBrowseCapture::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	const char * const path = rt->GetValue( 0 )->GetString();
	
	decPath tempPath;
	tempPath.SetFromUnix( path );
	if( tempPath.IsAbsolute() ){
		if( tempPath.GetComponentCount() == 0 || tempPath.GetComponentAt( 0 ) != "capture" ){
			DSTHROW_INFO( dueInvalidParam, "absolute path has to start with '/capture'" );
		}
		tempPath.RemoveComponentFrom( 0 );
	}
	
	// ensure directory exists
	decPath ensurePath;
	ensurePath.SetFromUnix( "/capture" );
	ensurePath.Add( tempPath );
	ensurePath.AddComponent( "__ds_capture_delete_me__" );
	
	if( ! vfs.ExistsFile( ensurePath ) && vfs.CanWriteFile( ensurePath ) ){
		decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( ensurePath ) );
		vfs.DeleteFile( ensurePath );
	}
	
	// browse directory
	decPath realPath;
	realPath.SetFromNative( clsFileSys.GetDS()->GetGameEngine()->GetPathCapture() );
	realPath.Add( tempPath );
	clsFileSys.BrowseNativeDirectory( realPath );
}

// public static func void browseConfig(String path)
deClassFileSystem::nfBrowseConfig::nfBrowseConfig( const sInitData &init ) :
dsFunction( init.clsFileSys, "browseConfig", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr ); // path
}
void deClassFileSystem::nfBrowseConfig::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	deVirtualFileSystem &vfs = *clsFileSys.GetDS()->GetGameEngine()->GetVirtualFileSystem();
	const char * const path = rt->GetValue( 0 )->GetString();
	
	decPath tempPath;
	tempPath.SetFromUnix( path );
	if( tempPath.IsAbsolute() ){
		if( tempPath.GetComponentCount() == 0 || tempPath.GetComponentAt( 0 ) != "config" ){
			DSTHROW_INFO( dueInvalidParam, "absolute path has to start with '/config'" );
		}
		tempPath.RemoveComponentFrom( 0 );
	}
	
	// ensure directory exists
	decPath ensurePath;
	ensurePath.SetFromUnix( "/config" );
	ensurePath.Add( tempPath );
	ensurePath.AddComponent( "__ds_config_delete_me__" );
	
	if( ! vfs.ExistsFile( ensurePath ) && vfs.CanWriteFile( ensurePath ) ){
		decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( ensurePath ) );
		vfs.DeleteFile( ensurePath );
	}
	
	// browse directory
	decPath realPath;
	realPath.SetFromNative( clsFileSys.GetDS()->GetGameEngine()->GetPathConfig() );
	realPath.Add( tempPath );
	clsFileSys.BrowseNativeDirectory( realPath );
}

// public static func Array getFileExtensions(ResourceLoaderType type)
deClassFileSystem::nfGetFileExtensions::nfGetFileExtensions( const sInitData &init ) :
dsFunction( init.clsFileSys, "getFileExtensions", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsArray ){
	p_AddParameter( init.clsResourceLoaderType ); // type
}
void deClassFileSystem::nfGetFileExtensions::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deClassFileSystem &clsFileSys = *( ( deClassFileSystem* )GetOwnerClass() );
	
	// determine module type to enumerate
	const deResourceLoader::eResourceType type = ( deResourceLoader::eResourceType )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	deModuleSystem::eModuleTypes moduleType = deModuleSystem::emtUnknown;
	
	switch( type ){
	case deResourceLoader::ertAnimation:
		moduleType = deModuleSystem::emtAnimation;
		break;
		
	case deResourceLoader::ertFont:
		moduleType = deModuleSystem::emtFont;
		break;
		
	case deResourceLoader::ertImage:
		moduleType = deModuleSystem::emtImage;
		break;
		
	case deResourceLoader::ertLanguagePack:
		moduleType = deModuleSystem::emtLanguagePack;
		break;
		
	case deResourceLoader::ertModel:
		moduleType = deModuleSystem::emtModel;
		break;
		
	case deResourceLoader::ertOcclusionMesh:
		moduleType = deModuleSystem::emtOcclusionMesh;
		break;
		
	case deResourceLoader::ertRig:
		moduleType = deModuleSystem::emtRig;
		break;
		
	case deResourceLoader::ertSkin:
		moduleType = deModuleSystem::emtSkin;
		break;
		
	case deResourceLoader::ertSound:
		moduleType = deModuleSystem::emtSound;
		break;
		
	case deResourceLoader::ertVideo:
		moduleType = deModuleSystem::emtVideo;
		break;
	}
	
	if( moduleType == deModuleSystem::emtUnknown ){
		DSTHROW( dueInvalidParam );
	}
	
	// enumerate modules
	const deModuleSystem &modsys = *clsFileSys.GetDS()->GetGameEngine()->GetModuleSystem();
	const int moduleCount = modsys.GetModuleCount();
	decPointerList modules;
	int i, j;
	
	for( i=0; i<moduleCount; i++ ){
		const deLoadableModule * const module = modsys.GetModuleAt( i );
		if( module->GetType() != moduleType || ! module->GetEnabled()
		|| module->GetErrorCode() != deLoadableModule::eecSuccess ){
			continue;
		}
		
		const decString &name = module->GetName();
		const int foundCount = modules.GetCount();
		for( j=0; j<foundCount; j++ ){
			const deLoadableModule * const module2 = ( const deLoadableModule * )modules.GetAt( j );
			if( module2->GetName() == name ){
				if( modsys.CompareVersion( module->GetVersion(), module2->GetVersion() ) > 0 ){
					modules.SetAt( j, ( void* )module );
				}
				break;
			}
		}
		
		if( j == foundCount ){
			modules.Add( ( void* )module );
		}
	}
	
	// build array
	const dsEngine &sengine = *clsFileSys.GetDS()->GetScriptEngine();
	dsValue *valueResult = rt->CreateValue( sengine.GetClassArray() );
	dsValue *valueExtension = rt->CreateValue( clsFileSys.GetClassFileExtension() );
	dsValue *valuePatterns = rt->CreateValue( sengine.GetClassArray() );
	
	try{
		// create array
		rt->CreateObject( valueResult, sengine.GetClassArray(), 0 );
		
		// iterate
		const int foundCount = modules.GetCount();
		for( i=0; i<foundCount; i++ ){
			const deLoadableModule &module = *( const deLoadableModule * )modules.GetAt( i );
			
			// create pattern array
			rt->CreateObject( valuePatterns, sengine.GetClassArray(), 0 );
			
			const decStringList &patterns = module.GetPatternList();
			const int patternCount = patterns.GetCount();
			for( j=0; j<patternCount; j++ ){
				rt->PushString( patterns.GetAt( j ) );
				rt->RunFunction( valuePatterns, "add", 1 );
			}
			
			// create file extension
			rt->PushString( module.GetDefaultExtension() );
			rt->PushValue( valuePatterns );
			rt->PushString( module.GetName() );
			rt->CreateObject( valueExtension, clsFileSys.GetClassFileExtension(), 3 );
			
			// add to list
			rt->PushValue( valueExtension );
			rt->RunFunction( valueResult, "add", 1 );
		}
		
		// push list as return value
		rt->PushValue( valueResult );
		
		// clean up
		rt->FreeValue( valuePatterns );
		valuePatterns = nullptr;
		
		rt->FreeValue( valueResult );
		valueResult = nullptr;
		
		rt->FreeValue( valueExtension );
		
	}catch( ... ){
		if( valuePatterns ){
			rt->FreeValue( valuePatterns );
		}
		if( valueExtension ){
			rt->FreeValue( valueExtension );
		}
		if( valueResult ){
			rt->FreeValue( valueResult );
		}
		throw;
	}
}



// Class deClassFileSystem
////////////////////////////

// Constructor
////////////////

deClassFileSystem::deClassFileSystem( deScriptingDragonScript *ds ) :
dsClass( "FileSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ),
pTypeFile( NULL ),
pTypeDirectory( NULL ),
pTypeSpecial( NULL ),
pTypesReady( false )
{
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( 0 );
}

deClassFileSystem::~deClassFileSystem(){
}



// Management
///////////////

void deClassFileSystem::CreateClassMembers( dsEngine *engine ){
	pClsFileType = engine->GetClass( "Dragengine.FileType" );
	pClsFileExtension = engine->GetClass( "Dragengine.FileExtension" );
	pClsResourceLoaderType = engine->GetClass( "Dragengine.ResourceLoaderType" );
	
	sInitData init;
	init.clsFileSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsByte = engine->GetClassByte();
	init.clsBool = engine->GetClassBool();
	init.clsInt = engine->GetClassInt();
	init.clsStr = engine->GetClassString();
	init.clsBlock = engine->GetClassBlock();
	init.clsFileType = pClsFileType;
	init.clsFileExtension = pClsFileExtension;
	init.clsResourceLoaderType = pClsResourceLoaderType;
	init.clsArray = engine->GetClassArray();
	
	AddFunction( new nfGetSeparator( init ) );
	AddFunction( new nfGetPathSeparator( init ) );
	AddFunction( new nfGetCurrentPrefix( init ) );
	AddFunction( new nfGetGamePath( init ) );
	
	AddFunction( new nfExistsFile( init ) );
	AddFunction( new nfCanReadFile( init ) );
	AddFunction( new nfCanWriteFile( init ) );
	AddFunction( new nfCanDeleteFile( init ) );
	AddFunction( new nfDeleteFile( init ) );
	AddFunction( new nfSearchFiles( init ) );
	AddFunction( new nfGetFileType( init ) );
	AddFunction( new nfPathMatchesPattern( init ) );
	AddFunction( new nfBrowseOverlay( init ) );
	AddFunction( new nfBrowseCapture( init ) );
	AddFunction( new nfBrowseConfig( init ) );
	AddFunction( new nfGetFileExtensions( init ) );
	
	CalcMemberOffsets();
}

void deClassFileSystem::PrepareTypes(){
	if( pTypesReady ){
		return;
	}
	
	pTypeFile = pClsFileType->GetVariable( deVFSContainer::eftRegularFile )->GetStaticValue();
	pTypeDirectory = pClsFileType->GetVariable( deVFSContainer::eftDirectory )->GetStaticValue();
	pTypeSpecial = pClsFileType->GetVariable( deVFSContainer::eftSpecial )->GetStaticValue();
	pTypesReady = true;
}

void deClassFileSystem::BrowseNativeDirectory( const decPath& path ) const{
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( path.GetPathNative(), widePath, MAX_PATH );
	ShellExecute( NULL, L"open", widePath, NULL, NULL, SW_SHOWDEFAULT );
	
	#else
	const char * const appname = "xdg-open";
	
	if( fork() == 0 ){
		// GetString() is required otherwise execlp fails to run correctly
		execlp( appname, appname, path.GetPathUnix().GetString(), nullptr );
		pDS->LogErrorFormat( "Failed running '%s' (error %d)\n", appname, errno );
		exit( 0 );
	}
	#endif
}
