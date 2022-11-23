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

#ifndef _DEOSUNIX_H_
#define _DEOSUNIX_H_

#include "../dragengine_configuration.h"

#if defined OS_UNIX && defined HAS_LIB_X11

#include "deOS.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>


/**
 * \brief Unix operating system.
 */
class deOSUnix : public deOS{
private:
	struct sDisplayInformation{
		decPoint currentResolution;
		int currentRefreshRate;
		decPoint *resolutions;
		int resolutionCount;
	};
	
	Display *pDisplay;
	int pScreen;
	Window pCurWindow;
	long pEventMask;
	Window pHostingMainWindow;
	Window pHostingRenderWindow;
	
	sDisplayInformation *pDisplayInformation;
	int pDisplayCount;
	decPoint *pDisplayResolutions;
	int pDisplayResolutionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new unix operating system object. */
	deOSUnix();
	
	/** \brief Clean up the unix operating system object. */
	virtual ~deOSUnix();
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
	 * \brief Cast to deOSUnix.
	 * \throws deeInvalidParam Not an instance of deOSUnix.
	 */
	virtual deOSUnix *CastToOSUnix();
	/*@}*/
	
	
	
	/** \name Unix related */
	/*@{*/
	/** \brief Display used during creation. */
	inline Display *GetDisplay() const{ return pDisplay; }
	
	/** \brief Screen used during creation. */
	inline int GetScreen() const{ return pScreen; }
	
	/**
	 * \brief Current game window.
	 * 
	 * This is usually the window created by the graphic module but can also
	 * be some other window. Only one window can be the application window.
	 * If you need more windows than one parent it to the window you set.
	 */
	inline Window GetWindow() const{ return pCurWindow; }
	
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
	void SetWindow( Window wnd );
	
	/**
	 * \brief Window event mask.
	 * 
	 * Set by the input module to receive the events it is interested in.
	 */
	inline long GetEventMask() const{ return pEventMask; }
	
	/**
	 * \brief Set event mask to set for all windows.
	 * 
	 * The input module sets this value to receive the required events.
	 */
	void SetEventMask( long mask );
	
	/** \brief Hosting main window or 0 if not set. */
	inline Window GetHostingMainWindow() const{ return pHostingMainWindow; }
	
	/**
	 * \brief Set hosting main window or 0 if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting main window is the main
	 * window of the hosting application that can be used to obtain visuals
	 * from. It is not necessary the one rendered into later on.
	 */
	void SetHostingMainWindow( Window window );
	
	/** \brief Hosting render window or 0 if not set. */
	inline Window GetHostingRenderWindow() const{ return pHostingRenderWindow; }
	
	/**
	 * \brief Set hosting render window or 0 if not set.
	 * 
	 * Hosting windows are useful to render into a specific application window
	 * instead of the default game window. The hosting render window is the
	 * window on which the graphic module has to render from now on. This window
	 * can be the same as the hosting main window but can also be different from it.
	 */
	void SetHostingRenderWindow( Window window );
	
	/** \brief Determine if a hosting main window is set. */
	bool HasHostingMainWindow() const;
	
	/** \brief Determine if a hosting render window is set. */
	bool HasHostingRenderWindow() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetWindowEventMask();
	decString pGetHomeDirectory();
	void pGetDisplayInformation();
};

#endif

#endif
