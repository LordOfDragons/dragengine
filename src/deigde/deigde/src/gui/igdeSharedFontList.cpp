/* 
 * Drag[en]gine IGDE
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

#include "igdeSharedFontList.h"

#include <deigde/gui/resources/igdeFontReference.h>

#include <dragengine/common/exceptions.h>



// Internal Class
///////////////////

igdeSharedFontList::cFont::cFont( const igdeFont::sConfiguration &config, igdeFont *font ) :
pConfig( config )
{
	pFont.TakeOver( font );
}

igdeSharedFontList::cFont::~cFont(){
}



// Class igdeSharedFontList
/////////////////////////////

igdeSharedFontList::igdeSharedFontList( igdeEnvironment &environment ) :
pEnvironment( environment ){
}

igdeSharedFontList::~igdeSharedFontList(){
}



// Management
///////////////

igdeFont *igdeSharedFontList::GetFontWith( const igdeFont::sConfiguration &configuration ){
	const int count = pFonts.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		cFont * const cfont = ( cFont* )pFonts.GetAt( i );
		if( configuration == cfont->GetConfig() ){
			return cfont->GetFont();
		}
	}
	
	cFont * const cfont = new cFont( configuration, new igdeFont( pEnvironment, configuration ) );
	pFonts.Add( cfont );
	cfont->FreeReference();
	
	return cfont->GetFont();
}
