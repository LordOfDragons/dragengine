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

#ifndef _GDEWPSTIMOCCOMPONENT_H_
#define _GDEWPSTIMOCCOMPONENT_H_

#include "gdeWPSTIMOCSubObject.h"

class gdeOCComponent;


/**
 * \brief Object class component.
 */
class gdeWPSTIMOCComponent : public gdeWPSTIMOCSubObject{
private:
	gdeOCComponent *pComponent;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCComponent( gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCComponent *component, int index );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMOCComponent();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Component. */
	inline gdeOCComponent *GetOCComponent() const{ return pComponent; }
	
	/** \brief Validate component and update state accordingly. */
	void Validate();
	
	/** \brief Verify if component is valid. */
	virtual bool IsValid() const;
	
	/** \brief Validate due to change in object class names or existance. */
	virtual void ValidateObjectClassName();
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
};

#endif
