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

#ifndef _IGDENATIVENULLGROUPBOX_H_
#define _IGDENATIVENULLGROUPBOX_H_

class igdeGroupBox;


/**
 * Null groupBox.
 */
class igdeNativeNullGroupBox{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullGroupBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullGroupBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullGroupBox* CreateNativeWidget( igdeGroupBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateCollapsed();
	virtual void UpdateTitle();
	virtual void UpdateTitleAlignment();
	virtual void UpdateStretchLast();
	/*@}*/
};

typedef igdeNativeNullGroupBox igdeNativeGroupBox;

#endif
