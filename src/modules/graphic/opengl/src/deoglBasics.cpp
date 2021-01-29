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
	const deoglMemoryConsumptionTexture &conSkin = consumption.GetSkin();
	const deoglMemoryConsumptionTexture &conTex1D = consumption.GetTexture1D();
	const deoglMemoryConsumptionTexture &conTex1DRen = consumption.GetTexture1DRenderable();
	const deoglMemoryConsumptionTexture &conTex2D = consumption.GetTexture2D();
	const deoglMemoryConsumptionTexture &conTex2DRen = consumption.GetTexture2DRenderable();
	const deoglMemoryConsumptionTexture &conTex3D = consumption.GetTexture3D();
	const deoglMemoryConsumptionTexture &conTex3DRen = consumption.GetTexture3DRenderable();
	const deoglMemoryConsumptionTexture &conTexArr = consumption.GetTextureArray();
	const deoglMemoryConsumptionTexture &conTexArrRen = consumption.GetTextureArrayRenderable();
	const deoglMemoryConsumptionTexture &conTexCube = consumption.GetTextureCube();
	const deoglMemoryConsumptionTexture &conTexCubeRen = consumption.GetTextureCubeRenderable();
	const deoglMemoryConsumptionTexture &conRenBuf = consumption.GetRenderbuffer();
	const deoglMemoryConsumptionVBO &conVBO = consumption.GetVBO();
	
	renderThread.GetLogger().LogWarnFormat(
		"%iM:"
		" skin(%i,%iM)"
		" tex1D(%i,%iM|%i,%iM)"
		" tex2D(%i,%iM|%i,%iM)"
		" tex3D(%i,%iM|%i,%iM)"
		" texArr(%i,%iM|%i,%iM)"
		" texCube(%i,%iM|%i,%iM)"
		" renBuf(%i,%iM)"
		" vbo(%i,%iM|%i,%iM)"
		" ibo(%i,%iM)"
		" ubo(%i,%iM)"
		" tbo(%i,%iM)"
		,
		( conSkin.GetGPU() + conTex1D.GetGPU() + conTex1DRen.GetGPU() + conTex2D.GetGPU()
		+ conTex2DRen.GetGPU() + conTex3D.GetGPU() + conTex3DRen.GetGPU() + conTexArr.GetGPU()
		+ conTexArrRen.GetGPU() + conTexCube.GetGPU() + conTexCubeRen.GetGPU() + conRenBuf.GetGPU()
		+ conVBO.GetGPU() + conVBO.GetSharedGPU() + conVBO.GetIBOGPU() + conVBO.GetUBOGPU()
		+ conVBO.GetTBOGPU() ) / 1000000,
		
		conSkin.GetCount(), conSkin.GetGPU() / 1000000,
		conTex1D.GetCount(), conTex1D.GetGPU() / 1000000,
		conTex1DRen.GetCount(), conTex1DRen.GetGPU() / 1000000,
		conTex2D.GetCount(), conTex2D.GetGPU() / 1000000,
		conTex2DRen.GetCount(), conTex2DRen.GetGPU() / 1000000,
		conTex3D.GetCount(), conTex3D.GetGPU() / 1000000,
		conTex3DRen.GetCount(), conTex3DRen.GetGPU() / 1000000,
		conTexArr.GetCount(), conTexArr.GetGPU() / 1000000,
		conTexArrRen.GetCount(), conTexArrRen.GetGPU() / 1000000,
		conTexCube.GetCount(), conTexCube.GetGPU() / 1000000,
		conTexCubeRen.GetCount(), conTexCubeRen.GetGPU() / 1000000,
		conRenBuf.GetCount(), conRenBuf.GetGPU() / 1000000,
		conVBO.GetCount(), conVBO.GetGPU() / 1000000,
		conVBO.GetSharedCount(), conVBO.GetSharedGPU() / 1000000,
		conVBO.GetIBOCount(),conVBO.GetIBOGPU() / 1000000,
		conVBO.GetUBOCount(),conVBO.GetUBOGPU() / 1000000,
		conVBO.GetTBOCount(),conVBO.GetTBOGPU() / 1000000
	);
}

void dbgCheckOglError( deoglRenderThread &renderThread, const char *file, int line ){
	const GLenum err = glGetError();
	
	switch( err ){
	case GL_NO_ERROR:
		break;
		
	case GL_INVALID_ENUM:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_INVALID_ENUM" ) );
		break;
		
	case GL_INVALID_VALUE:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_INVALID_VALUE" ) );
		break;
		
	case GL_INVALID_OPERATION:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_INVALID_OPERATION" ) );
		break;
		
	case GL_STACK_OVERFLOW:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_STACK_OVERFLOW" ) );
		break;
		
	case GL_STACK_UNDERFLOW:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_STACK_UNDERFLOW" ) );
		break;
		
	case GL_OUT_OF_MEMORY:
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, "GL_OUT_OF_MEMORY" ) );
		//dbgPrintMemoryUsage( renderThread );
		break;
		
	default:{
		decString message;
		message.Format( "Error %x (%d)", err, err );
		renderThread.GetLogger().LogException( deeInvalidParam( __FILE__, __LINE__, message ) );
		}
	}
	
	#ifdef OGL_THREAD_CHECK
	
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
	
	#endif
}
