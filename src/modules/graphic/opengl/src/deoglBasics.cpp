#include <stdio.h>

#include <math.h>

#include "deoglGL.h"

#include "memory/deoglMemoryManager.h"
#include "renderthread/deoglRenderThread.h"
#include "renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/string/decString.h>



#if defined OGL_THREAD_CHECK || defined OGL_CHECKCOMMANDS
#include <dragengine/logger/deLoggerConsoleColor.h>
struct sDebugHacksEnabled{
	sDebugHacksEnabled(){
		deLoggerConsoleColor *logger = NULL;
		
		try{
			logger = new deLoggerConsoleColor;
			#ifdef OGL_THREAD_CHECK
			logger->LogWarn( "OpenGL", "OpenGL calls only in render thread check enabled. Disable for production builds." );
			#endif
			#ifdef OGL_CHECKCOMMANDS
			logger->LogWarn( "OpenGL", "OpenGL command failure check enabled. Disable for production builds." );
			#endif
			
			logger->FreeReference();
			
		}catch( ... ){
			if( logger ){
				logger->FreeReference();
			}
		}
	}
};

static sDebugHacksEnabled debugHacksEnabled;
#endif



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
			pElapsed = fmod( pElapsed, pThreshold );
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
	const deoglMemoryConsumptionTexture &conTex1D = consumption.texture1D;
	const deoglMemoryConsumptionTexture &conTex1DRen = consumption.texture1DRenderable;
	const deoglMemoryConsumptionTexture &conTex2D = consumption.texture2D;
	const deoglMemoryConsumptionTexture &conTex2DRen = consumption.texture2DRenderable;
	const deoglMemoryConsumptionTexture &conTex3D = consumption.texture3D;
	const deoglMemoryConsumptionTexture &conTex3DRen = consumption.texture3DRenderable;
	const deoglMemoryConsumptionTexture &conTexArr = consumption.textureArray;
	const deoglMemoryConsumptionTexture &conTexArrRen = consumption.textureArrayRenderable;
	const deoglMemoryConsumptionTexture &conTexCube = consumption.textureCube;
	const deoglMemoryConsumptionTexture &conTexCubeRen = consumption.textureCubeRenderable;
	const deoglMemoryConsumptionRenderBuffer &conRenBuf = consumption.renderbuffer;
	const deoglMemoryConsumptionBufferObject &conBO = consumption.bufferObject;
	
	renderThread.GetLogger().LogWarnFormat(
		"%" OGLPFLLU "M:"
		" skin(%d,%dM)"
		" tex1D(%d,%dM|%d,%dM)"
		" tex2D(%d,%dM|%d,%dM)"
		" tex3D(%d,%dM|%d,%dM)"
		" texArr(%d,%dM|%d,%dM)"
		" texCube(%d,%dM|%d,%dM)"
		" renBuf(%d,%dM)"
		" vbo(%d,%dM|%d,%dM)"
		" ibo(%d,%dM|%d,%dM)"
		" ubo(%d,%dM)"
		" tbo(%d,%dM)"
		,
		( conSkin.all.GetConsumption() + conTex1D.all.GetConsumption()
		+ conTex1DRen.all.GetConsumption() + conTex2D.all.GetConsumption()
		+ conTex2DRen.all.GetConsumption() + conTex3D.all.GetConsumption()
		+ conTex3DRen.all.GetConsumption() + conTexArr.all.GetConsumption()
		+ conTexArrRen.all.GetConsumption() + conTexCube.all.GetConsumption()
		+ conTexCubeRen.all.GetConsumption() + conRenBuf.all.GetConsumption()
		+ conBO.vbo.GetConsumption() + conBO.ibo.GetConsumption() + conBO.ubo.GetConsumption()
		+ conBO.tbo.GetConsumption() + conBO.ssbo.GetConsumption() ) / 1000000ull,
		
		conSkin.all.GetCount(), conSkin.all.GetConsumptionMB(),
		conTex1D.all.GetCount(), conTex1D.all.GetConsumptionMB(),
		conTex1DRen.all.GetCount(), conTex1DRen.all.GetConsumptionMB(),
		conTex2D.all.GetCount(), conTex2D.all.GetConsumptionMB(),
		conTex2DRen.all.GetCount(), conTex2DRen.all.GetConsumptionMB(),
		conTex3D.all.GetCount(), conTex3D.all.GetConsumptionMB(),
		conTex3DRen.all.GetCount(), conTex3DRen.all.GetConsumptionMB(),
		conTexArr.all.GetCount(), conTexArr.all.GetConsumptionMB(),
		conTexArrRen.all.GetCount(), conTexArrRen.all.GetConsumptionMB(),
		conTexCube.all.GetCount(), conTexCube.all.GetConsumptionMB(),
		conTexCubeRen.all.GetCount(), conTexCubeRen.all.GetConsumptionMB(),
		conRenBuf.all.GetCount(), conRenBuf.all.GetConsumptionMB(),
		conBO.vbo.GetCount(), conBO.vbo.GetConsumptionMB(),
		conBO.vboShared.GetCount(), conBO.vboShared.GetConsumptionMB(),
		conBO.ibo.GetCount(), conBO.ibo.GetConsumptionMB(),
		conBO.iboShared.GetCount(), conBO.iboShared.GetConsumptionMB(),
		conBO.ubo.GetCount(), conBO.ubo.GetConsumptionMB(),
		conBO.tbo.GetCount(), conBO.tbo.GetConsumptionMB()
	);
}

void dbgCheckOglError( deoglRenderThread &renderThread, const char *file, int line ){
	const GLenum err = glGetError();
	
	switch( err ){
	case GL_NO_ERROR:
		break;
		
	case GL_INVALID_ENUM:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_INVALID_ENUM" ) );
		break;
		
	case GL_INVALID_VALUE:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_INVALID_VALUE" ) );
		break;
		
	case GL_INVALID_OPERATION:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_INVALID_OPERATION" ) );
		break;
		
	case GL_STACK_OVERFLOW:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_STACK_OVERFLOW" ) );
		break;
		
	case GL_STACK_UNDERFLOW:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_STACK_UNDERFLOW" ) );
		break;
		
	case GL_OUT_OF_MEMORY:
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, "GL_OUT_OF_MEMORY" ) );
		//dbgPrintMemoryUsage( renderThread );
		break;
		
	default:{
		decString message;
		message.Format( "Error %x (%d)", err, err );
		renderThread.GetLogger().LogException( deeInvalidParam( file, line, message ) );
		}
	}
	
	OGL_ON_RENDER_THREAD
}
