#include <stdio.h>
#include "deoalBasics.h"
#include "audiothread/deoalAudioThread.h"
#include "audiothread/deoalATLogger.h"

#ifdef OAL_THREAD_CHECK
#include <dragengine/common/exceptions.h>

#ifdef OS_UNIX
static pthread_t pAudioThreadPid = 0;
static pthread_t pMainThreadPid = 0;

void oaldbgInitThreadCheck(){
	pAudioThreadPid = pthread_self();
}
void oaldbgInitMainThreadCheck(){
	pMainThreadPid = pthread_self();
}
#endif

#ifdef OS_W32
static DWORD pAudioThreadPid = 0;
static DWORD pMainThreadPid = 0;

void oaldbgInitThreadCheck(){
	pAudioThreadPid = GetCurrentThreadId();
}
void oaldbgInitMainThreadCheck(){
	pMainThreadPid = GetCurrentThreadId();
}
#endif

void oaldbgOnMainThreadCheck(){
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



void dbgCheckOalError( deoalAudioThread &audioThread, const char *file, int line ){
	int error = alGetError();
	
	if( error != AL_NO_ERROR ){
		switch( error ){
		case AL_INVALID_NAME:
			audioThread.GetLogger().LogErrorFormat( "AL_INVALID_NAME at %s:%i", file, line );
			break;
			
		case AL_INVALID_ENUM:
			audioThread.GetLogger().LogErrorFormat( "AL_INVALID_ENUM at %s:%i", file, line );
			break;
			
		case AL_INVALID_VALUE:
			audioThread.GetLogger().LogErrorFormat( "AL_INVALID_VALUE at %s:%i", file, line );
			break;
			
		case AL_INVALID_OPERATION:
			audioThread.GetLogger().LogErrorFormat( "AL_INVALID_OPERATION at %s:%i", file, line );
			break;
			
		case AL_OUT_OF_MEMORY:
			audioThread.GetLogger().LogErrorFormat( "AL_OUT_OF_MEMORY at %s:%i", file, line );
			break;
			
		default:
			audioThread.GetLogger().LogErrorFormat( "openal error %X at %s:%i", error, file, line );
		};
	}
	
	#ifdef OAL_THREAD_CHECK
	
	#ifdef OS_UNIX
	if( pthread_self() != pAudioThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#ifdef OS_W32
	if( GetCurrentThreadId() != pAudioThreadPid ){
		DETHROW( deeInvalidAction );
	}
	#endif
	
	#endif
}
