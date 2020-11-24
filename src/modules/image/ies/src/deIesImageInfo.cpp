/* 
 * Drag[en]gine IES Photometric Image Module
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

#include "deIesImageInfo.h"

#include <dragengine/common/file/decBaseFileReader.h>


// Class deIesImageInfo
/////////////////////////

// Constructor, destructor
////////////////////////////

deIesImageInfo::deIesImageInfo() :
pWidth( 0 ),
pHeight( 0 ),
pDepth( 1 ),
pCurLine( 0 ){
}

deIesImageInfo::~deIesImageInfo(){
}


// Management
///////////////

int deIesImageInfo::GetWidth(){
	return pWidth;
}

int deIesImageInfo::GetHeight(){
	return pHeight;
}

int deIesImageInfo::GetDepth(){
	return pDepth;
}

int deIesImageInfo::GetComponentCount(){
	return 1;
}

int deIesImageInfo::GetBitCount(){
	return 16; //32;
}



// Protected Functions
////////////////////////

void deIesImageInfo::pReadLines( decBaseFileReader &reader ){
	const int length = reader.GetLength();
	decString content;
	content.Set( ' ', length );
	reader.Read( ( char* )content.GetString(), length );
	
	int position = 0;
	while( position < length ){
		const int deli = content.FindString( "\r\n", position );
		if( deli == -1 ){
			break;
		}
		
		pLines.Add( content.GetMiddle( position, deli ) );
		position = deli + 2;
	}
	
	if( position < length ){
		pLines.Add( content.GetMiddle( position ) );
	}
}
