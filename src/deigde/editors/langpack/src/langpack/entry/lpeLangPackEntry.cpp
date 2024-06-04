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

#include "lpeLangPackEntry.h"
#include "../lpeLangPack.h"

#include <dragengine/common/exceptions.h>



// Class lpeLangPackEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackEntry::lpeLangPackEntry() :
pLangPack( nullptr ),
pName( "Entry" ),
pSelected( false ),
pActive( false ){
}

lpeLangPackEntry::lpeLangPackEntry( const lpeLangPackEntry &entry ) :
pLangPack( nullptr ),
pName( entry.pName ),
pText( entry.pText ),
pSelected( false ),
pActive( false ){
}

lpeLangPackEntry::~lpeLangPackEntry(){
}



// Management
///////////////

void lpeLangPackEntry::SetLangPack( lpeLangPack *langpack ){
	pLangPack = langpack;
}



void lpeLangPackEntry::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	pName = name;
	NotifyNameChanged();
}

void lpeLangPackEntry::SetText( const decUnicodeString &text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	NotifyChanged();
}

void lpeLangPackEntry::SetSelected( bool selected ){
	pSelected = selected;
}

void lpeLangPackEntry::SetActive( bool active ){
	pActive = active;
}



void lpeLangPackEntry::NotifyChanged(){
	if( pLangPack ){
		pLangPack->NotifyEntryChanged( this );
	}
}

void lpeLangPackEntry::NotifyNameChanged(){
	if( pLangPack ){
		pLangPack->NotifyEntryNameChanged( this );
	}
}
