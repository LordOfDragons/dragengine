/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEWPLINKLISTENER_H_
#define _SEWPLINKLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPLink;


/**
 * \brief Link properties panel listener.
 */
class seWPLinkListener : public seSkyListener{
private:
	seWPLink &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPLinkListener( seWPLink &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPLinkListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged ( seSky *sky );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( seSky *sky, seController *controller );
	
	/** \brief Link added or removed. */
	virtual void LinkStructureChanged ( seSky *sky );
	
	/** \brief Link changed. */
	virtual void LinkChanged( seSky *sky, seLink *link );
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged( seSky *sky, seLink *link );
	
	/** \brief Active link changed. */
	virtual void ActiveLinkChanged( seSky *sky );
	/*@}*/
};

// end of include only once
#endif
