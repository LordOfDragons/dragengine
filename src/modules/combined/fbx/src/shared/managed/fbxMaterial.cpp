/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
