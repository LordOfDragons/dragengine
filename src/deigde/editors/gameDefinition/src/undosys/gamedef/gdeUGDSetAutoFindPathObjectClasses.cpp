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

#include "gdeUGDSetAutoFindPathObjectClasses.h"
#include "../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeUGDSetAutoFindPathObjectClasses
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUGDSetAutoFindPathObjectClasses::gdeUGDSetAutoFindPathObjectClasses(
	gdeGameDefinition *gameDefinition, const decStringList &newValue ) :
pGameDefinition( NULL ),
pNewValue( newValue )
{
	if( ! gameDefinition ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Game definition set auto find path object classes" );
	
	pOldValue = gameDefinition->GetAutoFindPathObjectClasses();
	
	pGameDefinition = gameDefinition;
	gameDefinition->AddReference();
}

gdeUGDSetAutoFindPathObjectClasses::~gdeUGDSetAutoFindPathObjectClasses(){
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}



// Management
///////////////

void gdeUGDSetAutoFindPathObjectClasses::Undo(){
	pGameDefinition->SetAutoFindPathObjectClasses( pOldValue );
}

void gdeUGDSetAutoFindPathObjectClasses::Redo(){
	pGameDefinition->SetAutoFindPathObjectClasses( pNewValue );
}
