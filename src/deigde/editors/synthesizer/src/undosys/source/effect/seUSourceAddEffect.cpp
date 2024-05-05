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

#include "seUSourceAddEffect.h"
#include "../../../synthesizer/effect/seEffect.h"
#include "../../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceAddEffect
/////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceAddEffect::seUSourceAddEffect( seSource *source, seEffect *effect, int index ) :
pSource( NULL ),
pEffect( NULL ),
pIndex( index )
{
	if( ! source || ! effect ){
		DETHROW( deeInvalidParam );
	}
	
	pSource = source;
	source->AddReference();
	
	pEffect = effect;
	effect->AddReference();
}

seUSourceAddEffect::~seUSourceAddEffect(){
	pCleanUp();
}



// Management
///////////////

void seUSourceAddEffect::Undo(){
	pSource->RemoveEffect( pEffect );
}

void seUSourceAddEffect::Redo(){
	pSource->InsertEffectAt( pEffect, pIndex );
}



// Private Functions
//////////////////////

void seUSourceAddEffect::pCleanUp(){
	if( pEffect ){
		pEffect->FreeReference();
	}
	if( pSource ){
		pSource->FreeReference();
	}
}
