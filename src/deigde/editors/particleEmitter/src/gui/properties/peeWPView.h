/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEWPVIEW_H_
#define _PEEWPVIEW_H_

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>
#include <deigde/gui/properties/igdeWPSkyReference.h>

class peeEmitter;
class peeWindowProperties;
class peeWPViewListener;



/**
 * \brief View Panel.
 */
class peeWPView : public igdeContainerScroll{
private:
	peeWindowProperties &pWindowProperties;
	peeEmitter *pEmitter;
	peeWPViewListener *pListener;
	
	igdeWPSkyReference pWPSky;
	igdeWPWObjectReference pWPEnvObject;
	igdeWPCameraReference pWPCamera;
	
	igdeEditVectorReference pEditEmitterPosition;
	igdeEditVectorReference pEditEmitterRotation;
	igdeTextFieldReference pEditEmitterBurstInterval;
	igdeTextFieldReference pEditEmitterWarmUpTime;
	igdeCheckBoxReference pChkEmitterEnableCasting;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	peeWPView( peeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up the rig. */
	virtual ~peeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
