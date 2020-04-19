/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include "deoglTerrainMaskImage.h"
#include "dragengine/resources/terrain/heightmap/deTerrainHeightMap.h"
#include "dragengine/common/exceptions.h"



// Class deoglTerrainMaskImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTerrainMaskImage::deoglTerrainMaskImage( deImage &image ) :
pImage( image ),
pWidth( image.GetWidth() ),
pHeight( image.GetHeight() ),
pData8( NULL ),
pData16( NULL ),
pData32( NULL ),
pRetained( false )
{
	image.RetainImageData();
	pRetained = true;
	
	if( image.GetBitCount() == 8 ){
		pData8 = image.GetDataGrayscale8();
		if( ! pData8 ){
			DETHROW( deeInvalidParam );
		}
		
	}else if( image.GetBitCount() == 16 ){
		pData16 = image.GetDataGrayscale16();
		if( ! pData16 ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		pData32 = image.GetDataGrayscale32();
		if( ! pData32 ){
			DETHROW( deeInvalidParam );
		}
	}
}

deoglTerrainMaskImage::~deoglTerrainMaskImage(){
	if( pRetained ){
		pImage.ReleaseImageData();
	}
}



// Management
///////////////

float deoglTerrainMaskImage::GetMaskValueAt( int x, int y ) const{
	if( pData8 ){
		return ( float )( pData8[ pWidth * y + x ].value ) * THM_8BIT_PTOH;
		
	}else if( pData16 ){
		return ( float )( pData16[ pWidth * y + x ].value ) * THM_16BIT_PTOH;
		
	}else{
		return pData32[ pWidth * y + x ].value;
	}
}

float deoglTerrainMaskImage::GetMaskValueAt( int index ) const{
	if( pData8 ){
		return ( float )( pData8[ index ].value ) * THM_8BIT_PTOH;
		
	}else if( pData16 ){
		return ( float )( pData16[ index ].value ) * THM_16BIT_PTOH;
		
	}else{
		return pData32[ index ].value;
	}
}
