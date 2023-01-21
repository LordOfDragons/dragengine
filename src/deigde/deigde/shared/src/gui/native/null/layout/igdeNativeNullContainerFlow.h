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

#ifndef _IGDENATIVENULLCONTAINERFLOW_H_
#define _IGDENATIVENULLCONTAINERFLOW_H_

class igdeContainerFlow;


/**
 * Null button.
 */
class igdeNativeNullContainerFlow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	igdeNativeNullContainerFlow();
	virtual ~igdeNativeNullContainerFlow();
	
	/** \brief Create native widget. */
	static void* CreateNativeWidget( igdeContainerFlow &owner );
	
	/** \brief Post create native widget. */
	static void PostCreateNativeWidget( igdeContainerFlow &owner, void *native );
	
	/** \brief Destroy native widget. */
	static void DestroyNativeWidget( igdeContainerFlow &owner, void *native );
	
	/** \brief Child removed. */
	static void ChildRemoved( igdeContainerFlow &owner, void *native );
	
	/** \brief Native container. */
	static void *GetNativeContainer( const igdeContainerFlow &owner, void *native );
	/*@}*/
};

typedef igdeNativeNullContainerFlow igdeNativeContainerFlow;

#endif
