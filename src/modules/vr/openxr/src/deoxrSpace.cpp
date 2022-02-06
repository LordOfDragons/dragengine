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

#include <stdlib.h>
#include <string.h>

#include "deoxrSpace.h"
#include "deoxrSession.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSpace
//////////////////////

deoxrSpace::deoxrSpace( deoxrSession &session ) :
pSession( session ),
pSpace( 0 )
{
	try{
		XrReferenceSpaceCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
		createInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
		
		OXR_CHECK( session.GetSystem().GetInstance().GetOxr(),
			session.GetSystem().GetInstance().xrCreateReferenceSpace(
				session.GetSession(), &createInfo, &pSpace ) );
		
		// xrGetReferenceSpaceBoundsRect : get chaperone
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSpace::~deoxrSpace(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoxrSpace::pCleanUp(){
}
