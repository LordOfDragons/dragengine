/* 
 * Drag[en]gine Animator Module
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

#include "dearAnimator.h"
#include "deDEAnimator.h"
#include "animation/dearAnimation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>



// Class dearAnimator
///////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimator::dearAnimator( deDEAnimator &module, deAnimator &animator ) :
pModule( module ),
pAnimator( animator ),
pAnimation( nullptr ),
pUpdateTracker( 0 )
{
	if( animator.GetAnimation() ){
		pAnimation = ( dearAnimation* )animator.GetAnimation()->GetPeerAnimator();
	}
}

dearAnimator::~dearAnimator(){
	pCleanUp();
}



// Management
///////////////



// Notifications
//////////////////

void dearAnimator::RigChanged(){
	pUpdateTracker++;
}

void dearAnimator::AnimationChanged(){
	pUpdateTracker++;
	
	pAnimation = nullptr;
	if( pAnimator.GetAnimation() ){
		pAnimation = ( dearAnimation* )pAnimator.GetAnimation()->GetPeerAnimator();
	}
}

void dearAnimator::BonesChanged(){
	pUpdateTracker++;
}

void dearAnimator::VertexPositionSetsChanged(){
	pUpdateTracker++;
}

void dearAnimator::ControllerCountChanged(){
	pUpdateTracker++;
}

void dearAnimator::LinksChanged(){
	pUpdateTracker++;
}

void dearAnimator::RulesChanged(){
	pUpdateTracker++;
}



// Private functions
//////////////////////

void dearAnimator::pCleanUp(){
}
