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

#include "deAnimator.h"
#include "deAnimatorManager.h"
#include "deAnimatorLink.h"
#include "controller/deAnimatorController.h"
#include "rule/deAnimatorRule.h"
#include "../animation/deAnimation.h"
#include "../rig/deRig.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimator.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deAnimator
/////////////////////

// Constructor, destructor
////////////////////////////

deAnimator::deAnimator( deAnimatorManager *manager ) :
deResource( manager ),
pControllers( NULL ),
pControllerCount( 0 ),
pControllerSize( 0 ),
pLinks( NULL ),
pLinkCount( 0 ),
pLinkSize( 0 ),
pPeerAnimator( NULL ){
}

deAnimator::~deAnimator(){
	pCleanUp();
}



// Management
///////////////

void deAnimator::SetRig( deRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	pRig = rig;
	
	if( pPeerAnimator ){
		pPeerAnimator->RigChanged();
	}
}

void deAnimator::SetAnimation( deAnimation *animation ){
	if( pAnimation == animation ){
		return;
	}
	
	pAnimation = animation;
	
	if( pPeerAnimator ){
		pPeerAnimator->AnimationChanged();
	}
}

void deAnimator::NotifyBonesChanged(){
	if( pPeerAnimator ){
		pPeerAnimator->BonesChanged();
	}
}



// Controller Management
//////////////////////////

deAnimatorController *deAnimator::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deAnimator::IndexOfController( deAnimatorController *controller ) const{
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ] == controller ){
			return i;
		}
	}
	
	return -1;
}

int deAnimator::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ]->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool deAnimator::HasController( deAnimatorController *controller ) const{
	if( ! controller ) DETHROW( deeInvalidParam );
	
	int i;
	
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ] == controller ){
			return true;
		}
	}
	
	return false;
}

void deAnimator::AddController( deAnimatorController *controller ){
	if( ! controller ) DETHROW( deeInvalidParam );
	
	if( pControllerCount == pControllerSize ){
		int i, newSize = pControllerSize * 3 / 2 + 1;
		deAnimatorController **newArray = new deAnimatorController*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pControllers ){
			for( i=0; i<pControllerSize; i++ ) newArray[ i ] = pControllers[ i ];
			delete [] pControllers;
		}
		pControllers = newArray;
		pControllerSize = newSize;
	}
	pControllers[ pControllerCount ] = controller;
	pControllerCount++;
	
	if( pPeerAnimator ){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::RemoveController( deAnimatorController *controller ){
	int index = IndexOfController( controller );
	if( index == -1 ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=index+1; i<pControllerCount; i++ ){
		pControllers[ i - 1 ] = pControllers[ i ];
	}
	pControllerCount--;
	delete controller;
	
	if( pPeerAnimator ){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::RemoveAllControllers(){
	while( pControllerCount > 0 ){
		delete pControllers[ pControllerCount - 1 ];
		pControllerCount--;
	}
	
	if( pPeerAnimator ){
		pPeerAnimator->ControllerCountChanged();
	}
}

void deAnimator::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllerCount ) DETHROW( deeInvalidParam );
	
	if( pPeerAnimator ){
		pPeerAnimator->ControllerChanged( index, pControllers[ index ] );
	}
}



// Link Management
////////////////////

deAnimatorLink *deAnimator::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ) DETHROW( deeInvalidParam );
	
	return pLinks[ index ];
}

int deAnimator::IndexOfLink( deAnimatorLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == link ) return i;
	}
	
	return -1;
}

bool deAnimator::HasLink( deAnimatorLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ] == link ) return true;
	}
	
	return false;
}

void deAnimator::AddLink( deAnimatorLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	if( pLinkCount == pLinkSize ){
		int i, newSize = pLinkSize * 3 / 2 + 1;
		deAnimatorLink **newArray = new deAnimatorLink*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLinks ){
			for( i=0; i<pLinkSize; i++ ) newArray[ i ] = pLinks[ i ];
			delete [] pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[ pLinkCount ] = link;
	pLinkCount++;
	
	if( pPeerAnimator ) pPeerAnimator->LinksChanged();
}

void deAnimator::RemoveLink( deAnimatorLink *link ){
	int i, index = IndexOfLink( link );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLinkCount; i++ ){
		pLinks[ i - 1 ] = pLinks[ i ];
	}
	pLinkCount--;
	
	if( pPeerAnimator ) pPeerAnimator->LinksChanged();
	
	delete link;
}

void deAnimator::RemoveAllLinks(){
	while( pLinkCount > 0 ){
		delete pLinks[ pLinkCount - 1 ];
		pLinkCount--;
	}
	
	if( pPeerAnimator ) pPeerAnimator->LinksChanged();
}

void deAnimator::NotifyLinkChangedAt( int index ){
	if( index < 0 || index >= pLinkCount ) DETHROW( deeInvalidParam );
	
	if( pPeerAnimator ) pPeerAnimator->LinksChanged();
}



// Rule Management
////////////////////

int deAnimator::GetRuleCount() const{
	return pRules.GetCount();
}

deAnimatorRule *deAnimator::GetRuleAt( int index ) const{
	return ( deAnimatorRule* )pRules.GetAt( index );
}

int deAnimator::IndexOfRule( deAnimatorRule *rule ) const{
	return pRules.IndexOf( rule );
}

bool deAnimator::HasRule( deAnimatorRule *rule ) const{
	return pRules.Has( rule );
}

void deAnimator::AddRule( deAnimatorRule *rule ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRules.Add( rule );
	
	if( pPeerAnimator ){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::RemoveRule( deAnimatorRule *rule ){
	pRules.Remove( rule );
	
	if( pPeerAnimator ){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::RemoveAllRules(){
	pRules.RemoveAll();
	
	if( pPeerAnimator ){
		pPeerAnimator->RulesChanged();
	}
}

void deAnimator::NotifyRulesChanged(){
	if( pPeerAnimator ){
		pPeerAnimator->RulesChanged();
	}
}


// System Peers
/////////////////

void deAnimator::SetPeerAnimator( deBaseAnimatorAnimator *peer ){
	if( peer == pPeerAnimator ){
		return;
	}
	
	if( pPeerAnimator ){
		delete pPeerAnimator;
	}
	pPeerAnimator = peer;
}



// Private function
/////////////////////

void deAnimator::pCleanUp(){
	if( pPeerAnimator ){
		delete pPeerAnimator;
		pPeerAnimator = NULL;
	}
	
	RemoveAllRules();
	
	RemoveAllLinks();
	if( pLinks ) delete [] pLinks;
	
	RemoveAllControllers();
	if( pControllers ){
		delete [] pControllers;
	}
}
