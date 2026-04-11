/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _MEPREVIEWCAMERA_H_
#define _MEPREVIEWCAMERA_H_

#include "mePreviewCameraCamera.h"
#include "mePreviewCameraListener.h"
#include "../../world/meWorld.h"
#include "../../world/object/meObject.h"

#include <deigde/gui/resources/igdeFont.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasView.h>

class meWindowMain;


/**
 * Preview selected cameras.
 */
class mePreviewCamera : public deObject{
public:
	using Ref = deTObjectReference<mePreviewCamera>;
	using CameraList = decTObjectOrderedSet<mePreviewCameraCamera>;
	
	
private:
	meWindowMain &pWindowMain;
	mePreviewCameraListener::Ref pListener;
	
	meWorld::Ref pWorld;
	
	igdeFont::Ref pFontLabel;
	
	decPoint pRenderWorldSize;
	
	deCanvasView::Ref pCanvasPanel;
	
	CameraList pCameras;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create preview camera. */
	mePreviewCamera(meWindowMain &windowMain, deCanvasView &container);
	
protected:
	/** Clean up preview camera. */
	~mePreviewCamera() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** World. */
	inline const meWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** Set world render. */
	void SetWorld(meWorld *world);
	
	/** Cameras. */
	inline const CameraList &GetCameras() const{ return pCameras; }
	
	/** Object is previewed. */
	bool HasObject(meObject *object) const;
	
	/** Panel. */
	inline const deCanvasView::Ref &GetPanel() const{ return pCanvasPanel; }
	
	/** Render world size. */
	inline const decPoint &GetRenderWorldSize() const{ return pRenderWorldSize; }
	
	/** Set render world size. */
	void SetRenderWorldSize(const decPoint &size);
	
	/** Label font. */
	inline const igdeFont::Ref &GetFontLabel() const{ return pFontLabel; }
	
	/** Rebuild cameras. */
	void RebuildCameras();
	
	/** Parent canvas has been resized. */
	void OnParentResized();
	
	/** Game like frame update. */
	void OnFrameUpdate(float elapsed);
	/*@}*/
	
	
private:
	void pCreateContent(deCanvasView &container);
};

#endif
