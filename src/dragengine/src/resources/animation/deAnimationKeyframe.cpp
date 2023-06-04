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
#include "deAnimation.h"
#include "deAnimationKeyframe.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"


// class deAnimationKeyframe
//////////////////////////////

// constructor, destructor
////////////////////////////

deAnimationKeyframe::deAnimationKeyframe() :
pTime( 0.0f ),
pScale( 1.0f, 1.0f, 1.0f ){
}

deAnimationKeyframe::~deAnimationKeyframe(){
}



// management
///////////////

void deAnimationKeyframe::SetTime( float time ){
	pTime = decMath::max( time, 0.0f );
}

void deAnimationKeyframe::SetPosition( const decVector &position ){
	pPosition = position;
}

void deAnimationKeyframe::SetRotation( const decVector &rotation ){
	pRotation = rotation;
}

void deAnimationKeyframe::SetScale( const decVector &scale ){
	pScale = scale;
}
