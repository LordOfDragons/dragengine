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

#include "deoxrAction.h"
#include "deoxrActionSet.h"
#include "../deoxrBasics.h"
#include "../deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrAction
/////////////////////////

deoxrAction::deoxrAction( deoxrActionSet &actionSet, eType type, const char *name, const char *localizedName ) :
pActionSet( actionSet ),
pType( type ),
pName( name ),
pLocalizedName( localizedName ),
pAction( XR_NULL_HANDLE )
{
	deoxrInstance &instance = actionSet.GetInstance();
	
	try{
		XrActionCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_ACTION_CREATE_INFO;
		strncpy( createInfo.actionName, name, sizeof( createInfo.actionName ) - 1 );
		strncpy( createInfo.localizedActionName, localizedName,
			sizeof( createInfo.localizedActionName ) - 1 );
		
		switch( type ){
		case etInputBool:
			createInfo.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
			break;
			
		case etInputFloat:
			createInfo.actionType = XR_ACTION_TYPE_FLOAT_INPUT;
			break;
			
		case etInputVector2:
			createInfo.actionType = XR_ACTION_TYPE_VECTOR2F_INPUT;
			break;
			
		case etInputPose:
			createInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
			break;
			
		case etOutputVibration:
			createInfo.actionType = XR_ACTION_TYPE_VIBRATION_OUTPUT;
			break;
			
		default:
			DETHROW( deeInvalidAction );
		}
		
		const XrPath subactionPaths[ 2 ] = { instance.GetPathHandLeft(), instance.GetPathHandRight() };
		createInfo.subactionPaths = subactionPaths;
		createInfo.countSubactionPaths = 2;
		
		OXR_CHECK( instance.GetOxr(), instance.xrCreateAction(
			actionSet.GetActionSet(), &createInfo, &pAction ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrAction::~deoxrAction(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoxrAction::pCleanUp(){
	if( pAction ){
		pActionSet.GetInstance().xrDestroyAction( pAction );
		pAction = XR_NULL_HANDLE;
	}
}
