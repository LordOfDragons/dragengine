/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALWIDGETOPTIONGROUP_H_
#define _DEALWIDGETOPTIONGROUP_H_

#include "../../deObject.h"
#include "../../common/collection/decPointerSet.h"

class dealWidgetOptionBox;


/**
 * \brief Widget option box group.
 */
class dealWidgetOptionGroup : public deObject{
private:
	decPointerSet pWidgets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create option group. */
	dealWidgetOptionGroup();
	
	/** \brief Clean up option group. */
	virtual ~dealWidgetOptionGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of widgets. */
	int GetCount() const;
	
	/** \brief Widget at index. */
	dealWidgetOptionBox *GetAt( int index ) const;
	
	/** \brief Add widget. */
	void Add( dealWidgetOptionBox *widget );
	
	/** \brief Remove widget. */
	void Remove( dealWidgetOptionBox *widget );
	
	/** \brief Remove all widgets. */
	void RemoveAll();
	
	/** \brief Selected option or \em NULL if none in the group is selected. */
	dealWidgetOptionBox *GetSelected() const;
	
	/** \brief Select option and deselect all others or \em NULL to deselect all. */
	void Select( dealWidgetOptionBox *option );
	/*@}*/
};

#endif
