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

#ifndef _DECANVASVISITOR_H_
#define _DECANVASVISITOR_H_

#include "../../dragengine_export.h"

class deCanvas;
class deCanvasImage;
class deCanvasPaint;
class deCanvasCanvasView;
class deCanvasRenderWorld;
class deCanvasText;
class deCanvasVideoPlayer;
class deCanvasView;


/**
 * \brief Canvas visitor.
 */
class DE_DLL_EXPORT deCanvasVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas visitor. */
	deCanvasVisitor();
	
	/** \brief Clean up canvas visitor. */
	virtual ~deCanvasVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void VisitCanvas(deCanvas &canvas);
	
	/** \brief Visit image canvas. */
	virtual void VisitImage(deCanvasImage &canvas);
	
	/** \brief Visit paint canvas. */
	virtual void VisitPaint(deCanvasPaint &canvas);
	
	/** \brief Visit canvas view canvas. */
	virtual void VisitCanvasView(deCanvasCanvasView &canvas);
	
	/** \brief Visit render world canvas. */
	virtual void VisitRenderWorld(deCanvasRenderWorld &canvas);
	
	/** \brief Visit text canvas. */
	virtual void VisitText(deCanvasText &canvas);
	
	/** \brief Visit video player canvas. */
	virtual void VisitVideoPlayer(deCanvasVideoPlayer &canvas);
	
	/** \brief Visit view canvas. */
	virtual void VisitView(deCanvasView &canvas);
	/*@}*/
};

#endif
