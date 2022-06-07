/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDENATIVENULLNVBOARD_H_
#define _IGDENATIVENULLNVBOARD_H_

#include <dragengine/common/math/decMath.h>

class igdeNVBoard;
class igdeNVLink;
class igdeNativeNullNVSlot;


/**
 * Null NodeView Board.
 */
class igdeNativeNullNVBoard{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVBoard();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVBoard();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVBoard* CreateNativeWidget( igdeNVBoard &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
	void BeginCreateLink( igdeNativeNullNVSlot *source );
	
	/** \brief Link drag and drop source or NULL. */
	inline igdeNativeNullNVSlot *GetCreateLinkSource() const{ return nullptr; }
	
	/** \brief Set create link drag and drop position. */
	void SetCreateLinkPosition( const decPoint &position );
	
	/** \brief Create link drag and drop target or NULL. */
	inline igdeNativeNullNVSlot *GetCreateLinkTarget() const{ return nullptr; }
	
	/** \brief Set create link drag and drop target or NULL. */
	void SetCreateLinkTarget( igdeNativeNullNVSlot *target );
	
	/** \brief Finish create link drag and drop. */
	void FinishCreateLink();
	
	/** \brief Link closest to position inside range in board coordinate system or NULL. */
	igdeNVLink *ClosestLinkNear( const decPoint &position, float range ) const;
	
	/** \brief Hover link or NULL. */
	inline igdeNVLink *GetHoverLink() const{ return nullptr; }
	
	/** \brief Set hover link. */
	void SetHoverLink( igdeNVLink *link );
	/*@}*/
};

typedef igdeNativeNullNVBoard igdeNativeNVBoard;

#endif
