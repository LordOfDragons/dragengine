/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOSWEBWASM_H_
#define _DEOSWEBWASM_H_

#include "../dragengine_configuration.h"

#ifdef OS_WEBWASM

#include "deOS.h"
#include "../common/string/decString.h"
#include "../common/math/decMath.h"


/**
 * \brief Web WASM operating system.
 */
class deOSWebWasm : public deOS{
public:
	/** \brief Application configuration. */
	struct sConfig{
		/** \brief Canvas identifier in the form "#<id>". */
		decString canvasId;
		
		/** \brief Size of canvas. */
		decPoint canvasSize;
	};
	
	
private:
	const sConfig pConfig;
	
	int pScreenWidth, pScreenHeight, pScreenRefreshRate;
	int pScaleFactor;
	void *pCurWindow, *pHostingMainWindow, *pHostingRenderWindow;
	bool pAppFrozen;
	decBoundary pContentRect;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new Web WASM operating system object. */
	deOSWebWasm(const sConfig &config);
	
	/** \brief Clean up the Web WASM operating system object. */
	~deOSWebWasm() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine path. */
	decString GetPathEngine() override;
	
	/** \brief Share path. */
	decString GetPathShare() override;
	
	/** \brief System configuration path. */
	decString GetPathSystemConfig() override;
	
	/** \brief User configuration path. */
	decString GetPathUserConfig() override;
	
	/** \brief User cache path. */
	decString GetPathUserCache() override;
	
	/** \brief User capture path. */
	decString GetPathUserCapture() override;
	
	/** \brief Process all events in the application event queue. */
	void ProcessEventLoop(bool sendToInputModule) override;
	
	/** \brief Canvas identifier. */
	inline const decString &GetCanvasId() const{ return pConfig.canvasId; }
	
	/** \brief Canvas size. */
	inline const decPoint &GetCanvasSize() const{ return pConfig.canvasSize; }
	/*@}*/
	
	
	
	/** \name Display information. */
	/*@{*/
	/** \brief Number of displays. */
	int GetDisplayCount() override;
	
	/**
	 * \brief Current resolution of display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	decPoint GetDisplayCurrentResolution(int display) override;
	
	/**
	 * \brief Current refresh rate of display.
	 * \param display Index of display to get current refresh rate for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	int GetDisplayCurrentRefreshRate(int display) override;
	
	/**
	 * \brief Number of resolutions supported on display.
	 * \param display Index of display to get current resolution for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater
	 * than GetDisplayCount().
	 */
	int GetDisplayResolutionCount(int display) override;
	
	/**
	 * \brief Resolution by index for display.
	 * \param display Index of display to get current resolution for.
	 * \param resolution Index of resolution on display.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * \throws deeInvalidParam \em resolution is less than 0 or equal to or greater than
	 *                         GetDisplayResolutionCount(display).
	 */
	decPoint GetDisplayResolution(int display, int resolution) override;
	
	/**
	 * \brief Current user locale language (ISO 639 language code) in lower case.
	 * \version 1.16
	 */
	decString GetUserLocaleLanguage() override;
	
	/**
	 * \brief Current user locale territory (ISO 3166 country code) lower case or empty string.
	 * \version 1.16
	 */
	decString GetUserLocaleTerritory() override;
	
	/**
	 * \brief Current global scaling factor for display.
	 * \version 1.20.
	 * \param display Index of display to get scaling factor for.
	 * \throws deeInvalidParam \em display is less than 0 or equal to or greater than
	 *                         GetDisplayCount().
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	int GetDisplayCurrentScaleFactor(int display) override;
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	/**
	 * \brief Cast to deOSWebWasm.
	 * \throws deeInvalidParam Not an instance of deOSWebWasm.
	 */
	deOSWebWasm *CastToOSWebWasm() override;
	/*@}*/
	
	
	
	/** \name Web WASM related */
	/*@{*/
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
	void SetWindow(void *window);
	
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
	void SetHostingMainWindow(void *window);
	
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
	void SetHostingRenderWindow(void *window);
	
	/** \brief Determine if a hosting main window is set. */
	bool HasHostingMainWindow() const;
	
	/** \brief Determine if a hosting render window is set. */
	bool HasHostingRenderWindow() const;
	
	/** \brief Application is frozen. */
	inline bool GetAppFrozen() const{ return pAppFrozen; }
	
	/** \brief Set if application is frozen. */
	void SetAppFrozen(bool frozen);
	
	/** \brief Content area. */
	inline const decBoundary &GetContentRect() const{ return pContentRect; }
	
	/** \brief Set content area. */
	void SetContentRect(const decBoundary &rect);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetOSParameters();
};

#endif

#endif
