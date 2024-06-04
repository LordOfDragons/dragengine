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

#ifndef _DEOSMACOS_H_
#define _DEOSMACOS_H_

#include "../dragengine_configuration.h"

#ifdef OS_MACOS

#include "deOS.h"

#ifdef OS_MACOS
#ifdef __OBJC__
@class NSWindow;
#else
class NSWindow;
#endif
#endif


/**
 * \brief MacOS operating system.
 */
class deOSMacOS : public deOS{
private:
	int pScreenWidth;
	int pScreenHeight;
	int pRefreshRate;
	NSWindow *pCurWindow;
	NSWindow *pHostingMainWindow;
	NSWindow *pHostingRenderWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new MacOS operating system object. */
	deOSMacOS();
	
	/** \brief Clean up the MacOS operating system object. */
	virtual ~deOSMacOS();
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
	 * 
	 * Certain events are directly processed. If \em sendToInputModule is true
	 * all events are also send to the active input module. Set \em sendToInputModule
	 * to false if you want to clear the event queue after a lengthy operation to
	 * avoid an event flood resulting in strange initial inputs.
	 */
	virtual void ProcessEventLoop( bool sendToInputModule );
	
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
	
	
	
	/** \name Display information. */
	/*@{*/
	/** \brief Number of displays. */
	virtual int GetDisplayCount();
	
	/**
	 * \brief Current resolution of display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 */
	virtual decPoint GetDisplayCurrentResolution( int display );
	
	/**
	 * \brief Current refresh rate of display.
	 * \param display Index of display to get current refresh rate for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 */
	virtual int GetDisplayCurrentRefreshRate( int display );
	
	/**
	 * \brief Number of resolutions supported on display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
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
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/**
	 * \brief Cast to deOSMacOS.
	 * \throws deeInvalidParam Not an instance of deOSMacOS.
	 */
	virtual deOSMacOS *CastToOSMacOS();
	/*@}*/
	
	
	
	/** \name MacOS related */
	/*@{*/
	/**
	 * \brief Current game window.
	 * 
	 * This is usually the window created by the graphic module but can also
	 * be some other window. Only one window can be the application window.
	 * If you need more windows than one parent it to the window you set.
	 */
	inline NSWindow *GetWindow() const{ return pCurWindow; }
	
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
	void SetWindow( NSWindow *window );
	
	/** \brief Hosting main window or NULL if not set. */
	inline NSWindow *GetHostingMainWindow() const{ return pHostingMainWindow; }
	
	/**
	 * \brief Set hosting main window or NULL if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting main window is the main
	 * window of the hosting application that can be used to obtain visuals
	 * from. It is not necessary the one rendered into later on.
	 */
	void SetHostingMainWindow( NSWindow *window );
	
	/** \brief Hosting render window or NULL if not set. */
	inline NSWindow *GetHostingRenderWindow() const{ return pHostingRenderWindow; }
	
	/**
	 * \brief Set hosting render window or NULL if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting render window is the
	 * window on which the graphic module has to render from now on. This window
	 * can be the same as the hosting main window but can also be different from it.
	 */
	void SetHostingRenderWindow( NSWindow *window );
	
	/** \brief Determine if a hosting main window is set. */
	bool HasHostingMainWindow() const;
	
	/** \brief Determine if a hosting render window is set. */
	bool HasHostingRenderWindow() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	decString pGetHomeDirectory();
};

#endif

#endif
