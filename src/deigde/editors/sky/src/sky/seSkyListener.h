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

#ifndef _SESKYLISTENER_H_
#define _SESKYLISTENER_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class seBody;
class seController;
class seLayer;
class seLink;
class seSky;



/**
 * \brief Sky listener.
 */
class seSkyListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seSkyListener();
	
protected:
	/** \brief Clean up listener. */
	virtual ~seSkyListener();
	/*@}*/
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( seSky *sky );
	
	/** \brief Undo changed. */
	virtual void UndoChanged( seSky *sky );
	
	/** \brief Sky parameters changed. */
	virtual void SkyChanged( seSky *sky );
	
	/** \brief The environment object changed. */
	virtual void EnvObjectChanged( seSky *sky );
	
	/** \brief The view changed. */
	virtual void ViewChanged( seSky *sky );
	
	/** \brief The camera changed. */
	virtual void CameraChanged( seSky *sky );
	
	
	
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged ( seSky *sky );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( seSky *sky, seController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( seSky *sky, seController *controller );
	
	/** \brief Controller changed value. */
	virtual void ControllerValueChanged( seSky *sky, seController *controller );
	
	/** \brief Controller selection changed. */
	virtual void ControllerSelectionChanged( seSky *sky );
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged( seSky *sky );
	
	
	
	/** \brief Link added or removed. */
	virtual void LinkStructureChanged ( seSky *sky );
	
	/** \brief Link changed. */
	virtual void LinkChanged( seSky *sky, seLink *link );
	
	/** \brief Link name changed. */
	virtual void LinkNameChanged( seSky *sky, seLink *link );
	
	/** \brief Link selection changed. */
	virtual void LinkSelectionChanged( seSky *sky );
	
	/** \brief Active controller changed. */
	virtual void ActiveLinkChanged( seSky *sky );
	
	
	
	/** \brief Layer added or removed. */
	virtual void LayerStructureChanged ( seSky *sky );
	
	/** \brief Layer changed. */
	virtual void LayerChanged( seSky *sky, seLayer *layer );
	
	/** \brief Layer name changed. */
	virtual void LayerNameChanged( seSky *sky, seLayer *layer );
	
	/** \brief Layer selection changed. */
	virtual void LayerSelectionChanged( seSky *sky );
	
	/** \brief Active layer changed. */
	virtual void ActiveLayerChanged( seSky *sky );
	
	
	
	/** \brief Body added or removed. */
	virtual void BodyStructureChanged ( seSky *sky, seLayer *layer );
	
	/** \brief Body changed. */
	virtual void BodyChanged( seSky *sky, seLayer *layer, seBody *body );
	
	/** \brief Body selection changed. */
	virtual void BodySelectionChanged( seSky *sky, seLayer *layer );
	
	/** \brief Active body changed. */
	virtual void ActiveBodyChanged( seSky *sky, seLayer *layer );
	
	
	
	/** \brief Target changed. */
    virtual void TargetChanged( seSky *sky, seLayer *layer, deSkyLayer::eTargets target );
	
	/** \brief Active target changed. */
	virtual void ActiveTargetChanged( seSky *sky, seLayer *layer );
	/*@}*/
};

#endif
