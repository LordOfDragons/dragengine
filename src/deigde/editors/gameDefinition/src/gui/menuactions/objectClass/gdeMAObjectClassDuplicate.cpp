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

#include "gdeMAObjectClassDuplicate.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/gdeUAddObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeMAObjectClassDuplicate
////////////////////////////////////

// Constructor
////////////////

gdeMAObjectClassDuplicate::gdeMAObjectClassDuplicate( gdeWindowMain &windowMain ) :
gdeBaseAction( windowMain, "Duplicate Object Class...",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongRight ),
	"Duplicate object class" )
{
}



// Management
///////////////

igdeUndo *gdeMAObjectClassDuplicate::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	const gdeObjectClassList &list = gameDefinition.GetObjectClasses();
	decString name( objectClass->GetName() );
	
	while( igdeCommonDialogs::GetString( &pWindowMain, "Duplicate Object Class", "Name:", name ) ){
		if( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pWindowMain, "Duplicate Object Class", "Object Class exists already." );
			continue;
		}
		
		deObjectReference duplicate;
		duplicate.TakeOver( new gdeObjectClass( *objectClass ) );
		( ( gdeObjectClass& )( deObject& )duplicate ).SetName( name );
		return new gdeUAddObjectClass( &gameDefinition, ( gdeObjectClass* )( deObject* )duplicate );
	}
	return NULL;
}

void gdeMAObjectClassDuplicate::Update(){
	gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		SetEnabled( false );
		return;
	}
	
	SetEnabled( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass 
		&& gameDefinition->GetActiveObjectClass() != NULL );
}
