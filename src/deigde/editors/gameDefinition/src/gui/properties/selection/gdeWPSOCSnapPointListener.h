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

#ifndef _GDEWPSOCSNAPPOINTLISTENER_H_
#define _GDEWPSOCSNAPPOINTLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCSnapPoint;



/**
 * \brief Object class snap point panel listener.
 */
class gdeWPSOCSnapPointListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCSnapPoint &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCSnapPointListener( gdeWPSOCSnapPoint &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCSnapPointListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class snap points changed. */
	virtual void OCSnapPointsChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class snap point changed. */
	virtual void OCSnapPointChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** \brief Active object class snap point changed. */
	virtual void ActiveOCSnapPointChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
