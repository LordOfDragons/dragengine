/* 
 * FBX Model Module
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

#include "fbxMaterial.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyLong.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxMaterial
//////////////////////

// Constructor, destructor
////////////////////////////

fbxMaterial::fbxMaterial( fbxScene &scene, fbxNode &nodeMaterial ) :
pScene( scene ),
pNodeMaterial( nodeMaterial ),
pMaterialID( nodeMaterial.GetPropertyAt( 0 )->CastLong().GetValue() ),
pName( nodeMaterial.GetPropertyAt( 1 )->CastString().GetValue() )
{
}

fbxMaterial::~fbxMaterial(){
}



// Management
///////////////

void fbxMaterial::SetName( const char *name ){
	pName = name;
}



void fbxMaterial::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	module.LogInfoFormat( "%sMaterial '%s':", prefix.GetString(), pName.GetString() );
}
