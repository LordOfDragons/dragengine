/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSTIMOCNAVBLOCKER_H_
#define _GDEWPSTIMOCNAVBLOCKER_H_

#include "gdeWPSTIMOCSubObject.h"

class gdeOCNavigationBlocker;


/**
 * \brief Object class navigation blocker.
 */
class gdeWPSTIMOCNavBlocker : public gdeWPSTIMOCSubObject{
private:
	gdeOCNavigationBlocker *pNavBlocker;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCNavBlocker( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCNavigationBlocker *navBlocker, int index );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMOCNavBlocker();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Navigation blocker. */
	inline gdeOCNavigationBlocker *GetOCNavBlocker() const{ return pNavBlocker; }
	
	/** \brief Validate and update state accordingly. */
	void Validate();
	
	/** \brief Verify if valid. */
	virtual bool IsValid() const;
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
};

#endif
