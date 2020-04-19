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
#include <stdlib.h>
#include <string.h>

#include "deoglLumimeter.h"
#include "deoglRLumimeter.h"

#include "../deGraphicOpenGl.h"

#include <dragengine/resources/sensor/deLumimeter.h>
#include <dragengine/common/exceptions.h>



// Class deoglLumimeter
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglLumimeter::deoglLumimeter( deGraphicOpenGl &ogl, const deLumimeter &lumimeter ) :
pOgl( ogl ),
pLumimeter( lumimeter ),
pRLumimeter( NULL ),

pLuminance( 0.0f ),

pDirtyLumimeter( true ),
pDirtyOctree( true )
{
	try{
		pRLumimeter = new deoglRLumimeter( ogl.GetRenderThread() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglLumimeter::~deoglLumimeter(){
	pCleanUp();
}



// Management
///////////////

void deoglLumimeter::SyncToRender(){
	if( pDirtyLumimeter ){
		pRLumimeter->SetPosition( pLumimeter.GetPosition() );
		pDirtyLumimeter = false;
	}
	
	if( pDirtyOctree ){
		pRLumimeter->UpdateOctreeNode();
		pDirtyOctree = false;
	}
	
	// TODO somehow sync measurements...
}



// Notifications
//////////////////

void deoglLumimeter::PositionChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}

void deoglLumimeter::DirectionChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}

void deoglLumimeter::ConeParameterChanged(){
	pDirtyLumimeter = true;
	pDirtyOctree = true;
}



// Measuring
//////////////

float deoglLumimeter::MeasureLuminance(){
	return pLuminance;
}

decColor deoglLumimeter::MeasureColor(){
	return pColor;
}



// Private Functions
//////////////////////

void deoglLumimeter::pCleanUp(){
	if( pRLumimeter ){
		pRLumimeter->FreeReference();
	}
}
