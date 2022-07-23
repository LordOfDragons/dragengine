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

#ifndef _DEOXRDPDPBASETWOHANDCONTROLLER_H_
#define _DEOXRDPDPBASETWOHANDCONTROLLER_H_

#include "deoxrDeviceProfile.h"
#include "../deoxrDevice.h"


/**
 * Base class for two hand controller profile.
 */
class deoxrDPBaseTwoHandController : public deoxrDeviceProfile{
protected:
	deoxrDevice::Ref pDeviceLeft;
	deoxrDevice::Ref pDeviceRight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPBaseTwoHandController( deoxrInstance &instance, const deoxrPath &path, const char *name );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPBaseTwoHandController();
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
	
	/** Remove device if matching type. */
	virtual void RemoveDevice( deInputDevice::eDeviceTypes type );
	/*@}*/
	
	
	
protected:
	virtual bool pProfileEnabled() const;
	virtual void pSuggestBindings() = 0;
	
	virtual void pAddDevice( bool left ) = 0;
	
	void pRemoveDevice( bool left );
};

#endif

