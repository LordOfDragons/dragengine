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

#ifndef _IGDENATIVENULLRENDERVIEW_H_
#define _IGDENATIVENULLRENDERVIEW_H_

#include <dragengine/common/math/decMath.h>

class igdeViewRenderWindow;


/**
 * Null Render View.
 */
class igdeNativeNullRenderView{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeNativeNullRenderView();
	
	/** \brief Clean up widget. */
	virtual ~igdeNativeNullRenderView();
	
	/** \brief Create native widget. */
	static igdeNativeNullRenderView* CreateNativeWidget(igdeViewRenderWindow &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline bool GetCanRender() const{ return true; }
	inline bool GetRenderWindowAttached() const{ return true; }
	
	virtual void DropNativeWindow();
	virtual bool IsReallyVisible() const;
	virtual bool IsShown() const;
	virtual decPoint GetSize() const;
	virtual void OnFrameUpdate();
	virtual void AttachRenderWindow();
	virtual void DetachRenderWindow();
	virtual void GrabInput();
	virtual void ReleaseInput();
	/*@}*/
};

typedef igdeNativeNullRenderView igdeNativeRenderView;

#endif
