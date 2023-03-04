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
static pthread_t pMainThreadPid = 0;

void dbgInitThreadCheck(){
	pRenderThreadPid = pthread_self();
}
void dbgInitMainThreadCheck(){
	pMainThreadPid = pthread_self();
}
#endif

#ifdef OS_W32
static DWORD pRenderThreadPid = 0;
static DWORD pMainThreadPid = 0;

void dbgInitThreadCheck(){
	pRenderThreadPid = GetCurrentThreadId();
}
void dbgInitMainThreadCheck(){
	pMainThreadPid = GetCurrentThreadId();
}
#endif

void dbgOnMainThreadCheck(){
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
	if( pthread_self() != pRenderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	if( GetCurrentThreadId() != pRenderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
}

void dbgOnMainOrRenderThreadCheck(){
	#ifdef OS_UNIX
	const pthread_t t = pthread_self();
	if( t != pMainThreadPid && t != pRenderThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	const DWORD t = GetCurrentThreadId();
	if( t != pMainThreadPid && t != pRenderThreadPid ){
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

void dbgCheckOglError( deoglRenderThread&, const char *file, int line ){
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
	
	OGL_ON_RENDER_THREAD
}

void oglWaitFence( deoglRenderThread &renderThread, GLsync &fence, const char *file, int line ){
	if( ! fence ){
		return;
	}
	
	oglClearError();
	
	switch( pglClientWaitSync( fence, 0, 1000000000 ) ){ // 1s timeout
	case GL_ALREADY_SIGNALED:
	case GL_CONDITION_SATISFIED:
		fence = 0;
		return;
		
	case GL_TIMEOUT_EXPIRED:
		fence = 0;
		throw deeInvalidAction( file, line, "Timeout while waiting for fence" );
		
	case GL_WAIT_FAILED:
		fence = 0;
		dbgCheckOglError( renderThread, file, line );
		return;
		
	default:
		fence = 0;
		throw deeInvalidAction( file, line, "Unknown return value while waiting for fence" );
	}
}
