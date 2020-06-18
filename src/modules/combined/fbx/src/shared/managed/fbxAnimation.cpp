/* 
 * FBX Modules
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
#include <inttypes.h>

#include "fbxAnimation.h"
#include "fbxAnimationMove.h"
#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimation
///////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimation::fbxAnimation( fbxScene &scene ) :
pScene( scene )
{
	decPointerList nodeStacks;
	scene.FindNodesNamed( nodeStacks, "AnimationStack" );
	
	const int nodeStackCount = nodeStacks.GetCount();
	deObjectReference refMove;
	int i;
	
	for( i=0; i<nodeStackCount; i++ ){
		refMove.TakeOver( new fbxAnimationMove( *this, *( ( fbxNode* )nodeStacks.GetAt( i ) ) ) );
		pMoves.Add( refMove );
	}
	
	const int moveCount = pMoves.GetCount();
	for( i=0; i<moveCount; i++ ){
		GetMoveAt( i )->Prepare();
	}
}

fbxAnimation::~fbxAnimation(){
}



// Management
///////////////

int fbxAnimation::GetMoveCount() const{
	return pMoves.GetCount();
}

fbxAnimationMove *fbxAnimation::GetMoveAt( int index ) const{
	return ( fbxAnimationMove* )pMoves.GetAt( index );
}

fbxAnimationMove *fbxAnimation::GetMoveNamed( const char *name ) const{
	const int count = pMoves.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		fbxAnimationMove * const move = ( fbxAnimationMove* )pMoves.GetAt( i );
		if( move->GetName() == name ){
			return move;
		}
	}
	return NULL;
}

void fbxAnimation::MatchRig( const fbxRig &rig ){
	const int count = pMoves.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( fbxAnimationMove* )( deObject* )pMoves.GetAt( i ) )->MatchRig( rig );
	}
}



void fbxAnimation::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	const int count = pMoves.GetCount();
	int i;
	
	module.LogInfoFormat( "%sAnimation", prefix.GetString() );
	
	const decString childPrefix( prefix + "  " );
	for( i=0; i<count; i++ ){
		GetMoveAt( i )->DebugPrintStructure( module, childPrefix, verbose );
	}
}



// Private Functions
//////////////////////
