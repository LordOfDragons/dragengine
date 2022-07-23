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
#include <string.h>

#include "deAnimatorLink.h"

#include "../../common/exceptions.h"



// Class deAnimatorLink
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

deAnimatorLink::deAnimatorLink() :
pController( -1 ),
pRepeat( 1 ),
pBoneParameter( ebpPositionZ ),
pBoneMinValue( 0.0f ),
pBoneMaxValue( 1.0f ),
pWrapY( false )
{
	pCurve.SetDefaultLinear();
}

deAnimatorLink::deAnimatorLink( const deAnimatorLink &copy ) :
pController( copy.pController ),
pCurve( copy.pCurve ),
pRepeat( copy.pRepeat ),
pBone( copy.pBone ),
pBoneParameter( copy.pBoneParameter ),
pBoneMinValue( copy.pBoneMinValue ),
pBoneMaxValue( copy.pBoneMaxValue ),
pWrapY( copy.pWrapY ){
}

deAnimatorLink::~deAnimatorLink(){
}



// Management
///////////////

void deAnimatorLink::SetController( int controller ){
	if( controller < -1 ){
		DETHROW( deeInvalidParam );
	}
	pController = controller;
}

void deAnimatorLink::SetCurve( const decCurveBezier &curve ){
	pCurve = curve;
}

void deAnimatorLink::SetRepeat( int repeat ){
	if( repeat < 1 ){
		DETHROW( deeInvalidParam );
	}
	pRepeat = repeat;
}

void deAnimatorLink::SetBone( const char *bone ){
	pBone = bone;
}

void deAnimatorLink::SetBoneParameter( eBoneParameter parameter ){
	pBoneParameter = parameter;
}

void deAnimatorLink::SetBoneValueRange( float minimum, float maximum ){
	pBoneMinValue = minimum;
	pBoneMaxValue = maximum;
}

void deAnimatorLink::SetWrapY( bool wrap ){
	pWrapY = wrap;
}



// Operators
//////////////

deAnimatorLink &deAnimatorLink::operator=( const deAnimatorLink &copy ){
	pController = copy.pController;
	pCurve = copy.pCurve;
	pRepeat = copy.pRepeat;
	pBone = copy.pBone;
	pBoneParameter = copy.pBoneParameter;
	pBoneMinValue = copy.pBoneMinValue;
	pBoneMaxValue = copy.pBoneMaxValue;
	pWrapY = copy.pWrapY;
	return *this;
}
