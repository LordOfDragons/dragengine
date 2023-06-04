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

#ifndef _IGDEMENUOPTION_H_
#define _IGDEMENUOPTION_H_

#include "igdeMenuCommand.h"



/**
 * \brief IGDE UI Option Menu Entry.
 * 
 * Menu entry changes "selected" state and calls OnAction() when user clicks menu
 * and the menu is enabled. Only one igdeMenuOption in the same group of options
 * is selected at all times.
 */
class DE_DLL_EXPORT igdeMenuOption : public igdeMenuCommand{
private:
	bool pSelected;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create menu entry. */
	igdeMenuOption( igdeEnvironment &environment );
	
	/** \brief Create menu entry. */
	igdeMenuOption( igdeEnvironment &environment, igdeAction *action );
	
	
	
protected:
	/**
	 * \brief Clean up menu entry.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeMenuOption();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Menu entry is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if menu entry is selected. */
	void SetSelected( bool selected );
	
	
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	/*@}*/
	
	
	
	/**
	 * \text IGDE Internal Use Only
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
	
	
	
protected:
	/** \brief Selected changed. */
	virtual void OnSelectedChanged();
	/*@}*/
};

#endif
