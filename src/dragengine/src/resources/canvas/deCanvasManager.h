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

#ifndef _DECANVASMANAGER_H_
#define _DECANVASMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deCanvas;
class deCanvasImage;
class deCanvasPaint;
class deCanvasCanvasView;
class deCanvasRenderWorld;
class deCanvasText;
class deCanvasVideoPlayer;
class deCanvasView;
class deEngine;


/**
 * \brief Canvas resource manager
 */
class DE_DLL_EXPORT deCanvasManager : public deResourceManager{
private:
	deResourceList pCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas resource manager. */
	deCanvasManager(deEngine *engine);
	
	/** \brief Clean up canvas resource manager and reports leaking resources. */
	~deCanvasManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of canvas. */
	int GetCanvasCount() const;
	
	/** \brief Root canvas resource for iteration purpose. */
	deCanvas *GetRootCanvas() const;
	
	/** \brief Create image canvas. */
	deCanvasImage::Ref CreateCanvasImage();
	
	/** \brief Create paint canvas. */
	deCanvasPaint *CreateCanvasPaint();
	
	/** \brief Create canvas view canvas. */
	deCanvasCanvasView *CreateCanvasCanvasView();
	
	/** \brief Create render world canvas. */
	deCanvasRenderWorld *CreateCanvasRenderWorld();
	
	/** \brief Create text canvas. */
	deCanvasText *CreateCanvasText();
	
	/** \brief Create video player canvas. */
	deCanvasVideoPlayer *CreateCanvasVideoPlayer();
	
	/** \brief Create view canvas. */
	deCanvasView *CreateCanvasView();
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic system peers of all stored resources have to be created. */
	void SystemGraphicLoad() override;
	
	/** \brief Graphic system peers of all stored resources have to be freed. */
	void SystemGraphicUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
