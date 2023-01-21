/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOSANDROID_H_
#define _DEOSANDROID_H_

#include "../dragengine_configuration.h"

#ifdef OS_ANDROID

#include "deOS.h"

struct ANativeActivity;
struct AConfiguration;
struct ALooper;
struct AInputQueue;
struct ANativeWindow;


/**
 * \brief Android operating system.
 */
class deOSAndroid : public deOS{
private:
	ANativeActivity &pActivity;
	AConfiguration &pConfig;
	ALooper &pLooper;
	AInputQueue &pInputQueue;
	ANativeWindow &pNativeWindow;
	
	int pScreenWidth;
	int pScreenHeight;
	int pScreenRefreshRate;
	void *pCurWindow;
	void *pHostingMainWindow;
	void *pHostingRenderWindow;
	bool pAppHasFocus;
	bool pAppFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new android operating system object. */
	deOSAndroid( ANativeActivity &activity, AConfiguration &config,
	ALooper &looper, AInputQueue &inputQueue, ANativeWindow &nativeWindow );
	
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
	/** \brief Native activity. */
	inline ANativeActivity &GetActivity() const{ return pActivity; }
	
	/** \brief Configuration. */
	inline AConfiguration &GetConfig() const{ return pConfig; }
	
	/** \brief Looper. */
	inline ALooper &GetLooper() const{ return pLooper; }
	
	/** \brief Input queue. */
	inline AInputQueue &GetInputQueue() const{ return pInputQueue; }
	
	/** \brief Window. */
	inline ANativeWindow &GetNativeWindow() const{ return pNativeWindow; }
	
	
	
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
	
	/** \brief Determine if the application has the focus. */
	inline bool GetAppHasFocus() const{ return pAppHasFocus; }
	
	/** \brief Set if the application has the focus. */
	void SetAppHasFocus( bool appHasFocus );
	
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
