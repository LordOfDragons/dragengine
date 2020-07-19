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

#ifndef _IGDENATIVEFOXNVBOARD_H_
#define _IGDENATIVEFOXNVBOARD_H_

#include "../foxtoolkit.h"
#include "../../../resources/igdeFontReference.h"
#include "../../../nodeview/igdeNVLinkReference.h"


class igdeNVBoard;
class igdeNVLink;
class igdeGuiTheme;
class igdeNativeFoxNVSlot;


/**
 * \brief FOX toolkit Native NodeView Board.
 */
class igdeNativeFoxNVBoard : public FXPacker{
	FXDECLARE( igdeNativeFoxNVBoard )
protected:
	   igdeNativeFoxNVBoard();
	
public:
	enum eFoxIDs{
		ID_SELF = FXPacker::ID_LAST,
		ID_LAST
	};
	
private:
	igdeNVBoard *pOwner;
	FXImage *pDoubleBuffer;
	
	igdeNativeFoxNVSlot *pCreateLinkSource;
	igdeNativeFoxNVSlot *pCreateLinkTarget;
	decPoint pCreateLinkPosition;
	
	igdeNVLinkReference pHoverLink;
	
	decPoint pDragBoard;
	bool pIsDragBoard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxNVBoard( igdeNVBoard &owner, FXComposite *parent, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxNVBoard();
	
	/** \brief Create native widget. */
	static igdeNativeFoxNVBoard* CreateNativeWidget( igdeNVBoard &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	
	
	/** \brief Owner. */
	inline igdeNVBoard &GetOwner() const{ return *pOwner; }
	
	
	
	/** \brief Update enabled. */
	void UpdateEnabled();
	
	/** \brief Update color from owner. */
	void UpdateColors();
	
	/** \brief Update nodes from owner. */
	void UpdateNodes();
	
	/** \brief Update links from owner. */
	void UpdateLinks();
	
	/** \brief Update offset from owner. */
	void UpdateOffset();
	
	/** \brief Get Size. */
	virtual decPoint GetSize();
	
	
	
	/** \brief Begin create link drag and drop. */
	void BeginCreateLink( igdeNativeFoxNVSlot *source );
	
	/** \brief Link drag and drop source or NULL. */
	inline igdeNativeFoxNVSlot *GetCreateLinkSource() const{ return pCreateLinkSource; }
	
	/** \brief Set create link drag and drop position. */
	void SetCreateLinkPosition( const decPoint &position );
	
	/** \brief Create link drag and drop target or NULL. */
	inline igdeNativeFoxNVSlot *GetCreateLinkTarget() const{ return pCreateLinkTarget; }
	
	/** \brief Set create link drag and drop target or NULL. */
	void SetCreateLinkTarget( igdeNativeFoxNVSlot *target );
	
	/** \brief Finish create link drag and drop. */
	void FinishCreateLink();
	
	/** \brief Link closest to position inside range in board coordinate system or NULL. */
	igdeNVLink *ClosestLinkNear( const decPoint &position, float range ) const;
	
	/** \brief Hover link or NULL. */
	inline igdeNVLink *GetHoverLink() const{ return pHoverLink; }
	
	/** \brief Set hover link. */
	void SetHoverLink( igdeNVLink *link );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint( FXObject *sender, FXSelector selector, void *data );
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	long onLeftMousePress( FXObject *sender, FXSelector selector, void *data );
	long onLeftMouseRelease( FXObject *sender, FXSelector selector, void *data );
	long onMouseMoved( FXObject *sender, FXSelector selector, void *data );
	long onRightMousePress( FXObject *sender, FXSelector selector, void *data );
	long onRightMouseRelease( FXObject *sender, FXSelector selector, void *data );
	
	
	
	void DrawLinks( FXDC &dc ) const;
	void DrawCreateLink( FXDC &dc ) const;
	
	void DrawBezier( FXDC &dc, FXColor color, const decPoint &p1, const decPoint &p2,
		const decPoint &p3, const decPoint &p4 ) const;
	
	void DrawBezier( FXDC &dc, const decPoint &p1, const decPoint &p2, const decPoint &p3,
		const decPoint &p4 ) const;
	
	void InternalDrawBezier( FXDC &dc, const decVector2 &p1, const decVector2 &p2,
		const decVector2 &p3, const decVector2 &p4 ) const;
	
	bool LineIntersectsBezier( const decPoint &lp1, const decPoint &lp2, const decPoint &bp1,
		const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const;
	
	bool InternalLineIntersectsBezier( const decVector2 &ldir, float llen, const decVector2 &lpos,
		const decVector2 &bp1, const decVector2 &bp2, const decVector2 &bp3, const decVector2 &bp4 ) const;
	
	float PointBezierDistance( const decPoint &p, const decPoint &bp1,
		const decPoint &bp2, const decPoint &bp3, const decPoint &bp4 ) const;
	
	float PointBezierDistance( const decVector2 &bp, const decVector2 &bp1, const decVector2 &bp2,
		const decVector2 &bp3, const decVector2 &bp4 ) const;
	/*@}*/
};

typedef igdeNativeFoxNVBoard igdeNativeNVBoard;

#endif
