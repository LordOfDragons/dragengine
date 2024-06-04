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
#include <string.h>
#include <stdlib.h>

#include "seULayerMoveUp.h"
#include "../../sky/seSky.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULayerMoveUp
/////////////////////////

// Constructor, destructor
////////////////////////////

seULayerMoveUp::seULayerMoveUp( seLayer *layer ) :
pLayer( NULL ),
pIndex( 0 )
{
	if( ! layer ){
		DETHROW( deeInvalidParam );
	}
	
	const seSky * const sky = layer->GetSky();
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Move Layer Up" );
	
	pIndex = sky->GetLayers().IndexOf( layer );
	if( pIndex < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pLayer = layer;
	layer->AddReference();
}

seULayerMoveUp::~seULayerMoveUp(){
	if( pLayer ){
		pLayer->FreeReference();
	}
}



// Management
///////////////

void seULayerMoveUp::Undo(){
	pLayer->GetSky()->MoveLayerTo( pLayer, pIndex );
}

void seULayerMoveUp::Redo(){
	pLayer->GetSky()->MoveLayerTo( pLayer, pIndex - 1 );
}
