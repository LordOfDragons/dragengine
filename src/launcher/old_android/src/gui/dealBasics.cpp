#include "dealBasics.h"
#include "../dealLauncher.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"

void dbgCheckEglError( dealLauncher &launcher, const char *file, int line ){
	const GLenum err = eglGetError();
	
	switch( err ){
	case EGL_SUCCESS:
		break;
		
	case EGL_NOT_INITIALIZED:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_NOT_INITIALIZED at %s:%d", file, line );
		break;
		
	case EGL_BAD_ACCESS:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_ACCESS at %s:%d", file, line );
		break;
		
	case EGL_BAD_ALLOC:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_ALLOC at %s:%d", file, line );
		break;
		
	case EGL_BAD_ATTRIBUTE:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_ATTRIBUTE at %s:%d", file, line );
		break;
		
	case EGL_BAD_CONTEXT:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_CONTEXT at %s:%d", file, line );
		break;
		
	case EGL_BAD_CONFIG:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_CONFIG at %s:%d", file, line );
		break;
		
	case EGL_BAD_CURRENT_SURFACE:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_CURRENT_SURFACE at %s:%d", file, line );
		break;
		
	case EGL_BAD_DISPLAY:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_DISPLAY at %s:%d", file, line );
		break;
		
	case EGL_BAD_SURFACE:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_SURFACE at %s:%d", file, line );
		break;
		
	case EGL_BAD_MATCH:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_MATCH at %s:%d", file, line );
		break;
		
	case EGL_BAD_PARAMETER:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_PARAMETER at %s:%d", file, line );
		break;
		
	case EGL_BAD_NATIVE_PIXMAP:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_NATIVE_PIXMAP at %s:%d", file, line );
		break;
		
	case EGL_BAD_NATIVE_WINDOW:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_BAD_NATIVE_WINDOW at %s:%d", file, line );
		break;
		
	case EGL_CONTEXT_LOST:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "EGL_CONTEXT_LOST at %s:%d", file, line );
		break;
		
	default:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "Error %x at %s:%d", err, file, line );
	}
}

void dbgCheckOglError( dealLauncher &launcher, const char *file, int line ){
	const GLenum err = glGetError();
	
	switch( err ){
	case GL_NO_ERROR:
		break;
		
	case GL_INVALID_ENUM:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_INVALID_ENUM at %s:%d", file, line );
		break;
		
	case GL_INVALID_VALUE:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_INVALID_VALUE at %s:%d", file, line );
		break;
		
	case GL_INVALID_OPERATION:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_INVALID_OPERATION at %s:%d", file, line );
		break;
		
	case GL_STACK_OVERFLOW:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_STACK_OVERFLOW at %s:%d", file, line );
		break;
		
	case GL_STACK_UNDERFLOW:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_STACK_UNDERFLOW at %s:%d", file, line );
		break;
		
	case GL_OUT_OF_MEMORY:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "GL_OUT_OF_MEMORY at %s:%d", file, line );
		//dbgPrintMemoryUsage( launcher );
		break;
		
	default:
		launcher.GetLogger().LogErrorFormat( LOGSOURCE, "Error %x at %s:%d", err, file, line );
	}
}
