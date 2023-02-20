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

#ifndef _IGDEACTIONEXTERNOPEN_H_
#define _IGDEACTIONEXTERNOPEN_H_

#include <stddef.h>

#include "igdeAction.h"


class igdeEnvironment;


/**
 * \brief IGDE UI Action System Open.
 * 
 * Open file or url using system specific application. For example directory path is opened
 * with a file manager, registered file types are opened with their preferred application
 * and URL are openned with their preferred application.
 */
class DE_DLL_EXPORT igdeActionExternOpen : public igdeAction{
private:
	igdeEnvironment &pEnvironment;
	decString pPath;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create action. */
	igdeActionExternOpen( igdeEnvironment &environment, const char *text,
		igdeIcon *icon, const char *description, const char *path = "" );
	
	
	
protected:
	/**
	 * \brief Clean up action.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeActionExternOpen();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Path to open or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path to open or empty string if not set. */
	void SetPath( const char *path );
	
	
	
	/**
	 * \brief Run action.
	 * 
	 * Called if user interacts with a UI element triggering actions.
	 */
	virtual void OnAction();
	
	/**
	 * \brief Request update of action parameters if required.
	 * 
	 * Called for example by UI elements to update action parameters.
	 * Default implementation does nothing.
	 */
	virtual void Update();
	/*@}*/
};

#endif
