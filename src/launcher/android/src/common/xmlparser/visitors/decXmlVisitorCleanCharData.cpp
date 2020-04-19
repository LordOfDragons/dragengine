/* 
 * Drag[en]gine Android Launcher
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlVisitorCleanCharData.h"
#include "../decXmlContainer.h"
#include "../decXmlCharacterData.h"
#include "../../exceptions.h"



// Class decXmlVisitorCleanCharData
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decXmlVisitorCleanCharData::decXmlVisitorCleanCharData() :
pIsFirstElement( false ),
pIsLastElement( false ){
}

decXmlVisitorCleanCharData::~decXmlVisitorCleanCharData(){
}



// Visiting
/////////////

void decXmlVisitorCleanCharData::VisitContainer( decXmlContainer &container ){
	const int count = container.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pIsFirstElement = i == 0;
		pIsLastElement = i == count - 1;
		container.GetElementAt( i )->Visit( *this );
	}
}

void decXmlVisitorCleanCharData::VisitCharacterData( decXmlCharacterData &data ){
	const decString &orgData = data.GetData();
	if( orgData.IsEmpty() ){
		return;
	}
	
	const int orgLen = orgData.GetLength();
	
	// create new buffer to hold data. it will be at most orgLen but mostly shorter.
	char * const newData = new char[ orgLen + 1 ];
	bool hasSpace = false;
	int curPos = 0;
	int orgPos;
	
	// copy over data and replace consecutive spaces with one single space. spaces before and
	// after text are ignored if the character data is the first and/or last element
	try{
		for( orgPos=0; orgPos<orgLen; orgPos++ ){
			if( IsSpace( orgData[ orgPos ] ) ){
				hasSpace = true;
				
			}else{
				if( hasSpace ){
					hasSpace = false;
					if( ! pIsFirstElement || curPos > 0 ){
						newData[ curPos++ ] = ' ';
					}
				}
				
				newData[ curPos++ ] = orgData[ orgPos ];
			}
		}
		
		if( hasSpace && ! pIsLastElement ){
			newData[ curPos++ ] = ' ';
		}
		
		newData[ curPos ] = '\0';
		
		// store the new data and free the buffer
		data.SetData( newData );
		delete [] newData;
		
	}catch( const deException & ){
		if( newData ){
			delete [] newData;
		}
		throw;
	}
}



// Private functions
//////////////////////

bool decXmlVisitorCleanCharData::IsSpace( int character ){
	switch( character ){
	case 0x20:
	case 0x9:
	case 0xD:
	case 0xA:
		return true;
		
	default:
		return false;
	}
}
