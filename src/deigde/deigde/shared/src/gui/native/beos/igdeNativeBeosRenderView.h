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

#ifndef _IGDENATIVEBEOSRENDERVIEW_H_
#define _IGDENATIVEBEOSRENDERVIEW_H_

#include <interface/View.h>
#include "../../igdeViewRenderWindow.h"

class igdeRenderView;


/**
 * \brief Native render view implementation for BeOS/Haiku.
 * 
 * Provides a rendering surface for 3D content within the IGDE. Handles window
 * attachment/detachment for render windows and manages input events, visibility,
 * and rendering callbacks.
 */
class igdeNativeBeosRenderView : public BView, public igdeViewRenderWindow::cNativeViewRenderWindow {
private:
	igdeRenderView *pOwner;
	igdeViewRenderWindow *pRenderWindowOwner;
	void *pRenderContext;
	int pViewportWidth;
	int pViewportHeight;
	bool pCanAttachRenderWindow;
	bool pRenderWindowAttached;
	bool pCanRender;
	bool pErrorRenderWindow;
	
	// Input tracking
	uint32 pMouseButtons;
	bool pMouseInside;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	igdeNativeBeosRenderView(igdeViewRenderWindow &owner, BView *parent);
	virtual ~igdeNativeBeosRenderView() override;
	
	static igdeNativeBeosRenderView* CreateNativeWidget(igdeViewRenderWindow &owner);
	static void PostCreateNativeWidget(igdeViewRenderWindow &owner, void *native);
	static void DestroyNativeWidget(igdeViewRenderWindow &owner, void *native);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	bool IsFocusable() const override;
	
	inline bool GetCanRender() const { return pCanRender; }
	inline bool GetRenderWindowAttached() const { return pRenderWindowAttached; }
	
	void DropNativeWindow() override;
	bool IsReallyVisible() const override;
	bool IsShown() const override;
	decPoint GetSize() const override;
	void OnFrameUpdate() override;
	
	void AttachRenderWindow() override;
	void DetachRenderWindow() override;
	
	void DrawEngineUnavailable(BView *dc);
	void DrawErrorRenderWindow(BView *dc);
	void DrawErrorRendering(BView *dc);
	
	void GrabInput() override;
	void ReleaseInput() override;
	
	// Rendering management
	void *GetRenderContext();
	void SetViewSize(int width, int height);
	void HandleRedraw();
	void InitializeRenderContext();
	/*@}*/
	
	/** \name Event Handlers */
	/*@{*/
	virtual void Draw(BRect updateRect) override;
	virtual void MouseDown(BPoint where) override;
	virtual void MouseUp(BPoint where) override;
	virtual void MouseMoved(BPoint where, uint32 code, const BMessage *message) override;
	virtual void KeyDown(const char *bytes, int32 numBytes) override;
	virtual void KeyUp(const char *bytes, int32 numBytes) override;
	virtual void FrameResized(float width, float height) override;
	virtual void AttachedToWindow() override;
	virtual void DetachedFromWindow() override;
	virtual void WindowActivated(bool active) override;
	/*@}*/

private:
	void pProcessRenderWindowAttachment();
	void pSyncSizes();
};

typedef igdeNativeBeosRenderView igdeNativeRenderView;

#endif
