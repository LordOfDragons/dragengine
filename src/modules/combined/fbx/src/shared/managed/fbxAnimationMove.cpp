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
#include "fbxAnimationMoveCurves.h"
#include "fbxRig.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimationMove
// ////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationMove::fbxAnimationMove( fbxAnimation &animation, fbxNode &nodeStack ) :
pAnimation( animation ),
pNodeStack( nodeStack ),
pNodeStackID( nodeStack.GetID() ),
pNodeLayer( NULL ),
pNodeLayerID( 0 ),
pName( nodeStack.GetPropertyAt( 1 )->CastString().GetValue() ),
pFrameRate( 25 )
{
	// find layer
	decPointerList connections;
	animation.GetScene().FindConnections( pNodeStackID, connections );
	int i, conCount = connections.GetCount();
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( i ) );
		if( connection.GetTarget() != pNodeStackID ){
			continue;
		}
		
		pNodeLayer = animation.GetScene().NodeWithID( connection.GetSource() );
		pNodeLayerID = pNodeLayer->GetID();
		break;
	}
	
	if( ! pNodeLayer ){
		DETHROW_INFO( deeInvalidParam, "AnimationLayer missing for AnimationStack" );
	}
	
	// find curve nodes
	deObjectReference refMove;
	
	connections.RemoveAll();
	animation.GetScene().FindConnections( pNodeLayerID, connections );
	conCount = connections.GetCount();
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( i ) );
		if( connection.GetTarget() != pNodeLayerID ){
			continue;
		}
		
		fbxNode &node = *animation.GetScene().NodeWithID( connection.GetSource() );
		if( node.GetName() == "AnimationCurveNode" ){
			refMove.TakeOver( new fbxAnimationMoveCurves( *this, node ) );
			pCurveNodes.Add( refMove );
		}
	}
}

fbxAnimationMove::~fbxAnimationMove(){
}



// Management
///////////////

void fbxAnimationMove::SetName( const char *name ){
	pName = name;
}

int fbxAnimationMove::GetCurvesCount() const{
	return pCurveNodes.GetCount();
}

fbxAnimationMoveCurves *fbxAnimationMove::GetCurvesAt( int index ) const{
	return ( fbxAnimationMoveCurves* )pCurveNodes.GetAt( index );
}

void fbxAnimationMove::MatchRig( const fbxRig &rig ){
	const int count = pCurveNodes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		fbxAnimationMoveCurves &curves = *( ( fbxAnimationMoveCurves* )pCurveNodes.GetAt( i ) );
		curves.SetRigBone( rig.GetBoneNamed( curves.GetBoneName() ) );
	}
}

void fbxAnimationMove::SetFrameRate( int frameRate ){
	pFrameRate = frameRate;
}

int fbxAnimationMove::TimeToFrame( float time ) const{
	return ( int )( ( time * pFrameRate ) + 0.5f );
}

float fbxAnimationMove::FrameToTime( int frame ) const{
	return ( float )frame / ( float )pFrameRate;
}

float fbxAnimationMove::QuantizeTime( float time ) const{
	return floorf( ( time * pFrameRate ) + 0.5f ) / ( float )pFrameRate;
}



void fbxAnimationMove::Prepare(){
	const int count = pCurveNodes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		GetCurvesAt( i )->Prepare();
	}
}



void fbxAnimationMove::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat( "%sMove '%s': %d curve nodes", prefix.GetString(), pName.GetString(), pCurveNodes.GetCount() );
	
	if( ! verbose ){
		return;
	}
	
	const int curvesCount = pCurveNodes.GetCount();
	const decString childPrefix( prefix + "  " );
	int i;
	for( i=0; i<curvesCount; i++ ){
		GetCurvesAt( i )->DebugPrintStructure( module, childPrefix, true );
	}
}
