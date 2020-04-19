/* 
 * Drag[en]gine IGDE World Editor
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

#include "meTerrainMaskImage.h"

#include "dragengine/common/exceptions.h"



// Definitions
////////////////

#define INV_FLOAT_255		( 1.0f / 255.0f )



// Class meTerrainMaskImage
/////////////////////////////

// Constructor, destructor
////////////////////////////

meTerrainMaskImage::meTerrainMaskImage( deImage *image ){
	if( ! image ) DETHROW( deeInvalidParam );
	
	pWidth = image->GetWidth();
	pHeight = image->GetHeight();
	
	pData8 = NULL;
	pData16 = NULL;
	pData32 = NULL;
	
	if( image->GetBitCount() == 8 ){
		pData8 = image->GetDataGrayscale8();
		
	}else if( image->GetBitCount() == 16 ){
		pData16 = image->GetDataGrayscale16();
		
	}else{
		pData32 = image->GetDataGrayscale32();
	}
}

meTerrainMaskImage::~meTerrainMaskImage(){
}



// Management
///////////////

unsigned char meTerrainMaskImage::GetMaskValueAt( int x, int y ) const{
	if( pData8 ){
		return pData8[ pWidth * y + x ].value;
		
	}else if( pData16 ){
		return ( unsigned char )( pData16[ pWidth * y + x ].value >> 8 );
		
	}else{
		return ( unsigned char )( pData32[ pWidth * y + x ].value * 255.0f );
	}
}

unsigned char meTerrainMaskImage::GetMaskValueAt( int index ) const{
	if( pData8 ){
		return pData8[ index ].value;
		
	}else if( pData16 ){
		return ( unsigned char )( pData16[ index ].value >> 8 );
		
	}else{
		return ( unsigned char )( pData32[ index ].value * 255.5f );
	}
}

void meTerrainMaskImage::SetMaskValueAt( int x, int y, unsigned char value ){
	if( pData8 ){
		pData8[ pWidth * y + x ].value = value;
		
	}else if( pData16 ){
		pData16[ pWidth * y + x ].value = ( unsigned short )value << 8;
		
	}else{
		pData32[ pWidth * y + x ].value = ( float )value * INV_FLOAT_255;
	}
}

void meTerrainMaskImage::SetMaskValueAt( int index, unsigned char value ){
	if( pData8 ){
		pData8[ index ].value = value;
		
	}else if( pData16 ){
		pData16[ index ].value = ( unsigned short )value << 8;
		
	}else{
		pData32[ index ].value = ( float )value * INV_FLOAT_255;
	}
}
