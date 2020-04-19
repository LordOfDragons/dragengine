/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglShaderManager.h"
#include "deoglShaderProgram.h"
#include "deoglShaderDefines.h"
#include "deoglShaderSources.h"
#include "deoglShaderCompiled.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderUnitSourceCode.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>



// Class deoglShaderManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderManager::deoglShaderManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pLanguage( NULL ),

pUnitSourceCodes( NULL ),
pUnitSourceCodeCount( 0 ),
pUnitSourceCodeSize( 0 ),

pSources( NULL ),
pSourcesCount( 0 ),
pSourcesSize( 0 ),

pPrograms( NULL ),
pProgramCount( 0 ),
pProgramSize( 0 ),

pPathShaderSources( "/share/shaderSources" ),
pPathShaders( "/share/shaders" )
{
	pLanguage = new deoglShaderLanguage( renderThread );
}

deoglShaderManager::~deoglShaderManager(){
	int i;
	
	for( i=0; i<pProgramCount; i++ ){
		if( pPrograms[ i ]->GetUsageCount() != 0 ){
			pRenderThread.GetOgl().LogWarnFormat( "ShaderManager CleanUp: Program '%s' with usage count %i",
				pPrograms[ i ]->GetSources()->GetName().GetString(), pPrograms[ i ]->GetUsageCount() );
		}
	}
	
	RemoveAllPrograms();
	if( pPrograms ){
		delete [] pPrograms;
	}
	
	RemoveAllSources();
	if( pSources ){
		delete [] pSources;
	}
	
	RemoveAllUnitSourceCodes();
	if( pUnitSourceCodes ){
		delete [] pUnitSourceCodes;
	}
	
	if( pLanguage ){
		delete pLanguage;
	}
}



// Management
///////////////



// Unit Source Codes
//////////////////////

deoglShaderUnitSourceCode *deoglShaderManager::GetUnitSourceCodeAt( int index ) const{
	if( index < 0 || index >= pUnitSourceCodeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pUnitSourceCodes[ index ];
}

bool deoglShaderManager::HasUnitSourceCodeWithPath( const char *filePath ) const{
	if( ! filePath ){
		DETHROW( deeInvalidParam );
	}
	
	int s;
	
	for( s=0; s<pUnitSourceCodeCount; s++ ){
		if( strcmp( filePath, pUnitSourceCodes[ s ]->GetFilePath() ) == 0 ){
			return true;
		}
	}
	
	return false;
}

deoglShaderUnitSourceCode *deoglShaderManager::GetUnitSourceCodeWithPath( const char *filePath ){
	if( ! filePath ){
		DETHROW( deeInvalidParam );
	}
	
	int s;
	
	for( s=0; s<pUnitSourceCodeCount; s++ ){
		if( strcmp( filePath, pUnitSourceCodes[ s ]->GetFilePath() ) == 0 ){
			return pUnitSourceCodes[ s ];
		}
	}
	
	return NULL;
}

void deoglShaderManager::AddUnitSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	if( ! sourceCode || HasUnitSourceCodeWithPath( sourceCode->GetFilePath() ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUnitSourceCodeCount == pUnitSourceCodeSize ){
		int newSize = pUnitSourceCodeSize * 3 / 2 + 1;
		deoglShaderUnitSourceCode **newArray = new deoglShaderUnitSourceCode*[ newSize ];
		
		if( pUnitSourceCodes ){
			memcpy( newArray, pUnitSourceCodes, sizeof( deoglShaderUnitSourceCode* ) * pUnitSourceCodeSize );
			delete [] pUnitSourceCodes;
		}
		pUnitSourceCodes = newArray;
		pUnitSourceCodeSize = newSize;
	}
	
	pUnitSourceCodes[ pUnitSourceCodeCount++ ] = sourceCode;
}

void deoglShaderManager::RemoveAllUnitSourceCodes(){
	while( pUnitSourceCodeCount > 0 ){
		pUnitSourceCodeCount--;
		delete pUnitSourceCodes[ pUnitSourceCodeCount ];
	}
}



void deoglShaderManager::LoadUnitSourceCodes(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pUnitSourceCodeCount;
	
	pLoadUnitSourceCodesIn( "" );
	
	if( ogl.GetConfiguration().GetDoLogInfo() ){
		ogl.LogInfoFormat( "Loaded %i shader unit source codes.", pUnitSourceCodeCount - oldCount );
	}
}



// Sources
////////////

deoglShaderSources *deoglShaderManager::GetSourcesAt( int index ) const{
	if( index < 0 || index >= pSourcesCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pSources[ index ];
}

bool deoglShaderManager::HasSourcesNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pSourcesCount; i++ ){
		if( pSources[ i ]->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

deoglShaderSources *deoglShaderManager::GetSourcesNamed( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pSourcesCount; i++ ){
		if( pSources[ i ]->GetName() == name ){
			return pSources[ i ];
		}
	}
	
	return NULL;
}

void deoglShaderManager::AddSources( deoglShaderSources *sources ){
	if( ! sources || HasSourcesNamed( sources->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSourcesCount == pSourcesSize ){
		int newSize = pSourcesSize * 3 / 2 + 1;
		deoglShaderSources **newArray = new deoglShaderSources*[ newSize ];
		
		if( pSources ){
			memcpy( newArray, pSources, sizeof( deoglShaderSources* ) * pSourcesSize );
			delete [] pSources;
		}
		
		pSources = newArray;
		pSourcesSize = newSize;
	}
	
	pSources[ pSourcesCount++ ] = sources;
}

void deoglShaderManager::RemoveAllSources(){
	while( pSourcesCount > 0 ){
		pSourcesCount--;
		delete pSources[ pSourcesCount ];
	}
}



void deoglShaderManager::LoadSources(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pSourcesCount;
	
	pLoadSourcesIn( "" );
	
	if( ogl.GetConfiguration().GetDoLogInfo() ){
		ogl.LogInfoFormat( "Loaded %i shaders.", pSourcesCount - oldCount );
	}
}



// Programs
/////////////

deoglShaderProgram *deoglShaderManager::GetProgramAt( int index ) const{
	if( index < 0 || index >= pProgramCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPrograms[ index ];
}
/*
bool deoglShaderManager::HasProgramWith( deoglShaderUnitProgram *fragment, deoglShaderUnitProgram *vertex,
deoglShaderUnitProgram *geometry, const deoglShaderDefines &defines ) const{
	if( ! fragment || ! vertex ) DETHROW( deeInvalidParam );
	
	int p;
	
	for( p=0; p<pProgramCount; p++ ){
		if( pPrograms[ p ]->Matches( fragment, vertex, geometry, defines ) ){
			return true;
		}
	}
	
	return false;
}

deoglShaderProgram *deoglShaderManager::GetProgramWith( deoglShaderUnitProgram *fragment, deoglShaderUnitProgram *vertex,
deoglShaderUnitProgram *geometry, const deoglShaderDefines &defines ){
	if( ! fragment || ! vertex ) DETHROW( deeInvalidParam );
	
	int p;
	
	for( p=0; p<pProgramCount; p++ ){
		if( pPrograms[ p ]->Matches( fragment, vertex, geometry, defines ) ){
			return pPrograms[ p ];
		}
	}
	
	deoglShaderProgram *program = NULL;
	
	try{
		program = new deoglShaderProgram( fragment, vertex, geometry, defines );
		if( ! program ) DETHROW( deeOutOfMemory );
		
		AddProgram( program );
		
	}catch( const deException & ){
		if( program ) delete program;
		throw;
	}
	
	if( pLanguage ){
		deoglShaderCompiled *compiled = NULL;
		try{
			compiled = pLanguage->CompileShader( sources, defines );
			if( ! compiled ) DETHROW( deeOutOfMemory );
			
			program->SetCompiled( compiled );
			
		}catch( const deException & ){
			if( compiled ) delete compiled;
			throw;
		}
	}
	
	return program;
}

deoglShaderProgram *deoglShaderManager::GetProgramWith( const char *pathFragment, const char *pathVertex, const char *pathGeometry, const deoglShaderDefines &defines ){
	if( ! pathFragment || ! pathVertex ) DETHROW( deeInvalidParam );
	
	deoglShaderUnitProgram *fragment = NULL;
	deoglShaderUnitProgram *vertex = NULL;
	deoglShaderUnitProgram *geometry = NULL;
	
	fragment = GetUnitProgramWith( GetUnitSourceCodeWithPath( pathFragment ), defines );
	vertex = GetUnitProgramWith( GetUnitSourceCodeWithPath( pathVertex ), defines );
	if( pathGeometry ){
	deoglShaderUnitProgram *geometry = NULL;
	
	
	int p;
	
	for( p=0; p<pProgramCount; p++ ){
		if( pPrograms[ p ]->Matches( fragment, vertex, geometry ) ){
			return pPrograms[ p ];
		}
	}
	
	deoglShaderProgram *program = NULL;
	
	try{
		program = new deoglShaderProgram( fragment, vertex, geometry );
		if( ! program ) DETHROW( deeOutOfMemory );
		
		AddProgram( program );
		
	}catch( const deException & ){
		if( program ) delete program;
		throw;
	}
	
	return program;
}
*/

bool deoglShaderManager::HasProgramWith( deoglShaderSources *sources, const deoglShaderDefines &defines ) const{
	if( ! sources ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pProgramCount; i++ ){
		if( sources == pPrograms[ i ]->GetSources() && defines.Equals( pPrograms[ i ]->GetDefines() ) ){
			return true;
		}
	}
	
	return false;
}

deoglShaderProgram *deoglShaderManager::GetProgramWith( deoglShaderSources *sources, const deoglShaderDefines &defines ){
	if( ! sources ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pProgramCount; i++ ){
		if( sources == pPrograms[ i ]->GetSources() && defines.Equals( pPrograms[ i ]->GetDefines() ) ){
			if( pPrograms[ i ]->GetUsageCount() < 0 ){
				pRenderThread.GetLogger().LogWarnFormat( "ShaderManager.GetProgramWith(): Program '%s' has usage count %i!",
					pPrograms[ i ]->GetSources()->GetName().GetString(), pPrograms[ i ]->GetUsageCount() );
				
				while( pPrograms[ i ]->GetUsageCount() < 0 ){
					pPrograms[ i ]->AddUsage();
				}
			}
			pPrograms[ i ]->AddUsage();
			return pPrograms[ i ];
		}
	}
	
	deoglShaderProgram *program = NULL;
	
	try{
		program = new deoglShaderProgram( sources, defines );
		
		if( ! sources->GetPathGeometrySourceCode().IsEmpty() ){
			program->SetGeometrySourceCode( GetUnitSourceCodeWithPath( sources->GetPathGeometrySourceCode().GetString() ) );
			
			if( ! program->GetGeometrySourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Shader(%s): Geometry source '%s' not found", sources->GetFilename().GetString(),
					sources->GetPathGeometrySourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		if( ! sources->GetPathVertexSourceCode().IsEmpty() ){
			program->SetVertexSourceCode( GetUnitSourceCodeWithPath( sources->GetPathVertexSourceCode().GetString() ) );
			
			if( ! program->GetVertexSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Shader(%s): Vertex source '%s' not found", sources->GetFilename().GetString(),
					sources->GetPathVertexSourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		if( ! sources->GetPathFragmentSourceCode().IsEmpty() ){
			program->SetFragmentSourceCode( GetUnitSourceCodeWithPath( sources->GetPathFragmentSourceCode().GetString() ) );
			
			if( ! program->GetFragmentSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Shader(%s): Fragment source '%s' not found", sources->GetFilename().GetString(),
					sources->GetPathFragmentSourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		AddProgram( program );
		
	}catch( const deException & ){
		if( program ){
			delete program;
		}
		throw;
	}
	
	if( pLanguage ){
		deoglShaderCompiled *compiled = NULL;
		
		try{
			compiled = pLanguage->CompileShader( *program );
			program->SetCompiled( compiled );
			
		}catch( const deException & ){
			if( compiled ){
				delete compiled;
			}
			throw;
		}
	}
	
	return program;
}

void deoglShaderManager::AddProgram( deoglShaderProgram *program ){
	if( ! program ){
		DETHROW( deeInvalidParam );
	}
	
	if( pProgramCount == pProgramSize ){
		int newSize = pProgramCount * 3 / 2 + 1;
		deoglShaderProgram **newArray = new deoglShaderProgram*[ newSize ];
		
		if( pPrograms ){
			memcpy( newArray, pPrograms, sizeof( deoglShaderProgram* ) * pProgramSize );
			delete [] pPrograms;
		}
		
		pPrograms = newArray;
		pProgramSize = newSize;
	}
	
	pPrograms[ pProgramCount ] = program;
	pProgramCount++;
}

void deoglShaderManager::RemoveAllPrograms(){
	while( pProgramCount > 0 ){
		pProgramCount--;
		delete pPrograms[ pProgramCount ];
	}
}



// Private Functions
//////////////////////

void deoglShaderManager::pLoadUnitSourceCodesIn( const char *directory ){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	deoglShaderUnitSourceCode *sourceCode = NULL;
	decBaseFileReaderReference reader;
	decString filename;
	decPath searchPath;
	decPath basePath;
	int i;
	
	basePath.SetFromUnix( pPathShaderSources.GetString() );
	const int basePathLen = strlen( basePath.GetPathUnix() ) + 1;
	
	try{
		deCollectFileSearchVisitor collect( "*.glsl", true );
		searchPath.SetFrom( basePath );
		searchPath.AddUnixPath( directory );
		vfs.SearchFiles( searchPath, collect );
		
		const dePathList &pathList = collect.GetFiles();
		const int count = pathList.GetCount();
		for( i=0; i<count; i++ ){
			const decPath &path = pathList.GetAt( i );
			filename = path.GetPathUnix().GetMiddle( basePathLen );
			/*if( ogl.GetConfiguration()->GetDoLogDebug() ){
				ogl.LogInfoFormat( "Loading shader unit source code %s...", filename );
			}*/
			
			reader.TakeOver( vfs.OpenFileForReading( path ) );
			sourceCode = new deoglShaderUnitSourceCode( filename, reader );
			AddUnitSourceCode( sourceCode );
			sourceCode = NULL;
		}
		
	}catch( const deException & ){
		if( sourceCode ){
			delete sourceCode;
		}
		ogl.LogInfoFormat( "Loading shader unit source code %s failed!", filename.GetString() );
		throw;
	}
}

void deoglShaderManager::pLoadSourcesIn( const char *directory ){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deLogger &logger = *ogl.GetGameEngine()->GetLogger();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	deoglShaderSources *sources = NULL;
	decBaseFileReaderReference reader;
	decString filename;
	decPath searchPath;
	decPath basePath;
	int i;
	
	// search for shaders in the current directory
	basePath.SetFromUnix( pPathShaders.GetString() );
	const int basePathLen = strlen( basePath.GetPathUnix() ) + 1;
	
	try{
		deCollectFileSearchVisitor collect( "*.shader.xml", true );
		searchPath.SetFrom( basePath );
		searchPath.AddUnixPath( directory );
		vfs.SearchFiles( searchPath, collect );
		
		const dePathList &pathList = collect.GetFiles();
		const int count = pathList.GetCount();
		for( i=0; i<count; i++ ){
			const decPath &path = pathList.GetAt( i );
			filename = path.GetPathUnix().GetMiddle( basePathLen );
			/*if( ogl.GetConfiguration()->GetDoLogDebug() ){
				ogl.LogInfoFormat( "Loading shader %s...", filename );
			}*/
			
			reader.TakeOver( vfs.OpenFileForReading( path ) );
			
			sources = new deoglShaderSources( logger, reader );
			
			if( HasSourcesNamed( sources->GetName().GetString() ) ){
				ogl.LogErrorFormat( "Shader file '%s' defines a shader named '%s' but"
					" a shader with this name already exists!",
					sources->GetFilename().GetString(), sources->GetName().GetString() );
				DETHROW( deeInvalidParam );
			}
			
			AddSources( sources );
			sources = NULL;
		}
		
	}catch( const deException & ){
		ogl.LogInfoFormat( "Loading shader %s failed!", filename.GetString() );
		if( sources ){
			delete sources;
		}
		throw;
	}
}
