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

#ifndef _DECANVASVIEW_H_
#define _DECANVASVIEW_H_

#include "deCanvas.h"


/**
 * \brief Canvas containing other canvas.
 */
class DE_DLL_EXPORT deCanvasView : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasView> Ref;
	
	
private:
	deCanvas *pCanvasRoot;
	deCanvas *pCanvasTail;
	int pCanvasCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasView(deCanvasManager *manager);
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deCanvasView() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of child canvas. */
	inline int GetCanvasCount() const{ return pCanvasCount; }
	
	/** \brief Root canvas or NULL if there is none. */
	inline deCanvas *GetRootCanvas() const{ return pCanvasRoot; }
	
	/**
	 * \brief Add child canvas.
	 * \throws deeInvalidParam \em canvas is NULL.
	 * \throws deeInvalidParam \em canvas has a parent view.
	 */
	void AddCanvas(deCanvas *canvas);
	
	/**
	 * \brief Remove child canvas.
	 * \throws deeInvalidParam \em canvas is NULL.
	 * \throws deeInvalidParam \em canvas parent view is not this canvas.
	 */
	void RemoveCanvas(deCanvas *canvas);
	
	/** \brief Remove all child canvas. */
	void RemoveAllCanvas();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	void Visit(deCanvasVisitor &visitor) override;
	/*@}*/
};

#endif
