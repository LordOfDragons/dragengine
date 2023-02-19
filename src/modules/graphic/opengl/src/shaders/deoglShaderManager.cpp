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
pLanguage( nullptr ),
pPathShaderSources( "/share/shaderSources" ),
pPathShaders( "/share/shaders" )
{
	pLanguage = new deoglShaderLanguage( renderThread );
}

deoglShaderManager::~deoglShaderManager(){
	if( pLanguage ){
		delete pLanguage;
	}
}



// Management
///////////////



// Unit Source Codes
//////////////////////

int deoglShaderManager::GetUnitSourceCodeCount() const{
	return pUnitSourceCodes.GetCount();
}

bool deoglShaderManager::HasUnitSourceCodeWithPath( const char *filePath ) const{
	return pUnitSourceCodes.Has( filePath );
}

deoglShaderUnitSourceCode *deoglShaderManager::GetUnitSourceCodeWithPath( const char *filePath ){
	deObject *o;
	return pUnitSourceCodes.GetAt( filePath, &o ) ? ( deoglShaderUnitSourceCode* )o : nullptr;
}

void deoglShaderManager::AddUnitSourceCode( deoglShaderUnitSourceCode *sourceCode ){
	DEASSERT_NOTNULL( sourceCode )
	DEASSERT_FALSE( pUnitSourceCodes.Has( sourceCode->GetFilePath() ) )
	
	pUnitSourceCodes.SetAt( sourceCode->GetFilePath(), sourceCode );
}

void deoglShaderManager::RemoveAllUnitSourceCodes(){
	pUnitSourceCodes.RemoveAll();
}



void deoglShaderManager::LoadUnitSourceCodes(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pUnitSourceCodes.GetCount();
	
	pLoadUnitSourceCodesIn( "" );
	
	if( ogl.GetConfiguration().GetDoLogInfo() ){
		ogl.LogInfoFormat( "Loaded %d shader unit source codes.", pUnitSourceCodes.GetCount() - oldCount );
	}
}



// Sources
////////////

int deoglShaderManager::GetSourcesCount() const{
	return pSources.GetCount();
}

bool deoglShaderManager::HasSourcesNamed( const char *name ) const{
	return pSources.Has( name );
}

const deoglShaderSources *deoglShaderManager::GetSourcesNamed( const char *name ){
	deObject *o;
	return pSources.GetAt( name, &o ) ? ( const deoglShaderSources * )o : nullptr;
}

void deoglShaderManager::LoadSources(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pSources.GetCount();
	
	pLoadSourcesIn( "" );
	
	if( ogl.GetConfiguration().GetDoLogInfo() ){
		ogl.LogInfoFormat( "Loaded %i shaders.", pSources.GetCount() - oldCount );
	}
}

decObjectList deoglShaderManager::GetSourcesAsList() const{
	return pSources.GetValues();
}



// Programs
/////////////

int deoglShaderManager::GetProgramCount() const{
	return pPrograms.GetCount();
}

const deoglShaderProgram *deoglShaderManager::GetProgramAt( int index ) const{
	return ( deoglShaderProgram* )pPrograms.GetAt( index );
}

bool deoglShaderManager::HasProgramWith( const deoglShaderSources *sources,
const deoglShaderDefines &defines ) const{
	DEASSERT_NOTNULL( sources )
	
	const int count = pPrograms.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglShaderProgram &program = *( deoglShaderProgram* )pPrograms.GetAt( i );
		if( sources == program.GetSources() && defines.Equals( program.GetDefines() ) ){
			return true;
		}
	}
	
	return false;
}

const deoglShaderProgram *deoglShaderManager::GetProgramWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines ){
	DEASSERT_NOTNULL( sources )
	
	const int count = pPrograms.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglShaderProgram *program = ( deoglShaderProgram* )pPrograms.GetAt( i );
		if( sources == program->GetSources() && defines.Equals( program->GetDefines() ) ){
			return program;
		}
	}
	
	const deoglShaderProgram::Ref program( deoglShaderProgram::Ref::New(
		new deoglShaderProgram( pRenderThread, sources, defines ) ) );
	
	if( ! sources->GetPathGeometrySourceCode().IsEmpty() ){
		program->SetGeometrySourceCode( GetUnitSourceCodeWithPath( sources->GetPathGeometrySourceCode() ) );
		
		if( ! program->GetGeometrySourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Geometry source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathGeometrySourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	if( ! sources->GetPathVertexSourceCode().IsEmpty() ){
		program->SetVertexSourceCode( GetUnitSourceCodeWithPath( sources->GetPathVertexSourceCode() ) );
		
		if( ! program->GetVertexSourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Vertex source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathVertexSourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	if( ! sources->GetPathFragmentSourceCode().IsEmpty() ){
		program->SetFragmentSourceCode( GetUnitSourceCodeWithPath( sources->GetPathFragmentSourceCode() ) );
		
		if( ! program->GetFragmentSourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Fragment source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathFragmentSourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	if( ! sources->GetPathComputeSourceCode().IsEmpty() ){
		program->SetComputeSourceCode( GetUnitSourceCodeWithPath( sources->GetPathComputeSourceCode() ) );
		
		if( ! program->GetComputeSourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Compute source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathComputeSourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	if( ! sources->GetPathTessellationControlSourceCode().IsEmpty() ){
		program->SetTessellationControlSourceCode( GetUnitSourceCodeWithPath(
			sources->GetPathTessellationControlSourceCode() ) );
		
		if( ! program->GetTessellationControlSourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation control source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathTessellationControlSourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	if( ! sources->GetPathTessellationEvaluationSourceCode().IsEmpty() ){
		program->SetTessellationEvaluationSourceCode( GetUnitSourceCodeWithPath(
			sources->GetPathTessellationEvaluationSourceCode() ) );
		
		if( ! program->GetTessellationEvaluationSourceCode() ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation evaluation source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathTessellationEvaluationSourceCode().GetString() );
			DETHROW( deeInvalidParam );
		}
	}
	
	pPrograms.Add( program );
	
	if( pLanguage ){
		deoglShaderCompiled *compiled = nullptr;
		
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



// Private Functions
//////////////////////

void deoglShaderManager::pLoadUnitSourceCodesIn( const char *directory ){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	decString filename;
	decPath searchPath;
	decPath basePath;
	int i;
	
	basePath.SetFromUnix( pPathShaderSources.GetString() );
	const int basePathLen = ( int )strlen( basePath.GetPathUnix() ) + 1;
	
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
			
			AddUnitSourceCode( deoglShaderUnitSourceCode::Ref::New( new deoglShaderUnitSourceCode(
				filename, decBaseFileReader::Ref::New( vfs.OpenFileForReading( path ) ) ) ) );
		}
		
	}catch( const deException & ){
		ogl.LogInfoFormat( "Loading shader unit source code %s failed!", filename.GetString() );
		throw;
	}
}

void deoglShaderManager::pLoadSourcesIn( const char *directory ){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deLogger &logger = *ogl.GetGameEngine()->GetLogger();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	decString filename;
	decPath searchPath;
	decPath basePath;
	int i;
	
	// search for shaders in the current directory
	basePath.SetFromUnix( pPathShaders.GetString() );
	const int basePathLen = ( int )strlen( basePath.GetPathUnix() ) + 1;
	
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
			
			const deoglShaderSources::Ref sources( deoglShaderSources::Ref::New(
				new deoglShaderSources( logger, decBaseFileReader::Ref::New(
					vfs.OpenFileForReading( path ) ) ) ) );
			
			if( pSources.Has( sources->GetName() ) ){
				ogl.LogErrorFormat( "Shader file '%s' defines a shader named '%s' but"
					" a shader with this name already exists!",
					sources->GetFilename().GetString(), sources->GetName().GetString() );
				DETHROW( deeInvalidParam );
			}
			
			pSources.SetAt( sources->GetName(), sources );
		}
		
	}catch( const deException & ){
		ogl.LogInfoFormat( "Loading shader %s failed!", filename.GetString() );
		throw;
	}
}
