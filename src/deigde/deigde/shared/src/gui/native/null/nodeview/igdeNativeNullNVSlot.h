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

#ifndef _IGDENATIVENULLNVSLOT_H_
#define _IGDENATIVENULLNVSLOT_H_

#include <dragengine/common/math/decMath.h>

class igdeNVSlot;
class igdeNativeNullNVSocket;


/**
 * Null NodeView Slot.
 */
class igdeNativeNullNVSlot{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullNVSlot();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullNVSlot();
	
	/** \brief Create native widget. */
	static igdeNativeNullNVSlot* CreateNativeWidget( igdeNVSlot &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
	/*@}*/
};

typedef igdeNativeNullNVSlot igdeNativeNVSlot;

#endif
