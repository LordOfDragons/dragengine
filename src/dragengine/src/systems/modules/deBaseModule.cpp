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

#include "deBaseModule.h"
#include "deModuleParameter.h"
#include "deLoadableModule.h"
#include "../deModuleSystem.h"
#include "../../deEngine.h"
#include "../../app/deOS.h"
#include "../../logger/deLogger.h"
#include "../../errortracing/deErrorTrace.h"
#include "../../errortracing/deErrorTracePoint.h"
#include "../../errortracing/deErrorTraceValue.h"
#include "../../common/file/decPath.h"
#include "../../common/string/decString.h"
#include "../../common/string/unicode/decUnicodeString.h"
#include "../../common/string/unicode/decUnicodeArgumentList.h"
#include "../../common/exceptions.h"
#include "../../filesystem/deVirtualFileSystem.h"
#include "../../filesystem/deVFSDiskDirectory.h"
#include "../../filesystem/deVFSNull.h"
#include "../../filesystem/deVFSCacheDiskDirectory.h"
#include "../../filesystem/deVFSRedirect.h"
#include "../../filesystem/deVFSContainer.h"



// Class deBaseModule
///////////////////////

// Constructors, destructors
//////////////////////////////

deBaseModule::deBaseModule( deLoadableModule &loadableModule ) :
pLoadableModule( loadableModule ),
pVFS( NULL )
{
	try{
		pCreateVFS();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deBaseModule::~deBaseModule(){
	pCleanUp();
}



// Parameters
///////////////

int deBaseModule::GetParameterCount() const{
	return 0;
}

void deBaseModule::GetParameterInfo( int index, deModuleParameter &parameter ) const{
	DETHROW( deeInvalidParam );
}

int deBaseModule::IndexOfParameterNamed( const char *name ) const{
	return -1;
}

decString deBaseModule::GetParameterValue( const char *name ) const{
	DETHROW( deeInvalidParam );
}

int deBaseModule::GetParameterInt( const char *name ) const{
	return GetParameterValue( name ).ToInt();
}

float deBaseModule::GetParameterFloat( const char *name ) const{
	return GetParameterValue( name ).ToFloat();
}

bool deBaseModule::GetParameterBool( const char *name ) const{
	return GetParameterValue( name ) == "1";
}

void deBaseModule::SetParameterValue( const char *name, const char *value ){
	DETHROW( deeInvalidParam );
}

void deBaseModule::SetParameterFloat( const char *name, float value ){
	decString stringValue;
	stringValue.Format( "%f", value );
	SetParameterValue( name, stringValue );
}

void deBaseModule::SetParameterInt( const char *name, int value ){
	decString stringValue;
	stringValue.Format( "%d", value );
	SetParameterValue( name, stringValue );
}

void deBaseModule::SetParameterBool( const char *name, bool value ){
	SetParameterValue( name, value ? "1" : "0" );
}



// Parallel Processing
////////////////////////

void deBaseModule::PauseParallelProcessingUpdate(){
}



// Debugging
//////////////

void deBaseModule::SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 0 ){
		answer.SetFromUTF8( "No command provided." );
		
	}else if( command.MatchesArgumentAt( 0, "help" ) ){
		answer.SetFromUTF8( "help => Displays this help screen." );
		
	}else{
		answer.SetFromUTF8( "Unknown command '" );
		answer += *command.GetArgumentAt( 0 );
		answer.AppendFromUTF8( "'." );
	}
}



// Accessors
//////////////

deEngine *deBaseModule::GetGameEngine() const{
	return pLoadableModule.GetSystem()->GetEngine();
}

deLoadableModule &deBaseModule::GetLoadableModule() const{
	return pLoadableModule;
}

deOS *deBaseModule::GetOS() const{
	return GetGameEngine()->GetOS();
}

decString deBaseModule::GetNativePathShare() const{
	const deModuleSystem &moduleSystem = *pLoadableModule.GetSystem();
	decPath path;
	
	path.SetFromNative( moduleSystem.GetEngine()->GetOS()->GetPathShare() );
	path.AddUnixPath( "modules" );
	path.AddUnixPath( moduleSystem.GetTypeDirectory( pLoadableModule.GetType() ) );
	path.AddUnixPath( pLoadableModule.GetDirectoryName() );
	path.AddComponent( pLoadableModule.GetVersion() );
	
	return path.GetPathNative();
}



// Debugging
//////////////

void deBaseModule::LogInfo( const char *message ){
	GetGameEngine()->GetLogger()->LogInfo( pLoadableModule.GetLoggingName(), message );
}

void deBaseModule::LogInfoFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	GetGameEngine()->GetLogger()->LogInfoFormatUsing( pLoadableModule.GetLoggingName(), message, list );
	va_end( list );
}

void deBaseModule::LogInfoFormatUsing( const char *message, va_list args ){
	GetGameEngine()->GetLogger()->LogInfoFormatUsing( pLoadableModule.GetLoggingName(), message, args );
}

void deBaseModule::LogWarn( const char *message ){
	GetGameEngine()->GetLogger()->LogWarn( pLoadableModule.GetLoggingName(), message );
}

void deBaseModule::LogWarnFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	GetGameEngine()->GetLogger()->LogWarnFormatUsing( pLoadableModule.GetLoggingName(), message, list );
	va_end( list );
}

void deBaseModule::LogWarnFormatUsing( const char *message, va_list args ){
	GetGameEngine()->GetLogger()->LogWarnFormatUsing( pLoadableModule.GetLoggingName(), message, args );
}

void deBaseModule::LogError( const char *message ){
	GetGameEngine()->GetLogger()->LogError( pLoadableModule.GetLoggingName(), message );
}

void deBaseModule::LogErrorFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	GetGameEngine()->GetLogger()->LogErrorFormatUsing( pLoadableModule.GetLoggingName(), message, list );
	va_end( list );
}

void deBaseModule::LogErrorFormatUsing( const char *message, va_list args ){
	GetGameEngine()->GetLogger()->LogErrorFormatUsing( pLoadableModule.GetLoggingName(), message, args );
}

void deBaseModule::LogException( const deException &exception ){
	GetGameEngine()->GetLogger()->LogException( pLoadableModule.GetLoggingName(), exception );
}



// Error Tracing
//////////////////

deErrorTracePoint *deBaseModule::SetErrorTrace( const char *error, const char *sourceFunc, int sourceLine ){
	return GetGameEngine()->GetErrorTrace()->AddAndSetIfEmpty( error, &pLoadableModule, sourceFunc, sourceLine );
}

deErrorTracePoint *deBaseModule::SetErrorTrace( const deException &exception ){
	decString text;
	text.Format( "Exception %s: %s", exception.GetName().GetString(), exception.GetDescription().GetString() );
	deErrorTracePoint * const tracepoint = GetGameEngine()->GetErrorTrace()->AddAndSetIfEmpty(
		text, &pLoadableModule, exception.GetFile(), exception.GetLine() );
	if( ! tracepoint ){
		return NULL;
	}
	
	tracepoint->AddValue( "Source File", exception.GetFile() );
	tracepoint->AddValueInt( "Source Line", exception.GetLine() );
	
	const decStringList &backtrace = exception.GetBacktrace();
	const int btcount = backtrace.GetCount();
	int i;
	
	text.Format( "%i trace points", btcount );
	deErrorTraceValue * const tpbacktrace = tracepoint->AddValue( "Backtrace", text );
	
	for( i=0; i<btcount; i++ ){
		text.Format( "Trace point %i", i + 1 );
		tpbacktrace->AddSubValue( text, backtrace.GetAt( i ) );
	}
	
	return tracepoint;
}

deErrorTracePoint *deBaseModule::AddErrorTracePoint( const char *sourceFunc, int sourceLine ){
	return GetGameEngine()->GetErrorTrace()->AddPoint( &pLoadableModule, sourceFunc, sourceLine );
}



// Private Functions
//////////////////////

void deBaseModule::pCleanUp(){
	if( pVFS ){
		pVFS->FreeReference();
	}
}

void deBaseModule::pCreateVFS(){
	const deModuleSystem &moduleSystem = *pLoadableModule.GetSystem();
	const char * const typeDirectory = moduleSystem.GetTypeDirectory( pLoadableModule.GetType() );
	const char * const directoryName = pLoadableModule.GetDirectoryName();
	const deEngine &engine = *moduleSystem.GetEngine();
	const deVirtualFileSystem::Ref &osFileSystem = engine.GetOSFileSystem();
	const deVirtualFileSystem::Ref &vfsAssetLibraries = moduleSystem.GetVFSAssetLibraries();
	deOS &os = *engine.GetOS();
	
	decPath pathRedirect;
	decPath pathDisk;
	decPath pathRoot;
	
	pVFS = new deVirtualFileSystem;
	
	if( osFileSystem ){
		// config directory (read-write, per module version)
		pathRedirect.SetFromUnix( "/config/modules" );
		pathRedirect.AddUnixPath( typeDirectory );
		pathRedirect.AddUnixPath( directoryName );
		
		pathRoot.SetFromUnix( "/config" );
		
		pVFS->AddContainer(deVFSRedirect::Ref::New(new deVFSRedirect(
			pathRoot, pathRedirect, osFileSystem, true)));
		
		// share directory (read-only, per module version)
		pathRedirect.SetFromUnix( "/share/modules" );
		pathRedirect.AddUnixPath( typeDirectory );
		pathRedirect.AddUnixPath( directoryName );
		pathRedirect.AddComponent( pLoadableModule.GetVersion() );
		
		pathRoot.SetFromUnix( "/share" );
		
		pVFS->AddContainer(deVFSRedirect::Ref::New(new deVFSRedirect(
			pathRoot, pathRedirect, osFileSystem, true)));
		
	}else{
		// system config directory (read-only, per module version)
		pathDisk.SetFromNative( os.GetPathSystemConfig() );
		pathDisk.AddUnixPath( "modules" );
		pathDisk.AddUnixPath( typeDirectory );
		pathDisk.AddUnixPath( directoryName );
		
		pathRoot.SetFromUnix( "/config" );
		
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			pathRoot, pathDisk, true)));
		
		
		// user config directory (writeable, shared between module versions)
		pathDisk.SetFromNative( os.GetPathUserConfig() );
		pathDisk.AddUnixPath( "modules" );
		pathDisk.AddUnixPath( typeDirectory );
		pathDisk.AddUnixPath( directoryName );
		
		pathRoot.SetFromUnix( "/config" );
		
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(pathRoot, pathDisk)));
		
		
		// share directory (read-only, per module version)
		pathDisk.SetFromNative( os.GetPathShare() );
		pathDisk.AddUnixPath( "modules" );
		pathDisk.AddUnixPath( typeDirectory );
		pathDisk.AddUnixPath( directoryName );
		pathDisk.AddComponent( pLoadableModule.GetVersion() );
		
		pathRoot.SetFromUnix( "/share" );
		
		pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(
			pathRoot, pathDisk, true)));
	}
	
	// asset libraries
	if(vfsAssetLibraries->GetContainerCount() > 0){
		pathRedirect.SetFromUnix("/modules");
		pathRedirect.AddUnixPath(typeDirectory);
		pathRedirect.AddUnixPath(directoryName);
		pathRedirect.AddComponent(pLoadableModule.GetVersion());
		
		pathRoot.SetFromUnix("/share");
		
		pVFS->AddContainer(deVFSRedirect::Ref::New(new deVFSRedirect(
			pathRoot, pathRedirect, vfsAssetLibraries, true)));
	}
	
	// capture directory (writeable, shared between module versions)
	pathDisk.SetFromNative( os.GetPathUserCapture() );
	pathDisk.AddUnixPath( "modules" );
	pathDisk.AddUnixPath( typeDirectory );
	pathDisk.AddUnixPath( directoryName );
	
	pathRoot.SetFromUnix( "/capture" );
	
	pVFS->AddContainer(deVFSDiskDirectory::Ref::New(new deVFSDiskDirectory(pathRoot, pathDisk)));
	
	
	// global cache directory (writeable, shared between module versions)
	pathDisk.SetFromNative( os.GetPathUserCache() );
	pathDisk.AddUnixPath( "global" );
	pathDisk.AddUnixPath( "modules" );
	pathDisk.AddUnixPath( typeDirectory );
	pathDisk.AddUnixPath( directoryName );
	
	pathRoot.SetFromUnix( "/cache/global" );
	
	deVFSCacheDiskDirectory::Ref cachedContainer(deVFSCacheDiskDirectory::Ref::New(
		new deVFSCacheDiskDirectory(pathRoot, pathDisk)));
	cachedContainer->SetMaxCacheSize(1000000); // 1000MB
	pVFS->AddContainer(cachedContainer);
	
	
	// local cache directory (writeable, shared between module versions)
	pathRoot.SetFromUnix( "/cache/local" );
	
	if( engine.GetCacheAppID().IsEmpty() ){
		pVFS->AddContainer(deVFSNull::Ref::New(new deVFSNull(pathRoot)));
		
	}else{
		pathDisk.SetFromNative( os.GetPathUserCache() );
		pathDisk.AddUnixPath( "local" );
		pathDisk.AddUnixPath( engine.GetCacheAppID() );
		pathDisk.AddUnixPath( "modules" );
		pathDisk.AddUnixPath( typeDirectory );
		pathDisk.AddUnixPath( directoryName );
		
		cachedContainer.TakeOver(new deVFSCacheDiskDirectory(pathRoot, pathDisk));
		cachedContainer->SetMaxCacheSize(1000000); // 1000MB
		pVFS->AddContainer(cachedContainer);
	}
}
