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

#include "../dragengine_configuration.h"

#ifdef OS_ANDROID

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include "deOSAndroid.h"
#include "../deEngine.h"
#include "../common/exceptions.h"

#include <android/configuration.h>
#include <android/looper.h>
#include <android/window.h>
#include <android/native_window.h>



// Class deOSAndroid
//////////////////////

// Constructor, Destructor
////////////////////////////

deOSAndroid::deOSAndroid(const sConfig &config) :
pConfig(config),
pScreenWidth(800),
pScreenHeight(600),
pScreenRefreshRate(30),
pCurWindow(nullptr),
pHostingMainWindow(nullptr),
pHostingRenderWindow(nullptr),
pAppFrozen(false)
{
	DEASSERT_NOTNULL(config.javavm)
	DEASSERT_FALSE(config.pathCache.IsEmpty())
	DEASSERT_FALSE(config.pathConfig.IsEmpty())
	DEASSERT_FALSE(config.pathEngine.IsEmpty())
	
	try{
		pGetOSParameters();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deOSAndroid::~deOSAndroid(){
	pCleanUp();
}



// Management
///////////////

decString deOSAndroid::GetPathEngine(){
	//return decString(DE_ENGINE_PATH);
	decString path;
	path.Format("%s/lib/dragengine", pConfig.pathEngine.GetString());
	return path;
}

decString deOSAndroid::GetPathShare(){
	//return decString(DE_SHARE_PATH);
	decString path;
	path.Format("%s/share/dragengine", pConfig.pathEngine.GetString());
	return path;
}

decString deOSAndroid::GetPathSystemConfig(){
	//return decString(DE_CONFIG_PATH);
	decString path;
	path.Format("%s/etc/dragengine", pConfig.pathEngine.GetString());
	return path;
}

decString deOSAndroid::GetPathUserConfig(){
	return pConfig.pathConfig;
}

decString deOSAndroid::GetPathUserCache(){
	return pConfig.pathCache;
}

decString deOSAndroid::GetPathUserCapture(){
	decString path;
	path.Format("%s/capture", pConfig.pathCache.GetString());
	return path;
}

void deOSAndroid::ProcessEventLoop(bool sendToInputModule){
	// not supported yet
	/*
	if(pConfig.nativeWindow){
		pScreenWidth = ANativeWindow_getWidth(pConfig.nativeWindow);
		pScreenHeight = ANativeWindow_getHeight(pConfig.nativeWindow);
	}
	*/
}

decString deOSAndroid::GetUserLocaleLanguage(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			return ls.GetLeft(deli).GetLower();
			
		}else{
			return ls.GetLower();
		}
	}
	return "en";
}

decString deOSAndroid::GetUserLocaleTerritory(){
	const char * const l = setlocale(LC_ALL, nullptr);
	if(l){
		const decString ls(l);
		const int deli = ls.Find('_');
		if(deli != -1){
			const int deli2 = ls.Find('.', deli + 1);
			if(deli2 != -1){
				return ls.GetMiddle(deli + 1, deli2).GetLower();
				
			}else{
				return ls.GetMiddle(deli + 1).GetLower();
			}
			
		}else{
			return ls.GetLower();
		}
	}
	return "";
}



// Display information
////////////////////////

int deOSAndroid::GetDisplayCount(){
	return 1;
}

decPoint deOSAndroid::GetDisplayCurrentResolution(int display){
	DEASSERT_TRUE(display == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSAndroid::GetDisplayCurrentRefreshRate(int display){
	DEASSERT_TRUE(display == 0)
	
	return pScreenRefreshRate;
}

int deOSAndroid::GetDisplayResolutionCount(int display){
	DEASSERT_TRUE(display == 0)
	
	return 1;
}

decPoint deOSAndroid::GetDisplayResolution(int display, int resolution){
	DEASSERT_TRUE(display == 0)
	DEASSERT_TRUE(resolution == 0)
	
	return decPoint(pScreenWidth, pScreenHeight);
}

int deOSAndroid::GetDisplayCurrentScaleFactor(int display){
	DEASSERT_TRUE(display == 0)
	
	return 100;
}



// Casting
////////////

deOSAndroid *deOSAndroid::CastToOSAndroid(){
	return this;
}



// Android related
////////////////////

void deOSAndroid::SetWindow(void *wnd){
	pCurWindow = wnd;
}

void deOSAndroid::SetHostingMainWindow(void *window){
	pHostingMainWindow = window;
}

void deOSAndroid::SetHostingRenderWindow(void *window){
	pHostingRenderWindow = window;
}

bool deOSAndroid::HasHostingMainWindow() const{
	return pHostingMainWindow != nullptr;
}

bool deOSAndroid::HasHostingRenderWindow() const{
	return pHostingRenderWindow != nullptr;
}



void deOSAndroid::SetAppFrozen(bool frozen){
	pAppFrozen = frozen;
}



// Private Functions
//////////////////////

void deOSAndroid::pCleanUp(){
}

void deOSAndroid::pGetOSParameters(){
	// NOTE
	// int32_t ANativeWindow_setBuffersGeometry( ANativeWindow *window, int32_t width, int32_t height, int32_t format )
	// sets the size in pixels of the window buffer while keeping the real window size the same. upscale if
	// not matching. could be used to increase performance by rendering to a smaller window buffer with automatic
	// upscaling
	// format = { WINDOW_FORMAT_RGBA_8888 = 1, WINDOW_FORMAT_RGBX_8888 = 2, WINDOW_FORMAT_RGB_565 = 4 }
	/*
	if(pConfig.nativeWindow){
		pScreenWidth = ANativeWindow_getWidth(pConfig.nativeWindow);
		pScreenHeight = ANativeWindow_getHeight(pConfig.nativeWindow);
	}
	*/
	
	// update window flags in case the engine ran in the mean time altering them.
	// first parameter adds flags while second one removes flags
	// 
	// AWINDOW_FLAG_ALLOW_LOCK_WHILE_SCREEN_ON
	//   window can be locked while app is in use
	// 
	// AWINDOW_FLAG_KEEP_SCREEN_ON 
	//   prevent from dimming and locking while app is in use
	// 
	// AWINDOW_FLAG_LAYOUT_IN_SCREEN
	//   window fills entire screen
	// 
	// AWINDOW_FLAG_FULLSCREEN
	//   hide status bars and alike
	/*
	ANativeActivity_setWindowFlags(pConfig.activity,
		AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_FULLSCREEN, 0);
	*/
	
	// get refresh rate. requires JNI to get the desired value. can be fetched once and stored
	// we assume jni env is already attached by the caller. doing so again here is a no-op but
	// we still get the current thread jni pointer. we do not detach since this is the job
	// of the caller
	JNIEnv *env = nullptr;
	pConfig.javavm->AttachCurrentThread(&env, 0);
	
	{
	// get application context
	jclass clsActivityThread = env->FindClass("android/app/ActivityThread");
	jmethodID metCurrentActivityThread = env->GetStaticMethodID(clsActivityThread,
		"currentActivityThread", "()Landroid/app/ActivityThread;");
	jobject objActivityThread = env->CallStaticObjectMethod(clsActivityThread, metCurrentActivityThread);
	
	jmethodID metGetApplication = env->GetMethodID(clsActivityThread,
		"getApplication", "()Landroid/app/Application;");
	jobject objContext = env->CallObjectMethod(objActivityThread, metGetApplication);
	
	// WindowManager windowManager = (WindowManager)pActivity.getSystemService(Context.WINDOW_SERVICE);
	jclass clsContext = env->GetObjectClass(objContext);
	jfieldID fldWindowService = env->GetStaticFieldID(clsContext, "WINDOW_SERVICE", "Ljava/lang/String;");
	jobject objWindowService = env->GetStaticObjectField(clsContext, fldWindowService);
	
	jmethodID metGetSystemService = env->GetMethodID(clsContext, "getSystemService",
		"(Ljava/lang/String;)Ljava/lang/Object;");
	jobject objWindowManager = env->CallObjectMethod(objContext, metGetSystemService, objWindowService);
	
	jclass clsWindowManager = env->GetObjectClass(objWindowManager);
	jmethodID metGetDefaultDisplay = env->GetMethodID(clsWindowManager,
		"getDefaultDisplay", "()Landroid/view/Display;");
	jobject objDisplay = env->CallObjectMethod(objWindowManager, metGetDefaultDisplay);
	
	// float refreshRating = display.getRefreshRate(); // in hertz
	jclass clsDisplay = env->GetObjectClass(objDisplay);
	jmethodID metGetRefreshRate = env->GetMethodID(clsDisplay, "getRefreshRate", "()F");
	pScreenRefreshRate = (int)(env->CallFloatMethod(objDisplay, metGetRefreshRate) + 0.1f);
	
	// display resolution
	jclass clsPoint = env->FindClass("android/graphics/Point");
	
	jmethodID metPointConstr = env->GetMethodID(clsPoint, "<init>", "()V");
	jobject objPoint = env->NewObject(clsPoint, metPointConstr);
	
	jmethodID metGetSize = env->GetMethodID(clsDisplay, "getSize", "(Landroid/graphics/Point;)V");
	env->CallVoidMethod(objDisplay, metGetSize, objPoint);
	
	pScreenWidth = env->GetIntField(objPoint, env->GetFieldID(clsPoint, "x", "I"));
	pScreenHeight = env->GetIntField(objPoint, env->GetFieldID(clsPoint, "y", "I"));
	
	env->DeleteLocalRef(objPoint);
	env->DeleteLocalRef(clsDisplay);
	env->DeleteLocalRef(clsWindowManager);
	env->DeleteLocalRef(clsContext);
	}
	
	// init locale
	setlocale(LC_ALL, "");
}

#endif
