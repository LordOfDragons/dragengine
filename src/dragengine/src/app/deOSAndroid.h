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

#ifndef _DEOSANDROID_H_
#define _DEOSANDROID_H_

#include "../dragengine_configuration.h"

#ifdef OS_ANDROID

#include <jni.h>

#include "deOS.h"
#include "../common/string/decString.h"

struct ANativeWindow;


/**
 * \brief Android operating system.
 */
class deOSAndroid : public deOS{
public:
	/**
	 * \brief Application configuration.
	 */
	struct sConfig{
		JavaVM *javavm = nullptr;
		
		ANativeWindow *nativeWindow = nullptr;
		
		/**
		 * \brief Path to game engine installation directory.
		 * 
		 * Typically this is File(context.filesDir, "dragengine").absolutePath .
		 */
		decString pathEngine;
		
		/**
		 * \brief Path to user configuration directory.
		 * 
		 * This directory includes engine configuration, game configurations and log files.
		 * 
		 * Typically this is File(context.filesDir, "dragengine-config").absolutePath
		 * or File(context.getExternalFilesDir(null), "dragengine-config").absolutePath .
		 */
		decString pathConfig;
		
		/**
		 * \brief Path to cache directory.
		 * 
		 * Typically this is File(context.cachedir, "dragengine").absolutePath
		 * or File(context.externalCacheDir, "dragengine").absolutePath .
		 */
		decString pathCache;
	};
	
	
	
private:
	const sConfig pConfig;
	
	int pScreenWidth;
	int pScreenHeight;
	int pScreenRefreshRate;
	void *pCurWindow;
	void *pHostingMainWindow;
	void *pHostingRenderWindow;
	bool pAppFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new android operating system object. */
	deOSAndroid(const sConfig &config);
	
	/** \brief Clean up the android operating system object. */
	virtual ~deOSAndroid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine path. */
	virtual decString GetPathEngine();
	
	/** \brief Share path. */
	virtual decString GetPathShare();
	
	/** \brief System configuration path. */
	virtual decString GetPathSystemConfig();
	
	/** \brief User configuration path. */
	virtual decString GetPathUserConfig();
	
	/** \brief User cache path. */
	virtual decString GetPathUserCache();
	
	/** \brief User capture path. */
	virtual decString GetPathUserCapture();
	
	/**
	 * \brief Process all events in the application event queue.
	 */
	virtual void ProcessEventLoop( bool sendToInputModule );
	/*@}*/
	
	
	
	/** \name Display information. */
	/*@{*/
	/** \brief Number of displays. */
	virtual int GetDisplayCount();
	
	/**
	 * \brief Current resolution of display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual decPoint GetDisplayCurrentResolution( int display );
	
	/**
	 * \brief Current refresh rate of display.
	 * \param display Index of display to get current refresh rate for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual int GetDisplayCurrentRefreshRate( int display );
	
	/**
	 * \brief Number of resolutions supported on display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual int GetDisplayResolutionCount( int display );
	
	/**
	 * \brief Resolution by index for display.
	 * \param display Index of display to get current resolution for.
	 * \param resolution Index of resolution on display.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * \throws deeInvalidParam \em resolution is less than 0 or equal to or greater than
	 *                         GetDisplayResolutionCount(display).
	 */
	virtual decPoint GetDisplayResolution( int display, int resolution );
	
	/**
	 * \brief Current user locale language (ISO 639 language code) in lower case.
	 * \version 1.16
	 */
	virtual decString GetUserLocaleLanguage();
	
	/**
	 * \brief Current user locale territory (ISO 3166 country code) lower case or empty string.
	 * \version 1.16
	 */
	virtual decString GetUserLocaleTerritory();
	
	/**
	 * \brief Current global scaling factor for display.
	 * \version 1.20.
	 * \param display Index of display to get scaling factor for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	virtual int GetDisplayCurrentScaleFactor( int display );
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/**
	 * \brief Cast to deOSAndroid.
	 * \throws deeInvalidParam Not an instance of deOSAndroid.
	 */
	virtual deOSAndroid *CastToOSAndroid();
	/*@}*/
	
	
	
	/** \name Android related */
	/*@{*/
	/** \brief Java VM. */
	inline JavaVM *GetJavaVM() const{ return pConfig.javavm; }
	
	/** \brief Window. */
	inline ANativeWindow *GetNativeWindow() const{ return pConfig.nativeWindow; }
	
	
	
	/**
	 * \brief Current game window.
	 * 
	 * This is usually the window created by the graphic module but can also
	 * be some other window. Only one window can be the application window.
	 * If you need more windows than one parent it to the window you set.
	 */
	inline void *GetWindow() const{ return pCurWindow; }
	
	/**
	 * \brief Set current game window.
	 * 
	 * This is usually the window created by the graphic module but can also
	 * be some other window. Only one window can be the application window.
	 * If you need more windows than one parent it to the window you set. To
	 * remove the window specify NULL as window handler. Always set your
	 * window as otherwise input modules relying on message or event queues
	 * will not get them.
	 */
	void SetWindow( void *window );
	
	/** \brief Hosting main window or 0 if not set. */
	inline void *GetHostingMainWindow() const{ return pHostingMainWindow; }
	
	/**
	 * \brief Set hosting main window or 0 if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting main window is the main
	 * window of the hosting application that can be used to obtain visuals
	 * from. It is not necessary the one rendered into later on.
	 */
	void SetHostingMainWindow( void *window );
	
	/** \brief Hosting render window or 0 if not set. */
	inline void *GetHostingRenderWindow() const{ return pHostingRenderWindow; }
	
	/**
	 * \brief Set hosting render window or 0 if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting render window is the
	 * window on which the graphic module has to render from now on. This window
	 * can be the same as the hosting main window but can also be different from it.
	 */
	void SetHostingRenderWindow( void *window );
	
	/** \brief Determine if a hosting main window is set. */
	bool HasHostingMainWindow() const;
	
	/** \brief Determine if a hosting render window is set. */
	bool HasHostingRenderWindow() const;
	
	/** \brief Application is frozen. */
	inline bool GetAppFrozen() const{ return pAppFrozen; }
	
	/** \brief Set if application is frozen. */
	void SetAppFrozen( bool frozen );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetOSParameters();
};

#endif

#endif
