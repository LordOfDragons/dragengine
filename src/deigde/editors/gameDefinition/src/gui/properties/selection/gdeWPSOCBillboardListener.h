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

#ifndef _GDEWPSOCBILLBOARDLISTENER_H_
#define _GDEWPSOCBILLBOARDLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCBillboard;



/**
 * \brief Object class billboard panel listener.
 */
class gdeWPSOCBillboardListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCBillboard &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCBillboardListener( gdeWPSOCBillboard &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCBillboardListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class billboards changed. */
	virtual void OCBillboardsChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class billboard changed. */
	virtual void OCBillboardChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard );
	
	/** \brief Active object class billboard changed. */
	virtual void ActiveOCBillboardChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
