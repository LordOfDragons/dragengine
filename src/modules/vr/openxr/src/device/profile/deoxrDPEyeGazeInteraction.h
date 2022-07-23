/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRDPEYEGAZEINTERACTION_H_
#define _DEOXRDPEYEGAZEINTERACTION_H_

#include "deoxrDeviceProfile.h"


/**
 * Eye gaze interaction profile.
 */
class deoxrDPEyeGazeInteraction : public deoxrDeviceProfile{
private:
	deoxrDevice::Ref pDevice;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPEyeGazeInteraction( deoxrInstance &instance );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPEyeGazeInteraction();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Check attached. */
	virtual void CheckAttached();
	
	/** Suggest bindings. */
	virtual void SuggestBindings();
	
	/** Clear actions. */
	virtual void ClearActions();
	/*@}*/
	
	
	
private:
	void pAddDevice();
	void pRemoveDevice();
};

#endif

