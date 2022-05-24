/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deBaseVRModule.h"
#include "../../../common/exceptions.h"


// Class deBaseVRModule
/////////////////////////

deBaseVRModule::deBaseVRModule( deLoadableModule &loadableModule ) :
deBaseModule( loadableModule ){
}

deBaseVRModule::~deBaseVRModule(){
}



// Management
///////////////

float deBaseVRModule::GetDeviceFaceExpression( int device, int expression ){
	return 0.0f;
}

deBaseVRModule::eVRRenderFormat deBaseVRModule::GetRenderFormat(){
	DETHROW( deeInvalidParam );
}

int deBaseVRModule::GetEyeViewImages( eEye, int count, void* ){
	if( count == 0 ){
		return 0;
		
	}else{
		DETHROW_INFO( deeInvalidParam, "count > 0" );
	}
}

void deBaseVRModule::GetEyeViewRenderTexCoords( eEye, decVector2& tcFrom, decVector2& tcTo){
	tcFrom.Set( 0.0f, 0.0f );
	tcTo.Set( 1.0f, 1.0f );
}

int deBaseVRModule::AcquireEyeViewImage( eEye ){
	return -1;
}

void deBaseVRModule::ReleaseEyeViewImage( eEye ){
}
