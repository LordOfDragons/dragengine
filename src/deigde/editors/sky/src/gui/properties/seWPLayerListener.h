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

#ifndef _SEWPLAYERLISTENER_H_
#define _SEWPLAYERLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPLayer;



/**
 * \brief Layer properties panel listener.
 */
class seWPLayerListener : public seSkyListener{
private:
	seWPLayer &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPLayerListener( seWPLayer &panel );
	
protected:
	/** \brief Clean up listener. */
	virtual ~seWPLayerListener();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Link added or removed. */
	virtual void LinkStructureChanged ( seSky *sky );
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged( seSky *sky, seLink *link );
	
	
	
	/** \brief Layer added or removed. */
	virtual void LayerStructureChanged ( seSky *sky );
	
	/** \brief Layer changed. */
	virtual void LayerChanged( seSky *sky, seLayer *layer );
	
	/** \brief Layer name changed. */
	virtual void LayerNameChanged( seSky *sky, seLayer *layer );
	
	/** \brief Active layer changed. */
	virtual void ActiveLayerChanged( seSky *sky );
	
	
	
	/** \brief Body added or removed. */
	virtual void BodyStructureChanged ( seSky *sky, seLayer *layer );
	
	/** \brief Body changed. */
	virtual void BodyChanged( seSky *sky, seLayer *layer, seBody *body );
	
	/** \brief Active body changed. */
	virtual void ActiveBodyChanged( seSky *sky, seLayer *layer );
	
	
	
	/** \brief Target changed. */
    virtual void TargetChanged( seSky *sky, seLayer *layer, deSkyLayer::eTargets target );
	
	/** \brief Active target changed. */
	virtual void ActiveTargetChanged( seSky *sky, seLayer *layer );
	/*@}*/
};

#endif
