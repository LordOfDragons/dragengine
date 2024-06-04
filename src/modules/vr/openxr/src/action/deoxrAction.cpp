/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
pActionSet( nullptr ),
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
		#ifdef OS_W32_VS
		strncpy_s( createInfo.actionName, sizeof( createInfo.actionName ),
			name, sizeof( createInfo.actionName ) - 1 );
		strncpy_s( createInfo.localizedActionName, sizeof( createInfo.localizedActionName ),
			localizedName, sizeof( createInfo.localizedActionName ) - 1 );
		#else
		strncpy( createInfo.actionName, name, sizeof( createInfo.actionName ) - 1 );
		strncpy( createInfo.localizedActionName, localizedName,
			sizeof( createInfo.localizedActionName ) - 1 );
		#endif
		
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
		
		OXR_CHECK( instance.xrCreateAction( actionSet.GetActionSet(), &createInfo, &pAction ) );
		
		pActionSet = &actionSet;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrAction::deoxrAction( deoxrActionSet &actionSet, eType type, const char *name,
	const char *localizedName, const XrPath *subactionPath, int subactionPathCount ) :
pActionSet( nullptr ),
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
		#ifdef OS_W32_VS
		strncpy_s( createInfo.actionName, name, sizeof( createInfo.actionName ) - 1 );
		strncpy_s( createInfo.localizedActionName, localizedName,
			sizeof( createInfo.localizedActionName ) - 1 );
		#else
		strncpy( createInfo.actionName, name, sizeof( createInfo.actionName ) - 1 );
		strncpy( createInfo.localizedActionName, localizedName,
			sizeof( createInfo.localizedActionName ) - 1 );
		#endif
		
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
		
		createInfo.subactionPaths = subactionPath;
		createInfo.countSubactionPaths = subactionPathCount;
		
		OXR_CHECK( instance.xrCreateAction( actionSet.GetActionSet(), &createInfo, &pAction ) );
		
		pActionSet = &actionSet;
		
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

void deoxrAction::DropActionSet(){
	pActionSet = nullptr;
	pAction = nullptr;
}



// Private Functions
//////////////////////

void deoxrAction::pCleanUp(){
	if( pAction ){
		if( pActionSet ){
			pActionSet->GetInstance().xrDestroyAction( pAction );
			pActionSet = nullptr;
		}
		pAction = nullptr;
	}
}
