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

#include "aeUControllerSetDefaultVector.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerSetDefaultVector
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerSetDefaultVector::aeUControllerSetDefaultVector(
	aeController *controller, const decVector &newVector ) :
pController( controller ),
pNewVector( newVector )
{
	DEASSERT_NOTNULL( controller )
	DEASSERT_NOTNULL( controller->GetAnimator() )
	
	pOldVector = controller->GetDefaultVector();
	SetShortInfo( "Set controller default vector" );
}

aeUControllerSetDefaultVector::~aeUControllerSetDefaultVector(){
}



// Management
///////////////

void aeUControllerSetDefaultVector::Undo(){
	pController->SetDefaultVector( pOldVector );
}

void aeUControllerSetDefaultVector::Redo(){
	pController->SetDefaultVector( pNewVector );
}
