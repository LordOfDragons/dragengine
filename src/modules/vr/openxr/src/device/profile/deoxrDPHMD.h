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

#ifndef _DEOXRDPHMD_H_
#define _DEOXRDPHMD_H_

#include "deoxrDeviceProfile.h"
#include "../deoxrDevice.h"


/**
 * HMD profile.
 */
class deoxrDPHMD : public deoxrDeviceProfile{
private:
	deoxrDevice::Ref pDevice;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPHMD( deoxrInstance &instance );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPHMD();
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
	
	
	
protected:
	void pAddDevice();
	void pRemoveDevice();
};

#endif

