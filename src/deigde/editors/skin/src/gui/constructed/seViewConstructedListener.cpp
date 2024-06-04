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

#include "seViewConstructed.h"
#include "seViewConstructedListener.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/environment/igdeEnvironment.h>



// Class seViewConstructedListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructedListener::seViewConstructedListener( seViewConstructed &view ) :
pView( view ){
}

seViewConstructedListener::~seViewConstructedListener(){
}



// Management
///////////////

void seViewConstructedListener::ActiveTextureChanged( seSkin *skin ){
	if( pView.GetSkin() != skin ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::ActivePropertyChanged( seSkin *skin, seTexture *texture ){
	if( pView.GetSkin() != skin || ! texture->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyNodeChanged( seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode *node ){
	if( pView.GetSkin() != skin || ! texture->GetActive()
	|| ! property->GetActive() || node != property->GetNodeGroup() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}

void seViewConstructedListener::PropertyActiveNodeLayerChanged( seSkin *skin, seTexture *texture,
seProperty *property ){
	if( pView.GetSkin() != skin || ! texture->GetActive() || ! property->GetActive() ){
		return;
	}
	
	pView.UpdateLayerRanges();
	pView.UpdateScrollbarRanges();
}
