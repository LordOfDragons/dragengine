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

#ifndef _GDEGDEBASEMAOCSUBOBJECT_H_
#define _GDEGDEBASEMAOCSUBOBJECT_H_

#include "../../gdeBaseAction.h"


class gdeObjectClass;


/**
 * \brief Menu action object class sub object.
 */
class gdeBaseMAOCSubObject : public gdeBaseAction{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create menu actions. */
	gdeBaseMAOCSubObject( gdeWindowMain &windowMain, const char *text,
		igdeIcon *icon, const char *description );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run action if game definition is not NULL. */
	virtual igdeUndo *OnAction( gdeGameDefinition &gameDefinition );
	
	/** \brief Run action if game definition is not NULL. */
	virtual igdeUndo *OnActionSubObject( gdeGameDefinition &gameDefinition,
		gdeObjectClass &objectClass ) = 0;
	
	/** \brief Request update of action parameters if required. */
	virtual void Update();
	
	/** \brief Get active object class if present. */
	gdeObjectClass *GetActiveObjectClass() const;
	/*@}*/
};

#endif
