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
#include <stdlib.h>
#include <string.h>

#include "gdeFilePattern.h"
#include "gdeFilePatternList.h"

#include <dragengine/common/exceptions.h>



// Class gdeFilePattern
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeFilePatternList::gdeFilePatternList(){
}

gdeFilePatternList::gdeFilePatternList( const gdeFilePatternList &list ) :
pPatterns( list.pPatterns ){
}

gdeFilePatternList::~gdeFilePatternList(){
}



// Management
///////////////

int gdeFilePatternList::GetCount() const{
	return pPatterns.GetCount();
}

gdeFilePattern *gdeFilePatternList::GetAt( int index ) const{
	return ( gdeFilePattern* )pPatterns.GetAt( index );
}

int gdeFilePatternList::IndexOf( gdeFilePattern *pattern ) const{
	return pPatterns.IndexOf( pattern );
}

bool gdeFilePatternList::Has( gdeFilePattern *pattern ) const{
	return pPatterns.Has( pattern );
}

void gdeFilePatternList::Add( gdeFilePattern *pattern ){
	if( ! pattern || Has( pattern ) ){
		DETHROW( deeInvalidParam );
	}
	
	pPatterns.Add( pattern );
}

void gdeFilePatternList::Remove( gdeFilePattern *pattern ){
	pPatterns.Remove( pattern );
}

void gdeFilePatternList::RemoveAll(){
	pPatterns.RemoveAll();
}



gdeFilePatternList &gdeFilePatternList::operator=( const gdeFilePatternList &list ){
	pPatterns = list.pPatterns;
	return *this;
}
 
