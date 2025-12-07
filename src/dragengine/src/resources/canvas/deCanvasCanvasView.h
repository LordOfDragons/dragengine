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

#ifndef _DECANVASCANVASVIEW_H_
#define _DECANVASCANVASVIEW_H_

#include "deCanvas.h"
#include "deCanvasView.h"


/**
 * \brief Canvas rendering a shared canvas view.
 * 
 * The Graphic Module takes care of rendering the canvas view once if
 * changed for all deCanvasCanvasView using the same canvas view as
 * well as managing the life-time of the rendered content. This is
 * the only allowed way to reuse a canvas view in different places.
 */
class DE_DLL_EXPORT deCanvasCanvasView : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasCanvasView> Ref;
	
	
private:
	deCanvasView::Ref pCanvasView;
	int pRepeatX;
	int pRepeatY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasCanvasView(deCanvasManager *manager);
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deCanvasCanvasView() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Canvas view or NULL if not set. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Set canvas view or NULL to unset. */
	void SetCanvasView(deCanvasView *canvasView);
	
	/** \brief Repeat count along X axis. */
	inline int GetRepeatX() const{ return pRepeatX; }
	
	/**
	 * \brief Set repeat count along X axis.
	 * \throws deeInvalidParam \em count is less than 1.
	 */
	void SetRepeatX(int count);
	
	/** \brief Repeat count along Y axis. */
	inline int GetRepeatY() const{ return pRepeatY; }
	
	/**
	 * \brief Set repeat count along Y axis.
	 * \throws deeInvalidParam \em count is less than 1.
	 */
	void SetRepeatY(int count);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	void Visit(deCanvasVisitor &visitor) override;
	/*@}*/
};

#endif
