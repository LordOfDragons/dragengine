/* 
 * Drag[en]gine IGDE Font Editor
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
#include <stdlib.h>
#include <string.h>

#include "feWPUndoHistory.h"
#include "feWPUndoHistoryListener.h"
#include "../../font/feFont.h"

#include <dragengine/common/exceptions.h>



// Class feWPUndoHistory
//////////////////////////

// Constructor, destructor
////////////////////////////

feWPUndoHistory::feWPUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pFont( NULL ),
pListener( new feWPUndoHistoryListener( *this ) ){
}

feWPUndoHistory::~feWPUndoHistory(){
	SetFont( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void feWPUndoHistory::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pFont ){
		pFont->RemoveNotifier( pListener );
		pFont->FreeReference();
	}
	
	pFont = font;
	
	if( font ){
		font->AddNotifier( pListener );
		font->AddReference();
		
		SetUndoSystem( font->GetUndoSystem() );
	}
}
