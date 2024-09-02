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

#ifndef _DEOGLDSRENDERABLECAMERA_H_
#define _DEOGLDSRENDERABLECAMERA_H_

#include "deoglDSRenderable.h"

class deoglRDSRenderableCamera;
class deoglCamera;

class deDSRenderableCamera;


/**
 * Dynamic skin camera renderable.
 */
class deoglDSRenderableCamera : public deoglDSRenderable{
public:
	const deDSRenderableCamera &pRenderableCamera;
	deoglRDSRenderableCamera *pRRenderableCamera;
	deoglCamera *pCamera;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable. */
	deoglDSRenderableCamera( deoglDynamicSkin &dynamicSkin, const deDSRenderableCamera &renderable );
	
	/** Clean up peer. */
	virtual ~deoglDSRenderableCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Renderable. */
	inline const deDSRenderableCamera &GetRenderableCamera() const{ return pRenderableCamera; }
	
	/** Render renderable. */
	virtual deoglRDSRenderable *GetRRenderable() const;
	
	/** Renderable changed. */
	virtual void RenderableChanged();
	
	/** Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/** Camera requires sync. */
	void CameraRequiresSync();
	
	/** Drop camera. */
	void DropCamera();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
