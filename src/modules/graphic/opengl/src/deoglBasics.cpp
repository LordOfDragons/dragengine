#include <stdio.h>

#include <math.h>

#include "deoglBasics.h"

#include "memory/deoglMemoryManager.h"
#include "renderthread/deoglRenderThread.h"
#include "renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/string/decString.h>


#ifdef OGL_THREAD_CHECK
#include <dragengine/common/exceptions.h>

#ifdef OS_UNIX
static pthread_t pRenderThreadPid = 0;
static pthread_t pLoaderThreadPid = 0;
static pthread_t pMainThreadPid = 0;

void ogldbgInitThreadCheck(){
	pRenderThreadPid = pthread_self();
}
void dbgInitLoaderThreadCheck(){
	pLoaderThreadPid = pthread_self();
}
void dbgExitLoaderThreadCheck(){
	pLoaderThreadPid = 0;
}
void ogldbgInitMainThreadCheck(){
	pMainThreadPid = pthread_self();
}
#endif

#ifdef OS_W32
static DWORD pRenderThreadPid = 0;
static DWORD pLoaderThreadPid = 0;
static DWORD pMainThreadPid = 0;

void ogldbgInitThreadCheck(){
	pRenderThreadPid = GetCurrentThreadId();
}
void dbgInitLoaderThreadCheck(){
	pLoaderThreadPid = GetCurrentThreadId();
}
void dbgExitLoaderThreadCheck(){
	pLoaderThreadPid = 0;
}
void ogldbgInitMainThreadCheck(){
	pMainThreadPid = GetCurrentThreadId();
}
#endif

void ogldbgOnMainThreadCheck(){
	#ifdef OS_UNIX
	if( pthread_self() != pMainThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	if( GetCurrentThreadId() != pMainThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
}

void dbgOnRenderThreadCheck(){
	#ifdef OS_UNIX
	const pthread_t t = pthread_self();
	if( t != pRenderThreadPid && t != pLoaderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	const DWORD t = GetCurrentThreadId();
	if( t != pRenderThreadPid && t != pLoaderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
}

void dbgOnMainOrRenderThreadCheck(){
	#ifdef OS_UNIX
	const pthread_t t = pthread_self();
	if( t != pMainThreadPid && t != pRenderThreadPid &&  t != pLoaderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	const DWORD t = GetCurrentThreadId();
	if( t != pMainThreadPid && t != pRenderThreadPid && t != pLoaderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
}
#endif



// Debug timer class to provide flodding the logger for high frequency logging.
class PrintDelayTimer{
private:
	decTimer pTimer;
	float pElapsed;
	float pThreshold;
	
public:
	PrintDelayTimer( float threshold ) :
	pElapsed( 0.0f ),
	pThreshold( threshold ){
	};
	
	bool Check(){
		pElapsed += pTimer.GetElapsedTime();
		
		if( pElapsed >= pThreshold ){
			pElapsed = fmodf( pElapsed, pThreshold );
			return true;
			
		}else{
			return false;
		}
	}
};



// Prints memory usage
//static PrintDelayTimer( 1.0f );
void dbgPrintMemoryUsage( deoglRenderThread &renderThread ){
	deoglMemoryConsumption &consumption = renderThread.GetMemoryManager().GetConsumption();
	const deoglMemoryConsumptionSkin &conSkin = consumption.skin;
	const deoglMemoryConsumptionTexture &conTex2D = consumption.texture2D;
	const deoglMemoryConsumptionTexture &conTex2DRen = consumption.texture2DRenderable;
	const deoglMemoryConsumptionTexture &conTex3D = consumption.texture3D;
	const deoglMemoryConsumptionTexture &conTex3DRen = consumption.texture3DRenderable;
	const deoglMemoryConsumptionTexture &conTexArr = consumption.textureArray;
	const deoglMemoryConsumptionTexture &conTexArrRen = consumption.textureArrayRenderable;
	const deoglMemoryConsumptionTexture &conTexCube = consumption.textureCube;
	const deoglMemoryConsumptionTexture &conTexCubeRen = consumption.textureCubeRenderable;
	const deoglMemoryConsumptionBufferObject &conBO = consumption.bufferObject;
	
	renderThread.GetLogger().LogWarnFormat(
		"%" OGLPFLLU "M:"
		" skin(%d,%dM)"
		" tex2D(%d,%dM|%d,%dM)"
		" tex3D(%d,%dM|%d,%dM)"
		" texArr(%d,%dM|%d,%dM)"
		" texCube(%d,%dM|%d,%dM)"
		" vbo(%d,%dM|%d,%dM)"
		" ibo(%d,%dM|%d,%dM)"
		" ubo(%d,%dM)"
		" tbo(%d,%dM)"
		,
		( conSkin.all.GetConsumption() + conTex2D.all.GetConsumption()
		+ conTex2DRen.all.GetConsumption() + conTex3D.all.GetConsumption()
		+ conTex3DRen.all.GetConsumption() + conTexArr.all.GetConsumption()
		+ conTexArrRen.all.GetConsumption() + conTexCube.all.GetConsumption()
		+ conTexCubeRen.all.GetConsumption() + conBO.vbo.GetConsumption()
		+ conBO.ibo.GetConsumption() + conBO.ubo.GetConsumption()
		+ conBO.tbo.GetConsumption() + conBO.ssbo.GetConsumption() ) / 1000000ull,
		
		conSkin.all.GetCount(), conSkin.all.GetConsumptionMB(),
		conTex2D.all.GetCount(), conTex2D.all.GetConsumptionMB(),
		conTex2DRen.all.GetCount(), conTex2DRen.all.GetConsumptionMB(),
		conTex3D.all.GetCount(), conTex3D.all.GetConsumptionMB(),
		conTex3DRen.all.GetCount(), conTex3DRen.all.GetConsumptionMB(),
		conTexArr.all.GetCount(), conTexArr.all.GetConsumptionMB(),
		conTexArrRen.all.GetCount(), conTexArrRen.all.GetConsumptionMB(),
		conTexCube.all.GetCount(), conTexCube.all.GetConsumptionMB(),
		conTexCubeRen.all.GetCount(), conTexCubeRen.all.GetConsumptionMB(),
		conBO.vbo.GetCount(), conBO.vbo.GetConsumptionMB(),
		conBO.vboShared.GetCount(), conBO.vboShared.GetConsumptionMB(),
		conBO.ibo.GetCount(), conBO.ibo.GetConsumptionMB(),
		conBO.iboShared.GetCount(), conBO.iboShared.GetConsumptionMB(),
		conBO.ubo.GetCount(), conBO.ubo.GetConsumptionMB(),
		conBO.tbo.GetCount(), conBO.tbo.GetConsumptionMB()
	);
}

void oglClearError(){
	while( glGetError() != GL_NO_ERROR );
}

void dbgCheckOglError(deoglRenderThread&, const char *file, int line, bool withRenderThreadCheck){
	const GLenum err = glGetError();
	
	switch( err ){
	case GL_NO_ERROR:
		break;
		
	case GL_INVALID_ENUM:
		throw deeInvalidParam( file, line, "GL_INVALID_ENUM" );
		
	case GL_INVALID_VALUE:
		throw deeInvalidParam( file, line, "GL_INVALID_VALUE" );
		
	case GL_INVALID_OPERATION:
		throw deeInvalidParam( file, line, "GL_INVALID_OPERATION" );
		
	case GL_STACK_OVERFLOW:
		throw deeInvalidParam( file, line, "GL_STACK_OVERFLOW" );
		
	case GL_STACK_UNDERFLOW:
		throw deeInvalidParam( file, line, "GL_STACK_UNDERFLOW" );
		
	case GL_OUT_OF_MEMORY:
		throw deeInvalidParam( file, line, "GL_OUT_OF_MEMORY" );
		
	default:{
		decString message;
		message.Format( "Error %x (%d)", err, err );
		throw deeInvalidParam( file, line, message );
		}
	}
	
	if(withRenderThreadCheck){
		OGL_ON_RENDER_THREAD
	}
}

#ifdef OS_WEBWASM

void oglCheckWasmError(deoglRenderThread &renderThread, EMSCRIPTEN_RESULT result, const char *file, int line){
	switch(result){
	case EMSCRIPTEN_RESULT_SUCCESS:
		break;
		
	case EMSCRIPTEN_RESULT_DEFERRED:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_DEFERRED");
		
	case EMSCRIPTEN_RESULT_NOT_SUPPORTED:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_NOT_SUPPORTED");
		
	case EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED");
		
	case EMSCRIPTEN_RESULT_INVALID_TARGET:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_INVALID_TARGET");
		
	case EMSCRIPTEN_RESULT_UNKNOWN_TARGET:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_UNKNOWN_TARGET");
		
	case EMSCRIPTEN_RESULT_INVALID_PARAM:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_INVALID_PARAM");
		
	case EMSCRIPTEN_RESULT_FAILED:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_FAILED");
		
	case EMSCRIPTEN_RESULT_NO_DATA:
		throw deeInvalidParam(file, line, "EMSCRIPTEN_RESULT_NO_DATA");
		
	default:{
		decString message;
		message.Format("Failed %x (%d)", result, result);
		throw deeInvalidParam(file, line, message);
		}
	}
}

#endif
