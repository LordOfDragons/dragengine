/* 
 * Drag[en]gine IGDE Game Definition Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gdeUCategorySetAutoCategorizePattern.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/category/gdeCategory.h"

#include <dragengine/common/exceptions.h>



// Class gdeUCategorySetAutoCategorizePattern
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUCategorySetAutoCategorizePattern::gdeUCategorySetAutoCategorizePattern(
	gdeGameDefinition *gameDefinition, gdeCategory *category, eCategoryType type,
	const decStringSet &newValue ) :
gdeUCategoryBase( gameDefinition, type ),
pCategory( NULL ),
pNewValue( newValue )
{
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Category set auto categorize pattern" );
	
	pOldValue = category->GetAutoCategorizePattern();
	
	pCategory = category;
	category->AddReference();
}

gdeUCategorySetAutoCategorizePattern::~gdeUCategorySetAutoCategorizePattern(){
	if( pCategory ){
		pCategory->FreeReference();
	}
}



// Management
///////////////

void gdeUCategorySetAutoCategorizePattern::Undo(){
	pCategory->SetAutoCategorizePattern( pOldValue );
	Notify();
}

void gdeUCategorySetAutoCategorizePattern::Redo(){
	pCategory->SetAutoCategorizePattern( pNewValue );
	Notify();
}
