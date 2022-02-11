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

#ifndef _DEOXRDPHTCVIVECOSMOSCONTROLLERINTERACTION_H_
#define _DEOXRDPHTCVIVECOSMOSCONTROLLERINTERACTION_H_

#include "deoxrDPBaseTwoHandController.h"


/**
 * HTC Vive Cosmos Controller Interaction controller profile.
 */
class deoxrDPHTCViveCosmosControllerInteraction : public deoxrDPBaseTwoHandController{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPHTCViveCosmosControllerInteraction( deoxrInstance &instance );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPHTCViveCosmosControllerInteraction();
	/*@}*/
	
	
protected:
	virtual bool pProfileEnabled() const;
	virtual void pSuggestBindings();
	virtual void pAddDevice( bool left );
};

#endif

