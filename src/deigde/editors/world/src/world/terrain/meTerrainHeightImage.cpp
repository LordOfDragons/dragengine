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

#include "meTerrainHeightImage.h"

#include "dragengine/resources/terrain/heightmap/deTerrainHeightMap.h"
#include "dragengine/common/exceptions.h"



// Class meTerrainHeightImage
///////////////////////////////

// Constructor, destructor
////////////////////////////

meTerrainHeightImage::meTerrainHeightImage( deImage *image ){
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

meTerrainHeightImage::~meTerrainHeightImage(){
}



// Management
///////////////

float meTerrainHeightImage::GetHeightAt( int x, int y ) const{
	if( pData8 ){
		return ( float )( pData8[ pWidth * y + x ].value - THM_8BIT_BASE ) * THM_8BIT_PTOH;
		
	}else if( pData16 ){
		return ( float )( pData16[ pWidth * y + x ].value - THM_16BIT_BASE ) * THM_16BIT_PTOH;
		
	}else{
		return pData32[ pWidth * y + x ].value;
	}
}

float meTerrainHeightImage::GetHeightAt( int index ) const{
	if( pData8 ){
		return ( float )( pData8[ index ].value - THM_8BIT_BASE ) * THM_8BIT_PTOH;
		
	}else if( pData16 ){
		return ( float )( pData16[ index ].value - THM_16BIT_BASE ) * THM_16BIT_PTOH;
		
	}else{
		return pData32[ index ].value;
	}
}

void meTerrainHeightImage::SetHeightAt( int x, int y, float height ){
	if( pData8 ){
		pData8[ pWidth * y + x ].value = ( unsigned char )( ( int )( height * THM_8BIT_HTOP ) + THM_8BIT_BASE );
		
	}else if( pData16 ){
		pData16[ pWidth * y + x ].value = ( unsigned short )( ( int )( height * THM_16BIT_HTOP ) + THM_16BIT_BASE );
		
	}else{
		pData32[ pWidth * y + x ].value = height;
	}
}

void meTerrainHeightImage::SetHeightAt( int index, float height ){
	if( pData8 ){
		pData8[ index ].value = ( unsigned char )( ( int )( height * THM_8BIT_HTOP ) + THM_8BIT_BASE );
		
	}else if( pData16 ){
		pData16[ index ].value = ( unsigned short )( ( int )( height * THM_16BIT_HTOP ) + THM_16BIT_BASE );
		
	}else{
		pData32[ index ].value = height;
	}
}
