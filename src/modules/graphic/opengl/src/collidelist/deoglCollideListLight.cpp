/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglCollideListLight.h"
#include "../light/deoglRLight.h"
#include "../renderthread/deoglRenderThread.h"
#include "../occlusiontest/deoglOcclusionTest.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListLight
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListLight::deoglCollideListLight() :
pLight( NULL ),
pCulled( false ){
}

deoglCollideListLight::~deoglCollideListLight(){
}



// Management
///////////////

void deoglCollideListLight::Clear(){
	SetLight( NULL );
	pCulled = false;
}

void deoglCollideListLight::SetLight( deoglRLight *light ){
	pLight = light;
}

void deoglCollideListLight::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListLight::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &cameraPosition ){
	if( ! pLight ){
		DETHROW( deeInvalidParam );
	}
	
	pCulled = false;
	
	pLight->UpdateLightVolume();
	
	occlusionTest.AddInputData(
		( pLight->GetMinimumExtend() - cameraPosition ).ToVector(),
		( pLight->GetMaximumExtend() - cameraPosition ).ToVector(), this );
}

void deoglCollideListLight::OcclusionTestInvisible(){
	pCulled = true;
}
