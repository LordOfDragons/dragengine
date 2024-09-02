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

#include "seUSourceGroupMoveSourceDown.h"
#include "../../../synthesizer/source/seSourceGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceGroupMoveSourceDown
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceGroupMoveSourceDown::seUSourceGroupMoveSourceDown( seSourceGroup *group, seSource *source ) :
pGroup( NULL ),
pSource( NULL )
{
	if( ! group || ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = group->GetSources().IndexOf( source );
	if( pIndex == -1 || pIndex == group->GetSources().GetCount() - 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGroup = group;
	group->AddReference();
	
	pSource = source;
	source->AddReference();
}

seUSourceGroupMoveSourceDown::~seUSourceGroupMoveSourceDown(){
	if( pSource ){
		pSource->FreeReference();
	}
	if( pGroup ){
		pGroup->FreeReference();
	}
}



// Management
///////////////

void seUSourceGroupMoveSourceDown::Undo(){
	pGroup->MoveSourceTo( pSource, pIndex );
}

void seUSourceGroupMoveSourceDown::Redo(){
	pGroup->MoveSourceTo( pSource, pIndex + 1 );
}
