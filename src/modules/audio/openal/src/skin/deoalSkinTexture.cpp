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
