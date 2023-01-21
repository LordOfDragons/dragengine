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

#ifndef _IGDENATIVENULLNVNODE_H_
#define _IGDENATIVENULLNVNODE_H_

#include <dragengine/common/math/decMath.h>

class igdeNVNode;


/**
 * Null NodeView Node.
 */
class igdeNativeNullNVNode{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVNode();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVNode();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVNode* CreateNativeWidget( igdeNVNode &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
	/*@}*/
};

typedef igdeNativeNullNVNode igdeNativeNVNode;

#endif
