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

#ifndef _IGDENATIVEFOXNVNODE_H_
#define _IGDENATIVEFOXNVNODE_H_

#include "../foxtoolkit.h"
#include "../../../nodeview/igdeNVNode.h"
#include "../../../resources/igdeFont.h"


class igdeNVNode;
class igdeGuiTheme;


/**
 * \brief FOX toolkit Native NodeView Node.
 */
class igdeNativeFoxNVNode : public FXVerticalFrame, public igdeNVNode::cNativeNVNode{
	FXDECLARE(igdeNativeFoxNVNode)
protected:
	   igdeNativeFoxNVNode();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_FRA_TITLE,
		ID_LAB_TITLE,
		ID_FRA_SLOTS,
		ID_LAST
	};
	
private:
	igdeNVNode *pOwner;
	igdeFont::Ref pFont;
	
	FXLabel *pLabTitle;
	FXHorizontalFrame *pFrameTitle;
	FXVerticalFrame *pFrameSlots;
	
	bool pTitleIsDraging;
	decPoint pTitleDragOffset;
	
	decPoint pMouseToNodeOffset;
	decPoint pDragOffset;
	bool pIsLDraging;
	bool pIsRDraging;
	bool pDragShift;
	bool pDragControl;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVNode(igdeNVNode &owner, FXComposite *parent, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeFoxNVNode() override;
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVNode* CreateNativeWidget(igdeNVNode &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	FXbool canFocus() const override;
	
	
	/** \brief Update title from owner. */
	void UpdateTitle() override;
	
	/** \brief Update description from owner. */
	void UpdateDescription() override;
	
	/** \brief Update enabled from owner. */
	void UpdateEnabled() override;
	
	/** \brief Update active from owner. */
	void UpdateActive() override;
	
	/** \brief Update color from owner. */
	void UpdateColors() override;
	
	/** \brief Update position from owner. */
	void UpdatePosition() override;
	
	/** \brief Resize node to fit size to content. */
	void FitSizeToContent() override;
	
	/** \brief Get Size. */
	decPoint GetSize() override;
	
	
	static int NVNodeFlags(const igdeNVNode &owner);
	static igdeFont *NVNodeFont(const igdeNVNode &owner, const igdeGuiTheme &guitheme);
	static int NVNodePadLeft(const igdeGuiTheme &guitheme);
	static int NVNodePadRight(const igdeGuiTheme &guitheme);
	static int NVNodePadTop(const igdeGuiTheme &guitheme);
	static int NVNodePadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onTitleLeftMouseDown(FXObject*, FXSelector, void*);
	long onTitleLeftMouseUp(FXObject*, FXSelector, void*);
	long onTitleMouseMove(FXObject*, FXSelector, void*);
	long onRightMousePress(FXObject*, FXSelector, void*);
	long onRightMouseRelease(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
	/*@}*/
};

using igdeNativeNVNode = igdeNativeFoxNVNode;

#endif
