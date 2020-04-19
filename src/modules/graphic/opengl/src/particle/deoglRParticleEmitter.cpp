/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterType.h"

#include "../renderthread/deoglRenderThread.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>



// Class deoglRParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitter::deoglRParticleEmitter( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pVBOLayoutShared( NULL ),
pVBOLayoutLocal( NULL ){
	LEAK_CHECK_CREATE( renderThread, ParticleEmitter );
}

deoglRParticleEmitter::~deoglRParticleEmitter(){
	LEAK_CHECK_FREE( pRenderThread, ParticleEmitter );
	if( pVBOLayoutLocal ){
		delete pVBOLayoutLocal;
	}
	if( pVBOLayoutShared ){
		delete pVBOLayoutShared;
	}
}



// Management
///////////////

deoglVBOLayout *deoglRParticleEmitter::GetVBOLayoutShared(){
	if( ! pVBOLayoutShared ){
		pVBOLayoutShared = new deoglVBOLayout;
		
		// name        | offset | type  | components
		// ------------+--------+-------+------------------------------------------------------------------
		// attribute 0 |      0 | float | lifetime, position.x, position.y, position.z
		// attribute 1 |     16 | float | linearVelocity, angularVelocity, castSize, castEmissivity
		// attribute 2 |     32 | uchar | linearDirection.x, linearDirection.y, linearDirection.z, rotation
		// attribute 3 |     36 | uchar | castRed, castGreen, castBlue, castTransparency
		pVBOLayoutShared->SetAttributeCount( 4 );
		pVBOLayoutShared->SetStride( 40 ); // best performance with multiple of 32/64?
		pVBOLayoutShared->SetSize( 0 );
		pVBOLayoutShared->SetIndexType( deoglVBOLayout::eitUnsignedShort );
		
		deoglVBOAttribute &attribute0 = pVBOLayoutShared->GetAttributeAt( 0 );
		attribute0.SetComponentCount( 4 );
		attribute0.SetDataType( deoglVBOAttribute::edtFloat );
		attribute0.SetOffset( 0 );
		
		deoglVBOAttribute &attribute1 = pVBOLayoutShared->GetAttributeAt( 1 );
		attribute1.SetComponentCount( 4 );
		attribute1.SetDataType( deoglVBOAttribute::edtFloat );
		attribute1.SetOffset( 16 );
		
		deoglVBOAttribute &attribute2 = pVBOLayoutShared->GetAttributeAt( 2 );
		attribute2.SetComponentCount( 4 );
		attribute2.SetDataType( deoglVBOAttribute::edtUByte );
		attribute2.SetOffset( 32 );
		
		deoglVBOAttribute &attribute3 = pVBOLayoutShared->GetAttributeAt( 3 );
		attribute3.SetComponentCount( 4 );
		attribute3.SetDataType( deoglVBOAttribute::edtUByte );
		attribute3.SetOffset( 36 );
	}
	
	return pVBOLayoutShared;
}

deoglVBOLayout *deoglRParticleEmitter::GetVBOLayoutLocal(){
	if( ! pVBOLayoutLocal ){
		pVBOLayoutLocal = new deoglVBOLayout;
		
		// name        | offset | type  | components
		// ------------+--------+-------+------------------------------------------------------------------
		// attribute 0 |      0 | float | beamLocation
		pVBOLayoutLocal->SetAttributeCount( 1 );
		pVBOLayoutLocal->SetStride( 4 );
		pVBOLayoutLocal->SetSize( 0 );
		pVBOLayoutLocal->SetIndexType( deoglVBOLayout::eitUnsignedShort );
		
		deoglVBOAttribute &attribute0 = pVBOLayoutLocal->GetAttributeAt( 0 );
		attribute0.SetComponentCount( 1 );
		attribute0.SetDataType( deoglVBOAttribute::edtFloat );
		attribute0.SetOffset( 0 );
	}
	
	return pVBOLayoutLocal;
}

void deoglRParticleEmitter::PrepareForRender(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterType* )pTypes.GetAt( i ) )->PrepareForRender();
	}
}



int deoglRParticleEmitter::GetTypeCount() const{
	return pTypes.GetCount();
}

deoglRParticleEmitterType &deoglRParticleEmitter::GetTypeAt( int index ) const{
	return *( ( deoglRParticleEmitterType* )pTypes.GetAt( index ) );
}

void deoglRParticleEmitter::RemoveAllTypes(){
	pTypes.RemoveAll();
}

void deoglRParticleEmitter::AddType( deoglRParticleEmitterType *type ){
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	pTypes.Add( type );
}
