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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglEffect.h"
#include "render/deoglREffect.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/effect/deEffect.h>



// Class deoglEffect
//////////////////////

// Constructor, destructor
////////////////////////////

deoglEffect::deoglEffect( deGraphicOpenGl &ogl, const deEffect &effect ) :
pOgl( ogl ),
pEffect( effect ),
pREffect( NULL ),
pDirtyEnabled( true ){
}

deoglEffect::~deoglEffect(){
	if( pREffect ){
		pREffect->FreeReference();
	}
}



// Management
///////////////

void deoglEffect::SyncToRender(){
	if( pDirtyEnabled ){
		pREffect->SetEnabled( pEffect.GetEnabled() );
		pDirtyEnabled = false;
	}
}



// Notifications
//////////////////

void deoglEffect::EnabledChanged(){
	pDirtyEnabled = true;
}



// Protected Functions
////////////////////////

void deoglEffect::InitREffect( deoglREffect *reffect ){
	if( pREffect ){
		DETHROW( deeInvalidParam );
	}
	pREffect = reffect;
}
