/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <dragengine/common/math/decMath.h>

#include "deoglMemoryConsumptionTexture.h"
#include "deoglMemoryConsumptionTextureUse.h"
#include "../../capabilities/deoglCapsTextureFormat.h"


// Class deoglMemoryConsumptionTextureUse
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionTextureUse::deoglMemoryConsumptionTextureUse( deoglMemoryConsumptionTexture &tracker ) :
colorCompressed( tracker.colorCompressed ),
colorUncompressed( tracker.colorUncompressed ),
depthCompressed( tracker.depthCompressed ),
depthUncompressed( tracker.depthUncompressed ){
}

deoglMemoryConsumptionTextureUse::~deoglMemoryConsumptionTextureUse(){
}



// Management
///////////////

unsigned long long deoglMemoryConsumptionTextureUse::Total() const{
	return TotalCompressed() + TotalUncompressed();
}

unsigned long long deoglMemoryConsumptionTextureUse::TotalCompressed() const{
	return colorCompressed.GetConsumption() + depthCompressed.GetConsumption();
}

unsigned long long deoglMemoryConsumptionTextureUse::TotalUncompressed() const{
	return colorUncompressed.GetConsumption() + depthUncompressed.GetConsumption();
}

void deoglMemoryConsumptionTextureUse::Clear(){
	colorCompressed = 0;
	colorUncompressed = 0;
	depthCompressed = 0;
	depthUncompressed = 0;
}

void deoglMemoryConsumptionTextureUse::Set( unsigned long long consumption, bool depth, bool compressed ){
	Clear();
	
	if( depth ){
		if( compressed ){
			depthCompressed = consumption;
			
		}else{
			depthUncompressed = consumption;
		}
		
	}else{
		if( compressed ){
			colorCompressed = consumption;
			
		}else{
			colorUncompressed = consumption;
		}
	}
}

void deoglMemoryConsumptionTextureUse::SetCompressed( unsigned long long consumption,
const deoglCapsTextureFormat& format ){
	Set( consumption, format.GetIsDepth() || format.GetIsDepthFloat(), true );
}

void deoglMemoryConsumptionTextureUse::SetUncompressed( const deoglCapsTextureFormat &format,
int width, int height, int depth, int mipMapLevels ){
	unsigned long long consumption = BaseConsumption( format, width, height, depth );
	if( mipMapLevels > 0 ){
		consumption = MipMappedConsumption( mipMapLevels, width, height, consumption );
	}
	Set( consumption, format.GetIsDepth() || format.GetIsDepthFloat(), false );
}

unsigned long long deoglMemoryConsumptionTextureUse::BaseConsumption(
const deoglCapsTextureFormat &format, int width, int height, int depth ) const{
	unsigned long long consumption = ( unsigned long long )width
		* ( unsigned long long )height
		* ( unsigned long long )depth;
	
	consumption *= ( unsigned long long )( format.GetBitsPerPixel() >> 3 );
	if( ( format.GetBitsPerPixel() & 0x7 ) > 0 ){
		consumption >>= 1ull;
	}
	
	return consumption;
}

unsigned long long deoglMemoryConsumptionTextureUse::MipMappedConsumption(
int levels, int width, int height, unsigned long long baseConsumption ) const{
	int i;
	
	for( i=0; i<levels; i++ ){
		width = decMath::max( width >> 1, 1 );
		height = decMath::max( height >> 1, 1 );
		
		baseConsumption += ( unsigned long long )width * ( unsigned long long )height;
	}
	
	return baseConsumption;
}
