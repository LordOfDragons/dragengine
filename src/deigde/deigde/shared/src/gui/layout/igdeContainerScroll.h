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

#ifndef _IGDECONTAINERSCROLL_H_
#define _IGDECONTAINERSCROLL_H_

#include "../igdeContainer.h"


/**
 * \brief IGDE UI Container Widget scrolling one widget.
 */
class igdeContainerScroll : public igdeContainer{
private:
	bool pCanScrollX;
	bool pCanScrollY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scrollbox. */
	igdeContainerScroll( igdeEnvironment &environment, bool canScrollX = true, bool canScrollY = true );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerScroll();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Can scroll X. */
	inline bool GetCanScrollX() const{ return pCanScrollX; }
	
	/** \brief Can scroll Y. */
	inline bool GetCanScrollY() const{ return pCanScrollY; }
	
	
	
	/** \brief Add child. */
	virtual void AddChild( igdeWidget *child );
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	/*@}*/
};

#endif
