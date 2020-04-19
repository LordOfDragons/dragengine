/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	/** \brief Update title from owner. */
	void UpdateTitle();
	
	/** \brief Update description from owner. */
	void UpdateDescription();
	
	/** \brief Update enabled from owner. */
	void UpdateEnabled();
	
	/** \brief Update active from owner. */
	void UpdateActive();
	
	/** \brief Update color from owner. */
	void UpdateColors();
	
	/** \brief Update position from owner. */
	void UpdatePosition();
	
	/** \brief Resize node to fit size to content. */
	void FitSizeToContent();
	
	
	
	static int NVNodeFlags( const igdeNVNode &owner );
	static igdeFont *NVNodeFont( const igdeNVNode &owner, const igdeGuiTheme &guitheme );
	static int NVNodePadLeft( const igdeGuiTheme &guitheme );
	static int NVNodePadRight( const igdeGuiTheme &guitheme );
	static int NVNodePadTop( const igdeGuiTheme &guitheme );
	static int NVNodePadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onTitleLeftMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onTitleLeftMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onTitleMouseMove( FXObject *sender, FXSelector selector, void *data );
	long onRightMousePress( FXObject *sender, FXSelector selector, void *data );
	long onRightMouseRelease( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
