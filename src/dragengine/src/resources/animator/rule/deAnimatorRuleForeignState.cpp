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

deAnimatorRuleForeignState::deAnimatorRuleForeignState() :
pSourceCoordinateFrame( ecfBoneLocal ),
pDestCoordinateFrame( ecfBoneLocal ),
pScalePosition( 1.0f ),
pScaleOrientation( 1.0f ),
pScaleSize( 1.0f ),
pScaleVertexPositionSet( 1.0f ),
pModifyX( true ),
pModifyY( true ),
pModifyZ( true ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( false ),
pEnableVertexPositionSet( true ){
}

deAnimatorRuleForeignState::~deAnimatorRuleForeignState(){
}



// Management
///////////////

void deAnimatorRuleForeignState::SetForeignBone( const char *boneName ){
	pForeignBone = boneName;
}

void deAnimatorRuleForeignState::SetForeignVertexPositionSet( const char *vertexPositionSet ){
	pForeignVertexPositionSet = vertexPositionSet;
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

void deAnimatorRuleForeignState::SetScaleVertexPositionSet( float scale ){
	pScaleVertexPositionSet = scale;
}

void deAnimatorRuleForeignState::SetModifyX( bool modify ){
	pModifyX = modify;
}

void deAnimatorRuleForeignState::SetModifyY( bool modify ){
	pModifyY = modify;
}

void deAnimatorRuleForeignState::SetModifyZ( bool modify ){
	pModifyZ = modify;
}

void deAnimatorRuleForeignState::SetSourceCoordinateFrame( eCoordinateFrames coordinateFrame ){
	pSourceCoordinateFrame = coordinateFrame;
}

void deAnimatorRuleForeignState::SetDestCoordinateFrame( eCoordinateFrames coordinateFrame ){
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

void deAnimatorRuleForeignState::SetEnableVertexPositionSet( bool enabled ){
	pEnableVertexPositionSet = enabled;
}



// Visiting
/////////////

void deAnimatorRuleForeignState::Visit( deAnimatorRuleVisitor &visitor ){
	visitor.VisitForeignState( *this );
}
