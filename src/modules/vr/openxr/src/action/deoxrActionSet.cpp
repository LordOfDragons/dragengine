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

#include "deoxrActionSet.h"
#include "../deVROpenXR.h"
#include "../deoxrBasics.h"
#include "../deoxrInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrActionSet
/////////////////////////

deoxrActionSet::deoxrActionSet( deoxrInstance &instance, const char *name, const char *localizedName ) :
pInstance( instance ),
pName( name ),
pLocalizedName( localizedName ),
pActionSet( XR_NULL_HANDLE )
{
	try{
		XrActionSetCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_ACTION_SET_CREATE_INFO;
		#ifdef OS_W32_VS
			strncpy_s( createInfo.actionSetName, sizeof( createInfo.actionSetName ),
				name, sizeof( createInfo.actionSetName ) - 1 );
			strncpy_s( createInfo.localizedActionSetName, sizeof( createInfo.localizedActionSetName ),
				localizedName, sizeof( createInfo.localizedActionSetName ) - 1 );
		#else
			strncpy( createInfo.actionSetName, name, sizeof( createInfo.actionSetName ) - 1 );
			strncpy( createInfo.localizedActionSetName, localizedName,
				sizeof( createInfo.localizedActionSetName ) - 1 );
		#endif
		createInfo.priority = 0;
		
		OXR_CHECK( instance.xrCreateActionSet( instance.GetInstance(), &createInfo, &pActionSet ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrActionSet::~deoxrActionSet(){
	pCleanUp();
}



// Management
///////////////

int deoxrActionSet::GetActionCount() const{
	return pActions.GetCount();
}

deoxrAction *deoxrActionSet::GetActionAt( int index ) const{
	return ( deoxrAction* )pActions.GetAt( index );
}

deoxrAction *deoxrActionSet::GetActionNamed( const char *name ) const{
	const int count = pActions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoxrAction * const action = ( deoxrAction* )pActions.GetAt( i );
		if( action->GetName() == name ){
			return action;
		}
	}
	
	return nullptr;
}

deoxrAction *deoxrActionSet::AddAction( deoxrAction::eType type,
const char *name, const char *localizedName ){
	if( GetActionNamed( name ) ){
		DETHROW_INFO( deeInvalidParam, decString( "duplicate action: " ) + name );
	}
	
	const deoxrAction::Ref action( deoxrAction::Ref::New(
		new deoxrAction( *this, type, name, localizedName ) ) );
	pActions.Add( action );
	return action;
}

deoxrAction *deoxrActionSet::AddAction( deoxrAction::eType type, const char *name,
const char *localizedName, const XrPath *subactionPath, int subactionPathCount ){
	if( GetActionNamed( name ) ){
		DETHROW_INFO( deeInvalidParam, decString( "duplicate action: " ) + name );
	}
	
	const deoxrAction::Ref action( deoxrAction::Ref::New( new deoxrAction(
		*this, type, name, localizedName, subactionPath, subactionPathCount ) ) );
	pActions.Add( action );
	return action;
}

deoxrAction *deoxrActionSet::AddBoolAction( const char *name, const char *localizedName ){
	return AddAction( deoxrAction::etInputBool, name, localizedName );
}

deoxrAction *deoxrActionSet::AddFloatAction( const char *name, const char *localizedName ){
	return AddAction( deoxrAction::etInputFloat, name, localizedName );
}

deoxrAction *deoxrActionSet::AddVector2Action( const char *name, const char *localizedName ){
	return AddAction( deoxrAction::etInputVector2, name, localizedName );
}

deoxrAction *deoxrActionSet::AddPoseAction( const char *name, const char *localizedName ){
	return AddAction( deoxrAction::etInputPose, name, localizedName );
}

deoxrAction *deoxrActionSet::AddVibrationAction( const char *name, const char *localizedName ){
	return AddAction( deoxrAction::etOutputVibration, name, localizedName );
}



// Private Functions
//////////////////////

void deoxrActionSet::pCleanUp(){
	pActions.RemoveAll();
	
	if( pActionSet ){
		pInstance.xrDestroyActionSet( pActionSet );
		pActionSet = XR_NULL_HANDLE;
	}
}
