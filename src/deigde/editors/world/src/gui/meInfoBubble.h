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

#ifndef _MEINFOBUBBLE_H_
#define _MEINFOBUBBLE_H_

#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>

class meView3D;
class igdeWidget;


/**
 * Information bubble.
 */
class meInfoBubble : public deObject{
public:
	typedef deTObjectReference<meInfoBubble> Ref;
	
	enum ePlacement{
		epTopLeft,
		epTopRight,
		epBottomLeft,
		epBottomRight
	};
	
	
	
private:
	meView3D &pView;
	
	deCanvasView::Ref pCanvasBubble;
	deCanvasView::Ref pCanvasContent;
	deCanvasPaint::Ref pCanvasBorder;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create information bubble. */
	meInfoBubble( meView3D &view );
	
protected:
	/** \brief Clean up information bubble. */
	virtual ~meInfoBubble();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Content view. */
	inline const deCanvasView::Ref &GetCanvasContent() const{ return pCanvasContent; }
	
	/** \brief Show bubble at mouse position. */
	void ShowAt( const decPoint &position, ePlacement placement );
	
	/** \brief Hide bubble. */
	void Hide();
	/*@}*/
	
	
	
private:
	void pResize();
	decPoint pPlace( const decPoint &position, ePlacement placement );
};

#endif
