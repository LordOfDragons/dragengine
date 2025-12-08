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

#ifndef _DERENDERWINDOWMANAGER_H_
#define _DERENDERWINDOWMANAGER_H_ 

#include "../../dragengine_configuration.h"

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deRenderWindow;


/**
 * \brief Render Window Resource Manager.
 */
class DE_DLL_EXPORT deRenderWindowManager : public deResourceManager{
private:
	deResourceList pRenderWindows;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render window resource manager. */
	deRenderWindowManager(deEngine *engine);
	
	/** \brief Clean up render window resource manager and reports leaking resources. */
	~deRenderWindowManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of render windows. */
	int GetRenderWindowCount() const;
	
	/** \brief Root render window resource. */
	deRenderWindow *GetRootRenderWindow() const;
	
	/** \brief Create top level render window. */
	deRenderWindow::Ref CreateRenderWindow();
	
	/** \brief Create render window inside a host window. */
	#ifdef OS_ANDROID
	deRenderWindow::Ref CreateRenderWindowInside(void *window);
	
	#elif defined OS_WEBWASM
	deRenderWindow::Ref CreateRenderWindowInside(void *window);
	
	#elif defined OS_BEOS
	deRenderWindow::Ref CreateRenderWindowInside(BWindow *window);
	
	#elif defined OS_UNIX_X11
	deRenderWindow::Ref CreateRenderWindowInside(Window window);
	
	#elif defined OS_W32
	deRenderWindow::Ref CreateRenderWindowInside(HWND window);
	#endif
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	void SystemGraphicLoad() override;
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	void SystemGraphicUnload() override;
	/*@}*/
	
	
	
	/** \name Resource only Functions */
	/*@{*/
	/**
	 * \brief Remove resource.
	 * 
	 * For use by deRenderWindow only.
	 */
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
