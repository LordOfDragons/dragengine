#include "android_native_app_glue.h"

#include <stdio.h>
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/asset_manager.h>

#include "dealLauncher.h"
#include "common/exceptions.h"
#include "logger/deLoggerConsole.h"

#define LOGSOURCE "DEAndroidLauncher"



/**
* Shared state for our app.
*/
/*
struct engine {
	struct android_app* app;

//	ASensorManager* sensorManager;
//	const ASensor* accelerometerSensor;
//	ASensorEventQueue* sensorEventQueue;
	
	dealLauncher *launcher;
};
*/

/** \brief Process the next input event. */
static int32_t processAndroidInputEvent(struct android_app *app, AInputEvent *event){
	((dealLauncher*)app->userData)->ProcessInputEvent(*event);
	return 1;
	/*
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->animating = 1;
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
	*/
}

#if 0
/** Enum assets in directory. */
static void enumAssetsIn(AAssetManager *assetmgr, const char *dirname){
	pNullLogger.LogInforFormat(LOGSOURCE, "List directory '%s'", dirname);
	AAssetDir *assetdir = AAssetManager_openDir(assetmgr, dirname[0]=='\0' ? "/" : dirname);
	
	const char *assetfile = AAssetDir_getNextFileName(assetdir);
	while(assetfile){
		pNullLogger.LogInforFormat(LOGSOURCE, "Asset %s/%s", dirname, assetfile);
		//AAsset *asset = AAssetManager_open( assetmgr, assetfile, AASSET_MODE_STREAMING );
		//
		//int AAsset_read(AAsset* asset, void* buf, size_t count);
		//off_t AAsset_seek(AAsset* asset, off_t offset, int whence);
		//off64_t AAsset_seek64(AAsset* asset, off64_t offset, int whence);
		//
		//off_t AAsset_getLength(AAsset* asset);
		//off64_t AAsset_getLength64(AAsset* asset);
		//
		//void AAsset_close(AAsset* asset);
// 		char buffer[ 256 ];
// 		sprintf( (char*)buffer, "%s/%s", dirname, assetfile );
// 		enumAssetsIn( assetmgr, buffer );
		assetfile = AAssetDir_getNextFileName(assetdir);
	}
	
	AAssetDir_close(assetdir);
}

/** Enum assets. */
static void enumAssets(AAssetManager *assetmgr){
	enumAssetsIn(assetmgr, "");
}
#endif



/** \brief Global null logger for big trouble situations. */
deLoggerConsole *pNullLogger = NULL;

static struct NullLogger{
	NullLogger(){
		pNullLogger = new deLoggerConsole;
	}
	~NullLogger(){
		if(pNullLogger){
			pNullLogger->FreeReference();
		}
	}
} vNullLogger;

/** \brief Process the next main command. */
static void processAndroidCommand(struct android_app *app, int32_t cmd){
	dealLauncher &launcher = *((dealLauncher*)app->userData);
	
	switch(cmd){
	case APP_CMD_SAVE_STATE:
		try{
			launcher.SaveState();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		
		break;
		
	case APP_CMD_INIT_WINDOW:
		// window is being shown, get it ready.
		if(! launcher.HasAppWindow()){
			break;
		};
		
		try{
			launcher.InitWindow();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		
		break;
		
	case APP_CMD_TERM_WINDOW:
		// window is being hidden or closed, clean it up.
		try{
			launcher.TerminateWindow();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_GAINED_FOCUS:
		try{
			launcher.FocusGained();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		
		// When our app gains focus, we start monitoring the accelerometer.
		/*
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
					engine->accelerometerSensor, (1000L/60)*1000);
		}
		*/
		break;
		
	case APP_CMD_LOST_FOCUS:
		try{
			launcher.FocusLost();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		/*
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		*/
		break;
		
	case APP_CMD_CONFIG_CHANGED:
		try{
			launcher.ConfigChanged();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_INPUT_CHANGED:
		try{
			launcher.InputChanged();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_START:
		try{
			launcher.AppStart();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_STOP:
		try{
			launcher.AppStop();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_PAUSE:
		try{
			launcher.AppPause();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_RESUME:
		try{
			launcher.AppResume();
			
		}catch(const deException &e){
			launcher.GetLogger().LogException(LOGSOURCE, e);
		}
		break;
		
	case APP_CMD_DESTROY:
		launcher.GetLogger().LogInfo(LOGSOURCE, "APP_CMD_DESTROY");
		break;
		
	default:
		launcher.GetLogger().LogWarnFormat(LOGSOURCE, "Unhandled command %d", cmd);
	}
}

/**
 * \brief Main entry point of a native application using android_native_app_glue.
 * \details Runs own thread, with own event loop for receiving input events and doing other things.
 */
void android_main(struct android_app *androidApp){
	// make sure glue isn't stripped.
	app_dummy();
	
	// create launcher
	dealLauncher *launcher = NULL;
	
	try{
		launcher = new dealLauncher(*androidApp);
		
	}catch(const deException &e){
		pNullLogger->LogException(LOGSOURCE, e);
		pNullLogger->FreeReference();
		return;
	}
	
	// init application object with callbacks and our user pointer
	androidApp->userData = launcher;
	androidApp->onAppCmd = processAndroidCommand;
	androidApp->onInputEvent = processAndroidInputEvent;
	
	// Prepare to monitor accelerometer
//	engine.sensorManager = ASensorManager_getInstance();
//	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
//			ASENSOR_TYPE_ACCELEROMETER);
//	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
//			state->looper, LOOPER_ID_USER, NULL, NULL);
	
	// loop waiting for stuff to do.
	bool keepRunning = true;
	
	while(keepRunning){
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;
		
		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident=ALooper_pollAll(0/*engine.animating ? 0 : -1*/, NULL, &events,
				(void**)&source)) >= 0) {
			
			// Process this event.
			if(source){
				source->process(androidApp, source);
			}
			
			// If a sensor has data, process it now.
			if(ident == LOOPER_ID_USER){
				/*
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
							&event, 1) > 0) {
// 						pNullLogger->LogInforFormat( LOGSOURCE, "accelerometer: x=%f y=%f z=%f",
// 								event.acceleration.x, event.acceleration.y,
// 								event.acceleration.z);
					}
				}
				*/
			}
		}
		
		try{
			keepRunning = launcher->FrameLoop();
			
		}catch(const deException &e){
			launcher->GetLogger().LogException(LOGSOURCE, e);
		}
		
		/*
		if (engine.animating) {
			// Done with events; draw next animation frame.
			
			engine.state.angle += .01f;
			if (engine.state.angle > 1) {
				engine.state.angle = 0;
			}

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine_draw_frame(&engine);
		}
		*/
	}
	
	// clean up
	if(launcher){
		try{
			delete launcher;
			pNullLogger->LogInfo(LOGSOURCE, "Launcher destroyed");
			
		}catch(const deException &e){
			pNullLogger->LogException(LOGSOURCE, e);
		}
		
		androidApp->userData = NULL;
	}
}
