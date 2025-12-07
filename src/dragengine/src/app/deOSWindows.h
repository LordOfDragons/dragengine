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

#ifndef _DEOSWINDOWS_H_
#define _DEOSWINDOWS_H_

#include "../dragengine_configuration.h"

#ifdef OS_W32

#include <wchar.h>

#include "include_windows.h"
#include "deOS.h"
#include "../common/string/decString.h"
#include "../common/string/unicode/decUnicodeString.h"


/**
 * \brief Windows Operating System.
 */
class DE_DLL_EXPORT deOSWindows : public deOS{
private:
	HINSTANCE pInstApp;
	int pScreenWidth;
	int pScreenHeight;
	HWND pCurWindow;
	int pResolutionCount;
	decPoint* pResolutions;
	int pRefreshRate;
	int pScaleFactor;
	
	decString pPathEngine;
	decString pPathEngineBase;
	decString pPathShare;
	decString pPathSystemConfig;
	decString pPathUserConfig;
	decString pPathUserCache;
	decString pPathUserCapture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new windows operating system object. */
	deOSWindows();
	
	/** \brief Clean up the windows operating system object. */
	virtual ~deOSWindows();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine path. */
	virtual decString GetPathEngine();
	
	/** \brief Engine path. */
	inline const decString &GetPathEngineBase() const{ return pPathEngineBase; }
	
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
	virtual void ProcessEventLoop(bool sendToInputModule);
	
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
	virtual int GetDisplayCurrentScaleFactor(int display);
	/*@}*/
	
	
	
	/** \name Display information. */
	/*@{*/
	/** \brief Number of displays. */
	virtual int GetDisplayCount();
	
	/**
	 * \brief Current refresh rate of display.
	 * \param display Index of display to get current refresh rate for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 */
	virtual int GetDisplayCurrentRefreshRate(int display);
	
	/**
	 * \brief Current resolution of display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 */
	virtual decPoint GetDisplayCurrentResolution(int display);
	
	/**
	 * \brief Number of resolutions supported on display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 */
	virtual int GetDisplayResolutionCount(int display);
	
	/**
	 * \brief Resolution by index for display.
	 * \param display Index of display to get current resolution for.
	 * \param resolution Index of resolution on display.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * \throws deeInvalidParam \em resolution is less than 0 or equal to or greater than
	 *                         GetDisplayResolutionCount(display).
	 */
	virtual decPoint GetDisplayResolution(int display, int resolution);
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/**
	 * \brief Cast to deOSWindows.
	 * \throws deeInvalidParam Not an instance of deOSWindows.
	 */
	virtual deOSWindows *CastToOSWindows();
	/*@}*/
	
	
	
	/** \name Windows related */
	/*@{*/
	/** \brief Application instance handle. */
	inline HINSTANCE GetInstApp() const{ return pInstApp; }
	
	/**
	 * \brief Set application instance handle.
	 * 
	 * This value has to be set by the launcher application.
	 */
	void SetInstApp(HINSTANCE hInstApp);
	
	/**
	 * \brief Current game window.
	 * 
	 * This is usually the window created by the graphic module but can also
	 * be some other window. Only one window can be the application window.
	 * If you need more windows than one parent it to the window you set.
	 */
	inline HWND GetWindow() const{ return pCurWindow; }
	
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
	void SetWindow(HWND window);
	
	/**
	 * \brief Parse path replacing special path indicators.
	 * 
	 * Replaces the following special path indicators:
	 * <table>
	 * <thead>
	 * <td>Special</td>
	 * <td>Description</td></td>
	 * </thead>
	 * <tr>
	 * <td>@ProgramFiles</td>
	 * <td>The Program Files folder. A typical path is C:\Program Files.</td>
	 * </tr>
	 * <tr>
	 * <td>@System</td>
	 * <td>The Windows System folder. A typical path is C:\Windows\System32.</td>
	 * </tr>
	 * <tr>
	 * <td>@RoamingAppData</td>
	 * <td>The file system directory that serves as a common repository for
	 * application-specific data. A typical path is C:\Documents and
	 * Settings\username\Application Data.</td>
	 * </tr>
	 * <tr>
	 * <td>@ProgramData</td>
	 * <td>The file system directory that contains application data for all users.
	 * A typical path is C:\Documents and Settings\All Users\Application Data.
	 * This folder is used for application data that is not user specific. For
	 * example, an application can store a spell-check dictionary, a database
	 * of clip art, or a log file in the $ProgramData folder. This information
	 * will not roam and is available to anyone using the computer.</td>
	 * </tr>
	 * <tr>
	 * <td>@PublicDocuments</td>
	 * <td>The file system directory that contains documents that are common to all
	 * users. A typical path is C:\Documents and Settings\All Users\Documents.</td>
	 * </tr>
	 * <tr>
	 * <td>@LocalAppData</td>
	 * <td>The file system directory that serves as a data repository for local
	 * (nonroaming) applications. A typical path is C:\Documents and
	 * Settings\username\Local Settings\Application Data.</td>
	 * </tr>
	 * <tr>
	 * <td>@Documents</td>
	 * <td>The virtual folder that represents the My Documents desktop item. A typical
	 * path is C:\Documents and Settings\username\My Documents.</td>
	 * </tr>
	 * <tr>
	 * <td>@Windows</td>
	 * <td>The Windows directory or $System. This corresponds to the %windir% or
	 * %SYSTEMROOT% environment variables. A typical path is C:\Windows.</td>
	 * </tr>
	 * </table>
	 * 
	 * Only one special path indicator can be used and it has to be the first component
	 * in the path.
	 */
	static decString ParseNativePath(const char *path);
	
	/** \brief Convert unicode to wide string. */
	static void UnicodeToWide(const decUnicodeString &unicode, wchar_t *wide, int maxSize);
	
	/** \brief Convert utf8 to wide string. */
	static void Utf8ToWide(const char *utf8, wchar_t *wide, int maxSize);
	
	/** \brief Convert utf8 from wide string. */
	static decString WideToUtf8(const wchar_t *wide);
	
	/** \brief Convert unicode from wide string. */
	static decUnicodeString WideToUnicode(const wchar_t *wide);
	
	/**
	 * \brief Get registry value.
	 * 
	 * If not present returns default value.
	 */
	static decString GetRegistryValue(const char *key,
		const char *entry, const char *defaultValue);
	
	static decString GetRegistryValueCurrentUser(const char *key,
		const char *entry, const char *defaultValue);
	
	/** \brief Set registry value. */
	static void SetRegistryValue(const char *key,
		const char *entry, const char *value);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	decString pGetUserLanguage() const;
	void pFindResolutions();
	void pFindScaleFactor();
	void pFindRefreshRate();
};

#endif

#endif
