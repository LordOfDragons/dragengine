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

#ifndef _MEPREVIEWCAMERACAMERA_H_
#define _MEPREVIEWCAMERACAMERA_H_

#include "../../world/object/meObject.h"

#include <dragengine/common/collection/decTSet.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasView.h>

class mePreviewCamera;
class deComponent;


/**
 * Preview selected cameras.
 */
class mePreviewCameraCamera : public deObject{
public:
	using Ref = deTObjectReference<mePreviewCameraCamera>;
	
private:
	mePreviewCamera &pPreviewCamera;
	meObject::Ref pObject;
	
	deCanvasView::Ref pCanvasPanel;
	deCanvasRenderWorld::Ref pCanvasRenderWorld;
	deCanvasPaint::Ref pCanvasBorder;
	deCanvasPaint::Ref pCanvasLabelBg;
	deCanvasText::Ref pCanvasLabel;
	
	decTObjectSet<deComponent> pIgnoreComponents;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view. */
	mePreviewCameraCamera(mePreviewCamera &previewCamera, meObject *object);
	
protected:
	/** Clean up view. */
	~mePreviewCameraCamera() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Object or nullptr if not set. */
	inline const meObject::Ref &GetObject() const{ return pObject; }
	
	/** Update. */
	void Update(float elapsed);
	
	/** Panel. */
	inline const deCanvasView::Ref &GetPanel() const{ return pCanvasPanel; }
	/*@}*/
	
	
private:
	void pCreateContent();
	void pUpdateIgnoreResources();
};

#endif
