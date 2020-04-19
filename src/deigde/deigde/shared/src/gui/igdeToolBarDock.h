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

#ifndef _IGDETOOLBARDOCK_H_
#define _IGDETOOLBARDOCK_H_

#include "igdeContainer.h"


/**
 * \brief IGDE UI ToolBar Docking Area.
 * 
 * ToolBars can be added to the docking area by the user any place he likes.
 */
class igdeToolBarDock : public igdeContainer{
public:
	/** \brief Dock side. */
	enum eSide{
		/** \brief Top. */
		esTop,
		
		/** \brief Left. */
		esLeft,
		
		/** \brief Right. */
		esRight,
		
		/** \brief Bottom. */
		esBottom
	};
	
	
	
private:
	eSide pSide;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeToolBarDock( igdeEnvironment &environment, eSide site );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeToolBarDock();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Side. */
	inline eSide GetSide() const{ return pSide; }
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
