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
#include "../../igdeViewRenderWindow.h"


/**
 * Null Render View.
 */
class igdeNativeNullRenderView : public igdeViewRenderWindow::cNativeViewRenderWindow{
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
	
	void DropNativeWindow() override;
	bool IsReallyVisible() const override;
	bool IsShown() const override;
	decPoint GetSize() const override;
	void OnFrameUpdate() override;
	void AttachRenderWindow() override;
	void DetachRenderWindow() override;
	void GrabInput() override;
	void ReleaseInput() override;
	/*@}*/
};

typedef igdeNativeNullRenderView igdeNativeRenderView;

#endif
