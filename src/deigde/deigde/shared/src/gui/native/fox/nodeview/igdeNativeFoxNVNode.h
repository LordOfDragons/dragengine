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
#include "../../../resources/igdeFontReference.h"


class igdeNVNode;
class igdeGuiTheme;


/**
 * \brief FOX toolkit Native NodeView Node.
 */
class igdeNativeFoxNVNode : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxNVNode )
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
	igdeFontReference pFont;
	
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
	igdeNativeFoxNVNode( igdeNVNode &owner, FXComposite *parent, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVNode();
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVNode* CreateNativeWidget( igdeNVNode &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	/** \brief Update title from owner. */
	virtual void UpdateTitle();
	
	/** \brief Update description from owner. */
	virtual void UpdateDescription();
	
	/** \brief Update enabled from owner. */
	virtual void UpdateEnabled();
	
	/** \brief Update active from owner. */
	virtual void UpdateActive();
	
	/** \brief Update color from owner. */
	virtual void UpdateColors();
	
	/** \brief Update position from owner. */
	virtual void UpdatePosition();
	
	/** \brief Resize node to fit size to content. */
	virtual void FitSizeToContent();
	
	/** \brief Get Size. */
	virtual decPoint GetSize();
	
	
	static int NVNodeFlags( const igdeNVNode &owner );
	static igdeFont *NVNodeFont( const igdeNVNode &owner, const igdeGuiTheme &guitheme );
	static int NVNodePadLeft( const igdeGuiTheme &guitheme );
	static int NVNodePadRight( const igdeGuiTheme &guitheme );
	static int NVNodePadTop( const igdeGuiTheme &guitheme );
	static int NVNodePadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onTitleLeftMouseDown( FXObject*, FXSelector, void* );
	long onTitleLeftMouseUp( FXObject*, FXSelector, void* );
	long onTitleMouseMove( FXObject*, FXSelector, void* );
	long onRightMousePress( FXObject*, FXSelector, void* );
	long onRightMouseRelease( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxNVNode igdeNativeNVNode;

#endif
