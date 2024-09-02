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

#include "seUSourceSynthConTargetLess.h"
#include "../../../synthesizer/controller/seController.h"
#include "../../../synthesizer/source/seSourceSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceSynthConTargetLess
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceSynthConTargetLess::seUSourceSynthConTargetLess( seSourceSynthesizer *source ) :
pSource( NULL ),
pOldController( NULL )
{
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Synthesizer source less connection targets" );
	
	pOldController = source->GetControllerAt( source->GetConnectionCount() - 1 );
	if( pOldController ){
		pOldController->AddReference();
	}
	
	pSource = source;
	pSource->AddReference();
}

seUSourceSynthConTargetLess::~seUSourceSynthConTargetLess(){
	if( pOldController ){
		pOldController->FreeReference();
	}
	if( pSource ){
		pSource->FreeReference();
	}
}



// Management
///////////////

void seUSourceSynthConTargetLess::Undo(){
	const int index = pSource->GetConnectionCount();
	
	pSource->SetConnectionCount( index + 1 );
	pSource->SetControllerAt( index, pOldController );
}

void seUSourceSynthConTargetLess::Redo(){
	pSource->SetConnectionCount( pSource->GetConnectionCount() - 1 );
}
