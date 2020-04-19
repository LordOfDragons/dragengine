/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalSkinTexture.h"
#include "../deAudioOpenAL.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>



// Class deoalSkinTexture
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalSkinTexture::deoalSkinTexture() :
pNoSound( false ),
pAbsorption( 0.12f ),
pAbsorptionLower( 0.35f ),
pAbsorptionUpper( 1.75f ),
pTransmission( 0.4f ),
pTransmissionLower( 2.8f ),
pTransmissionUpper( 0.28f ),
pAffectsSound( true ){
}

deoalSkinTexture::~deoalSkinTexture(){
}



// Management
///////////////

void deoalSkinTexture::ProcessProperties( const deSkinTexture &texture ){
	const int count = texture.GetPropertyCount();
	deSkinPropertyVisitorIdentify identify;
	int i;
	
	pName = texture.GetName();
	
	for( i=0; i<count; i++ ){
		deSkinProperty &property = *texture.GetPropertyAt( i );
		
		if( property.GetType() == "sound.none" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pNoSound = identify.CastToValue().GetValue() > 0.5f;
			}
			
		}else if( property.GetType() == "sound.absorption" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pAbsorption = decMath::clamp( identify.CastToValue().GetValue(), 0.0f, 1.0f );
			}
			
		}else if( property.GetType() == "sound.absorption.lower" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pAbsorptionLower = decMath::max( identify.CastToValue().GetValue(), 0.0f );
			}
			
		}else if( property.GetType() == "sound.absorption.upper" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pAbsorptionUpper = decMath::max( identify.CastToValue().GetValue(), 0.0f );
			}
			
		}else if( property.GetType() == "sound.transmission" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pTransmission = decMath::max( identify.CastToValue().GetValue(), 0.0f );
			}
			
		}else if( property.GetType() == "sound.transmission.lower" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pTransmissionLower = decMath::max( identify.CastToValue().GetValue(), 0.0f );
			}
			
		}else if( property.GetType() == "sound.transmission.upper" ){
			property.Visit( identify );
			if( identify.IsValue() ){
				pTransmissionUpper = decMath::max( identify.CastToValue().GetValue(), 0.0f );
			}
		}
	}
	
	pAffectsSound = ! pNoSound && ( pAbsorption > FLOAT_SAFE_EPSILON
		|| pTransmission < 10.0f - FLOAT_SAFE_EPSILON );
	// the threshold 10 for transmission is arbitrary. if the value is rather large chances
	// are the artist desired to have no transmission loss. the definition just speaks of
	// a large value in this case. reasonable values are though only in the range up to
	// 1-2m. 10m is usually not a reasonable value anymore
}
