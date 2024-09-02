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

#ifndef _DEOS_H_
#define _DEOS_H_

#include "../common/math/decMath.h"

class deEngine;
class deOSUnix;
class deOSConsole;
class deOSWindows;
class deOSBeOS;
class deOSAndroid;
class deOSMacOS;
class decString;


/**
 * \brief Interface for Operating System classes.
 */
class DE_DLL_EXPORT deOS{
private:
	deEngine *pEngine;
	bool pAppActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new operating system object. */
	deOS();
	
	/** \brief Clean up the operating system object. */
	virtual ~deOS();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine object or NULL if not existing. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Set engine object or NULL if not existing. */
	void SetEngine( deEngine * engine );
	
	/** \brief Engine path. */
	virtual decString GetPathEngine() = 0;
	
	/** \brief Share path. */
	virtual decString GetPathShare() = 0;
	
	/** \brief System configuration path. */
	virtual decString GetPathSystemConfig() = 0;
	
	/** \brief User configuration path. */
	virtual decString GetPathUserConfig() = 0;
	
	/** \brief User cache path. */
	virtual decString GetPathUserCache() = 0;
	
	/** \brief User capture path. */
	virtual decString GetPathUserCapture() = 0;
	
	/**
	 * \brief Process all events in the application event queue.
	 * 
	 * Certain events are directly processed. If \em sendToInputModule is true
	 * all events are also send to the active input module. Set \em sendToInputModule
	 * to false if you want to clear the event queue after a lengthy operation to
	 * avoid an event flood resulting in strange initial inputs.
	 */
	virtual void ProcessEventLoop( bool sendToInputModule ) = 0;
	
	/** \brief Application is active. */
	inline bool GetAppActive() const{ return pAppActive; }
	
	/**
	 * \brief Set if application is active.
	 * 
	 * Sends notifications to affected engine modules if required.
	 */
	virtual void SetAppActive( bool active );
	
	/**
	 * \brief Current user locale language (ISO 639 language code) in lower case.
	 * \version 1.16
	 */
	virtual decString GetUserLocaleLanguage() = 0;
	
	/**
	 * \brief Current user locale territory (ISO 3166 country code) in lower case or empty string.
	 * \version 1.16
	 */
	virtual decString GetUserLocaleTerritory() = 0;
	/*@}*/
	
	
	
	/** \name Display information. */
	/*@{*/
	/** \brief Number of displays. */
	virtual int GetDisplayCount() = 0;
	
	/**
	 * \brief Current resolution of display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual decPoint GetDisplayCurrentResolution( int display ) = 0;
	
	/**
	 * \brief Current refresh rate of display.
	 * \param display Index of display to get current refresh rate for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual int GetDisplayCurrentRefreshRate( int display ) = 0;
	
	/**
	 * \brief Number of resolutions supported on display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	virtual int GetDisplayResolutionCount( int display ) = 0;
	
	/**
	 * \brief Resolution by index for display.
	 * \param display Index of display to get current resolution for.
	 * \param resolution Index of resolution on display.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * \throws deeInvalidParam \em resolution is less than 0 or equal to or greater than
	 *                         GetDisplayResolutionCount(display).
	 */
	virtual decPoint GetDisplayResolution( int display, int resolution ) = 0;
	
	/**
	 * \brief Current global scaling factor for display.
	 * \version 1.20.
	 * \param display Index of display to get scaling factor for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	virtual int GetDisplayCurrentScaleFactor( int display ) = 0;
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/**
	 * \brief Cast to deOSUnix.
	 * \throws deeInvalidParam Not an instance of deOSUnix.
	 */
	virtual deOSUnix *CastToOSUnix();
	
	/**
	 * \brief Cast to deOSConsole.
	 * \throws deeInvalidParam Not an instance of deOSConsole.
	 */
	virtual deOSConsole *CastToOSConsole();
	
	/**
	 * \brief Cast to deOSWindows.
	 * \throws deeInvalidParam Not an instance of deOSWindows.
	 */
	virtual deOSWindows *CastToOSWindows();
	
	/**
	 * \brief Cast to deOSBeOS.
	 * \throws deeInvalidParam Not an instance of deOSBeOS.
	 */
	virtual deOSBeOS *CastToOSBeOS();
	
	/**
	 * \brief Cast to deOSAndroid.
	 * \throws deeInvalidParam Not an instance of deOSAndroid.
	 */
	virtual deOSAndroid *CastToOSAndroid();
	
	/**
	 * \brief Cast to deOSMacOS.
	 * \throws deeInvalidParam Not an instance of deOSMacOS.
	 */
	virtual deOSMacOS *CastToOSMacOS();
	/*@}*/
};

#endif
