/* 
 * Drag[en]gine IGDE World Editor
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

#include "meBaseUndoScale.h"
#include "../../worldedit.h"



// class meBaseUndoScale
//////////////////////////

// Constructor, destructor
////////////////////////////

meBaseUndoScale::meBaseUndoScale(){
	pModifyPosition = true;
	pModifySize = true;
	pFactors.Set( 1.0f, 1.0f, 1.0f );
	pUniformFactor = 1.0f;
	pScaleUniform = false;
	
	Update();
}

meBaseUndoScale::~meBaseUndoScale(){
}



// Management
///////////////

void meBaseUndoScale::SetModifyPosition( bool modifyPosition ){
	pModifyPosition = modifyPosition;
}

void meBaseUndoScale::SetModifySize( bool modifySize ){
	pModifySize = modifySize;
}

void meBaseUndoScale::SetFactors( const decVector &factors ){
	pFactors = factors;
	Update();
}

void meBaseUndoScale::SetUniformFactor( float factor ){
	pUniformFactor = factor;
	Update();
}

void meBaseUndoScale::SetScaleUniform( bool scaleUniform ){
	pScaleUniform = scaleUniform;
	Update();
}

void meBaseUndoScale::SetPivot( const decDVector &pivot ){
	pPivot = pivot;
	Update();
}

void meBaseUndoScale::Update(){
	// matrix... TODO
	/*
	return decMatrix::CreateTranslation( -p_center )
		* decMatrix::CreateRotationZ( -p_viewRot.z )
		* decMatrix::CreateRotationY( -p_viewRot.y )
		* decMatrix::CreateRotationX( -p_viewRot.x )
		* decMatrix::CreateScale( pScaleX, pScaleY, 1 )
		* decMatrix::CreateRotationX( p_viewRot.x )
		* decMatrix::CreateRotationY( p_viewRot.y )
		* decMatrix::CreateRotationZ( p_viewRot.z )
		* decMatrix::CreateTranslation( p_center );
	*/
	
	// set information
	decString info;
	info.Format( "factors(%g,%g,%g;%g) pivot(%g,%g,%g)",
		pFactors.x, pFactors.y, pFactors.z, pUniformFactor, pPivot.x, pPivot.y, pPivot.z );
	SetLongInfo( info );
}

void meBaseUndoScale::TransformElement( decDVector &position, decVector &scaling ){
	if( pModifySize ){
		if( pScaleUniform ){
			scaling.x *= pUniformFactor;
			scaling.y *= pUniformFactor;
			scaling.z *= pUniformFactor;
			
		}else{
			scaling.x *= pFactors.x;
			scaling.y *= pFactors.y;
			scaling.z *= pFactors.z;
		}
	}
	
	if( pModifyPosition ){
		position.x = pPivot.x + ( position.x - pPivot.x ) * ( double )pFactors.x;
		position.y = pPivot.y + ( position.y - pPivot.y ) * ( double )pFactors.y;
		position.z = pPivot.z + ( position.z - pPivot.z ) * ( double )pFactors.z;
	}
}



// Undo and Redo actions
//////////////////////////

void meBaseUndoScale::ProgressiveRedo(){
	Undo();
	Redo();
}
