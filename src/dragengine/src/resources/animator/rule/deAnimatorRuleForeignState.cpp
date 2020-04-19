/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deAnimatorRuleForeignState.h"
#include "deAnimatorRuleVisitor.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deAnimatorRuleForeignState
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorRuleForeignState::deAnimatorRuleForeignState(){
	pScalePosition = 0.0f;
	pScaleOrientation = 0.0f;
	pScaleSize = 0.0f;
	pSourceCoordinateFrame = ecfBoneLocal;
	pDestCoordinateFrame = ecfBoneLocal;
	pEnablePosition = true;
	pEnableOrientation = true;
	pEnableSize = false;
}

deAnimatorRuleForeignState::~deAnimatorRuleForeignState(){
}



// Management
///////////////

void deAnimatorRuleForeignState::SetForeignBone( const char *boneName ){
	if( ! boneName ){
		DETHROW( deeInvalidParam );
	}
	pForeignBone = boneName;
}

void deAnimatorRuleForeignState::SetScalePosition( float scalePosition ){
	pScalePosition = scalePosition;
}

void deAnimatorRuleForeignState::SetScaleOrientation( float scaleOrientation ){
	pScaleOrientation = scaleOrientation;
}

void deAnimatorRuleForeignState::SetScaleSize( float scaleSize ){
	pScaleSize = scaleSize;
}

void deAnimatorRuleForeignState::SetSourceCoordinateFrame( eCoordinateFrames coordinateFrame ){
	if( coordinateFrame < ecfBoneLocal || coordinateFrame > ecfComponent ){
		DETHROW( deeInvalidParam );
	}
	pSourceCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleForeignState::SetDestCoordinateFrame( eCoordinateFrames coordinateFrame ){
	if( coordinateFrame < ecfBoneLocal || coordinateFrame > ecfComponent ){
		DETHROW( deeInvalidParam );
	}
	pDestCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleForeignState::SetEnablePosition( bool enable ){
	pEnablePosition = enable;
}

void deAnimatorRuleForeignState::SetEnableOrientation( bool enable ){
	pEnableOrientation = enable;
}

void deAnimatorRuleForeignState::SetEnableSize( bool enable ){
	pEnableSize = enable;
}



// Visiting
/////////////

void deAnimatorRuleForeignState::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitForeignState( *this );
}
