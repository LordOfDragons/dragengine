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

#ifndef _GDEWPSTIMOCSNAPPOINT_H_
#define _GDEWPSTIMOCSNAPPOINT_H_

#include "gdeWPSTIMOCSubObject.h"

class gdeOCSnapPoint;


/**
 * \brief Object class snap point.
 */
class gdeWPSTIMOCSnapPoint : public gdeWPSTIMOCSubObject{
private:
	gdeOCSnapPoint *pSnapPoint;
	int pIndex;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCSnapPoint( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCSnapPoint *snapPoint, int index );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMOCSnapPoint();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Snap point. */
	inline gdeOCSnapPoint *GetOCSnapPoint() const{ return pSnapPoint; }
	
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
	
	/** \brief Update name. */
	void UpdateName();
	/*@}*/
};

#endif
