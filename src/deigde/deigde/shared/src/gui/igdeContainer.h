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

#ifndef _IGDECONTAINER_H_
#define _IGDECONTAINER_H_

#include "igdeWidget.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>



/**
 * \brief IGDE UI Container Widget grouping widgets.
 */
class DE_DLL_EXPORT igdeContainer : public igdeWidget{
private:
	decObjectOrderedSet pChildren;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget with layout elNone. */
	igdeContainer( igdeEnvironment &environment );
	
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of children. */
	int GetChildCount() const;
	
	/** \brief Get child at index. */
	igdeWidget *GetChildAt( int index ) const;
	
	/** \brief Index of child or -1 if absent. */
	int IndexOfChild( igdeWidget *widget ) const;
	
	/** \brief Add child. */
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
	 * \brief Drop native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DropNativeWidget();
	
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
	
	/** \brief GuiTheme changed. */
	virtual void OnGuiThemeChanged();
	
	/**
	 * \brief Notify children gui theme changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void NotifyChildrenGuiThemeChanged();
	/*@}*/
};

#endif
