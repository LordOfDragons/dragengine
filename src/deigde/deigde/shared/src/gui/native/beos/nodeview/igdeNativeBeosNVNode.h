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

#ifndef _IGDENATIBESOSNVNODE_H_
#define _IGDENATIBESOSNVNODE_H_

#include <interface/View.h>
#include "../../../nodeview/igdeNVNode.h"
#include "../../../resources/igdeFont.h"

class igdeNVNode;
class igdeGuiTheme;


/**
 * \brief BeOS native NodeView node widget.
 */
class igdeNativeBeosNVNode : public BView, public igdeNVNode::cNativeNVNode{
private:
	igdeNVNode *pOwner;
	igdeFont::Ref pFont;
	bool pTitleIsDragging;
	decPoint pTitleDragOffset;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosNVNode(igdeNVNode &owner, BView *parent, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosNVNode() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosNVNode* CreateNativeWidget(igdeNVNode &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner. */
	inline igdeNVNode &GetOwner() const{ return *pOwner; }
	
	
	
	void UpdateTitle() override;
	void UpdateDescription() override;
	void UpdateEnabled() override;
	void UpdateActive() override;
	void UpdateColors() override;
	void UpdatePosition() override;
	void FitSizeToContent() override;
	decPoint GetSize() override;
	
	
	
	static igdeFont *NVNodeFont(const igdeNVNode &owner, const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseMoved(BPoint where, uint32 code, const BMessage *message) override;
	void MouseUp(BPoint where) override;
	void FrameResized(float width, float height) override;
	/*@}*/
};

typedef igdeNativeBeosNVNode igdeNativeNVNode;

#endif
