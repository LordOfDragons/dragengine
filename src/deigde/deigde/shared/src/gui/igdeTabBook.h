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

#ifndef _IGDETABBOOK_H_
#define _IGDETABBOOK_H_

#include "igdeContainer.h"
#include "resources/igdeIconReference.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief IGDE UI Tab book.
 */
class igdeTabBook : public igdeContainer{
private:
	decObjectList pHeaders;
	int pActivePanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeTabBook( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTabBook();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active panel index. */
	inline int GetActivePanel() const{ return pActivePanel; }
	
	/** \brief Set active panel by index. */
	void SetActivePanel( int index );
	
	
	
	/** \brief Add child . */
	virtual void AddChild( igdeWidget *child, const char *text );
	
	/** \brief Add child . */
	virtual void AddChild( igdeWidget *child, const char *text, igdeIcon *icon );
	
	/** \brief Add child . */
	virtual void AddChild( igdeWidget *child, const char *text, igdeIcon *icon,
		const char *description );
	
	/** \brief Add child not possible without header information. */
	virtual void AddChild( igdeWidget *child );
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Header.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	class cHeader : public deObject{
	public:
		decString text;
		decString description;
		igdeIconReference icon;
		
		cHeader( const char *text, igdeIcon *icon, const char *description );
		virtual ~cHeader();
	};
	
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
	
	/**
	 * \brief Get native container widget pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void *GetNativeContainer() const;
	
	
	
protected:
	/**
	 * \brief Create child widget native widgets.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateChildWidgetNativeWidgets();
	/*@}*/
};

#endif
