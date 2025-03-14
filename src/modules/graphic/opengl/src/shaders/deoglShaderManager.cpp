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
#include <string.h>
#include <inttypes.h>

#include "deoglShaderManager.h"
#include "deoglShaderDefines.h"
#include "deoglShaderSources.h"
#include "deoglShaderCompiled.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderUnitSourceCode.h"
#include "compiler/deoglShaderCompileListener.h"
#include "../deGraphicOpenGl.h"
#include "../deoglCaches.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/threading/deMutexGuard.h>



// Definitions
////////////////

// cache revision. if anything except *.glsl or *.shader.xml changes increment this
// value to make sure existing caches are invalidate
#define SHADER_CACHE_REVISION 1

// MinGW bug workaround
#if defined OS_W32 && defined __MINGW64__
#undef PRIu64
#define PRIu64 "I64u"
#endif


// Class deoglShaderManager::cCompilingShader
///////////////////////////////////////////////

deoglShaderManager::cCompileProgram::cCompileProgram(deoglShaderManager &manager,
	const deoglShaderProgram::Ref &program) :
pManager(manager),
pProgram(program)
{
	DEASSERT_NOTNULL(program)
}

void deoglShaderManager::cCompileProgram::AddListener(cGetProgramListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void deoglShaderManager::cCompileProgram::CompileFinished(deoglShaderCompiled *compiled){
	{
	const deMutexGuard guard(pManager.pMutexCompilePrograms);
	const int index = pManager.pCompilePrograms.IndexOf(this);
	if(index != -1){
		pManager.pCompilePrograms.RemoveFrom(index);
	}
	
	deoglShaderProgram *program = pManager.pGetProgramWith(
		pProgram->GetSources(), pProgram->GetDefines());
	if(!program && compiled){
		program = pProgram;
		pManager.pPrograms.Add(pProgram);
		program->SetCompiled(compiled);
	}
	pProgram = nullptr;
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		cGetProgramListener * const listener = (cGetProgramListener*)pListeners.GetAt(i);
		try{
			listener->GetProgramFinished(program);
			
		}catch(const deException &e){
			pManager.pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
	}
	pListeners.RemoveAll();
	}
	
	pManager.pSemaphoreCompileFinished.SignalAll();
}


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

void deoglShaderManager::ValidateCaches(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deCacheHelper &cache = ogl.GetCaches().GetShaders();
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	// validation string composes of the path and modification times of all shader sources (*.glsl)
	// and shaders (*.shader.xml) in the order they have been loaded. we could also sort the list
	// but running on the same machine with the same operating system and the same file system
	// usually returns identical file listings. in the worst case the cache is invalidate when
	// it could be kept valid
	const decString validationString( pCacheValidationString.Join( "\n" ) );
	
	// if the validation string differs from the cached validation string drop all
	// cached shaders
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	const decPath pathValidationString( decPath::CreatePathUnix( "/cache/global/shaderValidation" ) );
	
	if( vfs.ExistsFile( pathValidationString ) ){
		decString oldValidationString;
		try{
			const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
					vfs.OpenFileForReading( pathValidationString ) ) );
			const int size = reader->GetLength();
			oldValidationString.Set( ' ', size );
			reader->Read( ( void* )oldValidationString.GetString(), size );
			
			if( validationString == oldValidationString ){
				logger.LogInfo( "ShaderManager Cache: Validated" );
				
			}else{
				logger.LogInfo( "ShaderManager Cache: Validation failed. Invalidate caches" );
				cache.DeleteAll();
			}
			
		}catch( const deException & ){
			logger.LogInfo( "ShaderManager Cache: Validation file failed reading. Invalidate caches" );
			cache.DeleteAll();
		}
		
	}else{
		logger.LogInfo( "ShaderManager Cache: Validation file missing. Invalidate caches" );
		cache.DeleteAll();
	}
	
	// write validation string
	try{
		decBaseFileWriter::Ref::New( vfs.OpenFileForWriting( pathValidationString ) )
			->WriteString( validationString );
		
	}catch( const deException & ){
		logger.LogInfo( "ShaderManager Cache: Writing validation failed" );
	}
}



// Unit Source Codes
//////////////////////

int deoglShaderManager::GetUnitSourceCodeCount() const{
	return pUnitSourceCodes.GetCount();
}

bool deoglShaderManager::HasUnitSourceCodeWithPath( const char *filePath ) const{
	return pUnitSourceCodes.Has( filePath );
}

deoglShaderUnitSourceCode *deoglShaderManager::GetUnitSourceCodeWithPath( const char *filePath ) const{
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
		pRenderThread.GetLogger().LogInfoFormat( "Loaded %d shader unit source codes.", 
			pUnitSourceCodes.GetCount() - oldCount );
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
		pRenderThread.GetLogger().LogInfoFormat( "Loaded %i shaders.",
			pSources.GetCount() - oldCount );
	}
}

decObjectList deoglShaderManager::GetSourcesAsList() const{
	return pSources.GetValues();
}



// Programs
/////////////

int deoglShaderManager::GetProgramCount(){
	const deMutexGuard guard(pMutexCompilePrograms);
	return pPrograms.GetCount();
}

const deoglShaderProgram *deoglShaderManager::GetProgramAt( int index ){
	const deMutexGuard guard(pMutexCompilePrograms);
	return (deoglShaderProgram*)pPrograms.GetAt(index);
}

/*
bool deoglShaderManager::HasProgramWith(const deoglShaderSources *sources,
const deoglShaderDefines &defines) const{
	DEASSERT_NOTNULL(sources)
	
	const int count = pPrograms.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglShaderProgram &program = *(deoglShaderProgram* )pPrograms.GetAt(i);
		if(sources == program.GetSources() && defines.Equals(program.GetDefines())){
			return true;
		}
	}
	
	return false;
}
*/

const deoglShaderProgram *deoglShaderManager::GetProgramWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines){
	DEASSERT_NOTNULL(sources)
	DEASSERT_NOTNULL(pLanguage)
	
	const deMutexGuard guard(pMutexCompilePrograms);
	deoglShaderProgram * const foundProgram = pGetProgramWith(sources, defines);
	if(foundProgram){
		return foundProgram;
	}
	
	const deoglShaderProgram::Ref program(pCreateProgram(sources, defines));
	pPrograms.Add(program);
	
	deoglShaderCompiled *compiled = nullptr;
	try{
		compiled = pLanguage->CompileShader(*program);
		program->SetCompiled(compiled);
		
	}catch(const deException &){
		if(compiled){
			delete compiled;
		}
		throw;
	}
	
	return program;
}

void deoglShaderManager::GetProgramWithAsync(const deoglShaderSources *sources,
const deoglShaderDefines &defines, cGetProgramListener *listener){
	DEASSERT_NOTNULL(sources)
	DEASSERT_NOTNULL(pLanguage)
	
	cCompileProgram *compiling = nullptr;
	
	{
	const deMutexGuard guard(pMutexCompilePrograms);
	deoglShaderProgram * const foundProgram = pGetProgramWith(sources, defines);
	if(foundProgram){
		try{
			listener->GetProgramFinished(foundProgram);
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
		return;
	}
	
	compiling = pGetCompilingWith(sources, defines);
	if(compiling){
		compiling->AddListener(listener);
		return;
	}
	
	try{
		compiling = new cCompileProgram(*this, deoglShaderProgram::Ref::New(
			pCreateProgram(sources, defines)));
		compiling->AddListener(listener);
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
		if(compiling){
			delete compiling;
		}
		delete listener;
		throw;
	}
	pCompilePrograms.Add(compiling);
	}
	
	// has to be outside mutex lock since call can immediately call listener which acquires lock
	pLanguage->CompileShaderAsync(compiling->GetProgram(), compiling);
}

void deoglShaderManager::WaitAllProgramsCompiled(){
	while(true){
		{
		const deMutexGuard guard(pMutexCompilePrograms);
		if(pCompilePrograms.GetCount() == 0){
			break;
		}
		}
		pSemaphoreCompileFinished.Wait();
	}
}


// Private Functions
//////////////////////

deoglShaderProgram *deoglShaderManager::pGetProgramWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines) const{
	const int count = pPrograms.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglShaderProgram * const program = (deoglShaderProgram*)pPrograms.GetAt(i);
		if(sources == program->GetSources() && defines.Equals(program->GetDefines())){
			return program;
		}
	}
	
	return nullptr;
}

deoglShaderManager::cCompileProgram *deoglShaderManager::pGetCompilingWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines) const{
	const int count = pCompilePrograms.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cCompileProgram * const compiling = (cCompileProgram*)pCompilePrograms.GetAt(i);
		if(sources == compiling->GetProgram()->GetSources()
		&& defines.Equals(compiling->GetProgram()->GetDefines())){
			return compiling;
		}
	}
	
	return nullptr;
}

deoglShaderProgram::Ref deoglShaderManager::pCreateProgram(
const deoglShaderSources *sources, const deoglShaderDefines &defines){
	const deoglShaderProgram::Ref program(deoglShaderProgram::Ref::New(
		new deoglShaderProgram(pRenderThread, sources, defines)));
	
	if(!sources->GetPathGeometrySourceCode().IsEmpty()){
		program->SetGeometrySourceCode(GetUnitSourceCodeWithPath(sources->GetPathGeometrySourceCode()));
		
		if(!program->GetGeometrySourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Geometry source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathGeometrySourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources->GetPathVertexSourceCode().IsEmpty()){
		program->SetVertexSourceCode(GetUnitSourceCodeWithPath(sources->GetPathVertexSourceCode()));
		
		if(!program->GetVertexSourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Vertex source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathVertexSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources->GetPathFragmentSourceCode().IsEmpty()){
		program->SetFragmentSourceCode(GetUnitSourceCodeWithPath(sources->GetPathFragmentSourceCode()));
		
		if(!program->GetFragmentSourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Fragment source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathFragmentSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources->GetPathComputeSourceCode().IsEmpty()){
		program->SetComputeSourceCode(GetUnitSourceCodeWithPath(sources->GetPathComputeSourceCode()));
		
		if(!program->GetComputeSourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Compute source '%s' not found", sources->GetFilename().GetString(),
				sources->GetPathComputeSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources->GetPathTessellationControlSourceCode().IsEmpty()){
		program->SetTessellationControlSourceCode(GetUnitSourceCodeWithPath(
			sources->GetPathTessellationControlSourceCode()));
		
		if(!program->GetTessellationControlSourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation control source '%s' not found",
				sources->GetFilename().GetString(),
				sources->GetPathTessellationControlSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources->GetPathTessellationEvaluationSourceCode().IsEmpty()){
		program->SetTessellationEvaluationSourceCode(GetUnitSourceCodeWithPath(
			sources->GetPathTessellationEvaluationSourceCode()));
		
		if(!program->GetTessellationEvaluationSourceCode()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation evaluation source '%s' not found",
				sources->GetFilename().GetString(),
				sources->GetPathTessellationEvaluationSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	decString cacheId;
	cacheId.Format("shader%d;%s;%s", SHADER_CACHE_REVISION,
		sources->GetFilename().GetString(), defines.CalcCacheId().GetString());
	program->SetCacheId(cacheId);
	
	return program;
}

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
		decString validationString;
		
		for( i=0; i<count; i++ ){
			const decPath &path = pathList.GetAt( i );
			filename = path.GetPathUnix().GetMiddle( basePathLen );
			/*if( ogl.GetConfiguration()->GetDoLogDebug() ){
				pRenderThread.GetLogger().LogInfoFormat( "Loading shader unit source code %s...", filename );
			}*/
			
			const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
				vfs.OpenFileForReading( path ) ) );
			
			AddUnitSourceCode( deoglShaderUnitSourceCode::Ref::New(
				new deoglShaderUnitSourceCode( filename, reader ) ) );
			
			validationString.Format( "%s: %" PRIu64, filename.GetString(),
				( uint64_t )reader->GetModificationTime() );
			pCacheValidationString.Add( validationString );
		}
		
	}catch( const deException & ){
		pRenderThread.GetLogger().LogInfoFormat(
			"Loading shader unit source code %s failed!", filename.GetString() );
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
		decString validationString;
		
		for( i=0; i<count; i++ ){
			const decPath &path = pathList.GetAt( i );
			filename = path.GetPathUnix().GetMiddle( basePathLen );
			/*if( ogl.GetConfiguration()->GetDoLogDebug() ){
				pRenderThread.GetLogger().LogInfoFormat( "Loading shader %s...", filename );
			}*/
			
			const decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
					vfs.OpenFileForReading( path ) ) );
			
			const deoglShaderSources::Ref sources( deoglShaderSources::Ref::New(
				new deoglShaderSources( logger, reader ) ) );
			
			if( pSources.Has( sources->GetName() ) ){
				ogl.LogErrorFormat( "Shader file '%s' defines a shader named '%s' but"
					" a shader with this name already exists!",
					sources->GetFilename().GetString(), sources->GetName().GetString() );
				DETHROW( deeInvalidParam );
			}
			
			pSources.SetAt( sources->GetName(), sources );
			
			validationString.Format( "%s: %" PRIu64, filename.GetString(),
				( uint64_t )reader->GetModificationTime() );
			pCacheValidationString.Add( validationString );
		}
		
	}catch( const deException & ){
		pRenderThread.GetLogger().LogInfoFormat( "Loading shader %s failed!", filename.GetString() );
		throw;
	}
}
