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

#ifndef _IGDENATIVEFOXNVSLOT_H_
#define _IGDENATIVEFOXNVSLOT_H_

#include "../foxtoolkit.h"
#include "../../../resources/igdeFontReference.h"


class igdeNVSlot;
class igdeGuiTheme;
class igdeNativeFoxNVSocket;


/**
 * \brief FOX toolkit Native NodeView Slot.
 */
class igdeNativeFoxNVSlot : public FXHorizontalFrame{
	FXDECLARE( igdeNativeFoxNVSlot )
protected:
	   igdeNativeFoxNVSlot();
	
public:
	enum eFoxIDs{
		ID_SELF = FXHorizontalFrame::ID_LAST,
		ID_SOCKET,
		ID_LABEL,
		ID_LAST
	};
	
private:
	igdeNVSlot *pOwner;
	igdeFontReference pFont;
	igdeNativeFoxNVSocket *pSocket;
	FXLabel *pLabel;
	
	FXDragType pDragTypeSocket;
	bool pIsDnd;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVSlot( igdeNVSlot &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVSlot();
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVSlot* CreateNativeWidget( igdeNVSlot &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	/** \brief Owner. */
	inline igdeNVSlot &GetOwner() const{ return *pOwner; }
	
	
	
	/** \brief Update text from widget. */
	void UpdateText();
	
	/** \brief Update description from widget. */
	void UpdateDescription();
	
	/** \brief Update enabled from widget. */
	void UpdateEnabled();
	
	/** \brief Update color from widget. */
	void UpdateColor();
	
	/** \brief Update linked state from widget. */
	void UpdateLinkedState();
	
	
	
	/** \brief Center of slot in local window coordinates. */
	decPoint GetCenter() const;
	
	/** \brief Center of slot in owner node coordinates.. */
	decPoint GetCenterNode() const;
	
	/** \brief Center of slot in owner board coordinates.. */
	decPoint GetCenterBoard() const;
	
	/** \brief Connector position of slot in local window coordinates. */
	decPoint GetConnector() const;
	
	/** \brief Connector position of slot in owner node coordinates.. */
	decPoint GetConnectorNode() const;
	
	/** \brief Connector position of slot in owner board coordinates.. */
	decPoint GetConnectorBoard() const;
	
	
	
	static int NVSlotFlags( const igdeNVSlot &owner );
	static igdeFont *NVSlotFont( const igdeNVSlot &owner, const igdeGuiTheme &guitheme );
	static int NVSlotPadLeft( const igdeGuiTheme &guitheme );
	static int NVSlotPadRight( const igdeGuiTheme &guitheme );
	static int NVSlotPadTop( const igdeGuiTheme &guitheme );
	static int NVSlotPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	
	long onSocketCommand( FXObject*, FXSelector, void* );
	long onSocketLeftMouseDown( FXObject*, FXSelector, void* );
	long onSocketMouseMoved( FXObject*, FXSelector, void* );
	long onSocketLeftMouseUp( FXObject*, FXSelector, void* );
	long onSocketDndEnter( FXObject*, FXSelector, void* );
	long onSocketDndLeave( FXObject*, FXSelector, void* );
	long onSocketDndDrop( FXObject*, FXSelector, void* );
	long onSocketDndMotion( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxNVSlot igdeNativeNVSlot;

#endif
