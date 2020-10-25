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

#include "deoglVSDetermineChannelFormat.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../deoglSkinPropertyMap.h"
#include "../channel/deoglSkinChannel.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../texture/deoglImage.h"
#include "../../texture/deoglRImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>



// Class deoglVSDetermineChannelFormat
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVSDetermineChannelFormat::deoglVSDetermineChannelFormat( deoglRenderThread &renderThread,
const deoglRSkin &skin, const deoglSkinTexture &oglTex, const deSkinTexture &tex ) :
pRenderThread( renderThread ),
pSkin( skin ),
pOglTex( oglTex ),
pSkinTex( tex ),
pOglChanType( deoglSkinChannel::ectColor ),
pRequiredSize( 1, 1, 1 ),
pIsUniform( true ),
pIsDynamic( false ),
pIsDefined( false ),
pRequiredComponentCount( 1 ),
pRequiresFloat( false ),
pAllowMipMap( true ),
pSharedImage( NULL ){
}

deoglVSDetermineChannelFormat::~deoglVSDetermineChannelFormat(){
}



// Management
///////////////

void deoglVSDetermineChannelFormat::ProcessChannel( deoglSkinChannel::eChannelTypes channel ){
	pRequiredSize.Set( 1, 1, 1 );
	pIsUniform = true;
	pIsDynamic = false;
	pIsDefined = false;
	pOglChanType = channel;
	pRequiredComponentCount = 1;
	pRequiresFloat = false;
	pAllowMipMap = true;
	pSharedImage = NULL;
	
	switch( channel ){
	case deoglSkinChannel::ectColorOmnidirCube:
	case deoglSkinChannel::ectEnvironmentMap:
	case deoglSkinChannel::ectEnvironmentRoom:
	case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		pRequiredSize.z = 6;
		break;
		
	default:
		break;
	}
	
	const int propertyCount = pSkinTex.GetPropertyCount();
	int i;
	
	for( i=0; i<propertyCount; i++ ){
		VisitProperty( *pSkinTex.GetPropertyAt( i ) );
	}
	
	// adjust depth if required
	switch( channel ){
	case deoglSkinChannel::ectColorOmnidirCube:
	case deoglSkinChannel::ectEnvironmentMap:
	case deoglSkinChannel::ectEnvironmentRoom:
	case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		break;
		
	default:
		if( ! pOglTex.GetVariationU() && ! pOglTex.GetVariationV() ){
			pRequiredSize.z = 1;
		}
	}
	
	// if this is a normal map channel we have to adjust the result under certain conditions
	// since a deviation map could be stored in the alpha channel
	if( channel == deoglSkinChannel::ectNormal ){
		// if the channel is not defined there is no need for a deviation map too
		if( ! pIsDefined ){
			
		// if the normal map is uniform the deviation map is 0. this does not require an alpha channel
		// if the shader disables the deviation processing. otherwise the channel is required since
		// opengl assumes 1 for the alpha channel if not defined
		}else if( pIsUniform ){
			pRequiredComponentCount = 4;
			
		// if the normal map is dynamic the calculation of the deviation map would require too much
		// additional processing time to warrant the improvement. furthermore the dynamic nature
		// of the normal map hides to some degree the lack of roughness adjustment. thus no alpha
		// channel is used in this case
		}else if( pIsDynamic ){
			
		// otherwise we have a default normal map from a texture or node source. in this case an
		// alpha channel is calculated containing the deviation
		}else{
			pRequiredComponentCount = 4;
		}
		//printf( "channel=%i, width=%i, height=%i, depth=%i, components=%i\n", pOglChanType,
		//	pRequiredWidth, pRequiredHeight, pRequiredDepth, pRequiredComponentCount );
	}
	
	// check if mip mapping is allowed
	switch( channel ){
	case deoglSkinChannel::ectColorOmnidirCube:
	case deoglSkinChannel::ectColorOmnidirEquirect:
	case deoglSkinChannel::ectEnvironmentMap:
	case deoglSkinChannel::ectEnvironmentRoom:
	case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		pAllowMipMap = false;
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::VisitProperty( deSkinProperty &property ){
	const deoglSkinPropertyMap::ePropertyTypes channelType = deoglSkinPropertyMap::GetTypeFor( property.GetType() );
	
	if( ! pChannelMatches( channelType ) ){
		return;
	}
	
	deSkinPropertyVisitorIdentify identify;
	property.Visit( identify );
	
	if( identify.IsImage() ){
		pProcessPropertyImage( identify.CastToImage(), channelType );
		
	}else if( identify.IsColor() || identify.IsVideo() ){
		pProcessPropertyColorVideo( channelType );
		
	}else if( identify.IsValue() ){
		pProcessPropertyValue( channelType );
		
	}else if( identify.IsConstructed() ){
		pProcessPropertyConstructed( identify.CastToConstructed(), channelType );
		
	}else if( identify.IsMapped() ){
		pProcessPropertyColorVideo( channelType );
		//pIsDynamic = true;
	}
}



bool deoglVSDetermineChannelFormat::SetRequiredSize( const decPoint3 &size ){
	if( pIsUniform ){
		pRequiredSize = size;
		pIsUniform = false;
		
	}else{
		if( pRequiredSize != decPoint3( 1, 1, 1 ) ){
			if( pRequiredSize != size ){
				return false;
			}
			
		}else{
			pRequiredSize = size;
		}
	}
	
	return true;
}

void deoglVSDetermineChannelFormat::SetSharedImage( deoglRImage *image ){
	if( ! image ){
		return;
	}
	
	if( ! pIsDefined ){
		pSharedImage = image;
		pIsDefined = true;
		
	}else{
		pSharedImage = NULL;
	}
}



void deoglVSDetermineChannelFormat::WarnImageNotSquareSize( const deSkinPropertyImage &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Image(%s) does not have square dimension.",
		pSkin.GetFilename().GetString(), property.GetType().GetString(),
		property.GetImage() ? property.GetImage()->GetFilename().GetString() : "" );
}

void deoglVSDetermineChannelFormat::WarnImageNotOmnidirectional( const deSkinPropertyImage &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Image(%s) is not usable for omnidirectional mapping.",
		pSkin.GetFilename().GetString(), property.GetType().GetString(),
		property.GetImage() ? property.GetImage()->GetFilename().GetString() : "" );
}

void deoglVSDetermineChannelFormat::WarnImageIncompatibleSize( const deSkinPropertyImage &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Image(%s) does not match dimension.",
		pSkin.GetFilename().GetString(), property.GetType().GetString(),
		property.GetImage() ? property.GetImage()->GetFilename().GetString() : "" );
}

void deoglVSDetermineChannelFormat::WarnImageIncompatibleComponentCount( const deSkinPropertyImage &property, int componentCount ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): %i-Component Image(%s) is not supported.",
		pSkin.GetFilename().GetString(), property.GetType().GetString(),
		componentCount, property.GetImage() ? property.GetImage()->GetFilename().GetString() : "" );
}



void deoglVSDetermineChannelFormat::WarnImageIncompatibleSize( const deSkinPropertyConstructed &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Constructed does not match dimension.",
		pSkin.GetFilename().GetString(), property.GetType().GetString() );
}

void deoglVSDetermineChannelFormat::WarnImageNotSquareSize( const deSkinPropertyConstructed &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Constructed does not have square dimension.",
		pSkin.GetFilename().GetString(), property.GetType().GetString() );
}

void deoglVSDetermineChannelFormat::WarnImageNotOmnidirectional( const deSkinPropertyConstructed &property ) const{
	pRenderThread.GetLogger().LogWarnFormat( "Skin(%s) Property(%s): Constructed is not usable for omnidirectional mapping.",
		pSkin.GetFilename().GetString(), property.GetType().GetString() );
}



// Private Functions
//////////////////////

bool deoglVSDetermineChannelFormat::pChannelMatches( const deoglSkinPropertyMap::ePropertyTypes channelType ) const{
	switch( channelType ){
	case deoglSkinPropertyMap::eptColor:
		return pOglChanType == deoglSkinChannel::ectColor;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		return pOglChanType == deoglSkinChannel::ectColorTintMask;
		
	case deoglSkinPropertyMap::eptTransparency:
		return pOglChanType == deoglSkinChannel::ectTransparency;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		return pOglChanType == deoglSkinChannel::ectColorOmnidirCube;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		return pOglChanType == deoglSkinChannel::ectColorOmnidirEquirect;
		
	case deoglSkinPropertyMap::eptSolidity:
		return pOglChanType == deoglSkinChannel::ectSolidity;
		
	case deoglSkinPropertyMap::eptNormal:
		return pOglChanType == deoglSkinChannel::ectNormal;
		
	case deoglSkinPropertyMap::eptHeight:
		return pOglChanType == deoglSkinChannel::ectHeight;
		
	case deoglSkinPropertyMap::eptEmissivity:
		return pOglChanType == deoglSkinChannel::ectEmissivity;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		return pOglChanType == deoglSkinChannel::ectRefractDistort;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		return pOglChanType == deoglSkinChannel::ectAO;
		
	case deoglSkinPropertyMap::eptReflectivity:
		return pOglChanType == deoglSkinChannel::ectReflectivity;
		
	case deoglSkinPropertyMap::eptRoughness:
		return pOglChanType == deoglSkinChannel::ectRoughness;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		return pOglChanType == deoglSkinChannel::ectEnvironmentMap;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		return pOglChanType == deoglSkinChannel::ectEnvironmentRoom;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		return pOglChanType == deoglSkinChannel::ectEnvironmentRoomMask;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		return pOglChanType == deoglSkinChannel::ectEnvironmentRoomEmissivity;
		
	case deoglSkinPropertyMap::eptAbsorption:
		return pOglChanType == deoglSkinChannel::ectAbsorption;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		return pOglChanType == deoglSkinChannel::ectRimEmissivity;
		
	default:
		return false;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyImage( const deSkinPropertyImage &property,
const deoglSkinPropertyMap::ePropertyTypes channelType ){
	const deImage * const image = property.GetImage();
	if( ! image ){
		return;
	}
	
	const int componentCount = image->GetComponentCount();
	const bool requiresFloat = ( image->GetBitCount() > 8 );
	const decPoint3 imageSize( image->GetWidth(), image->GetHeight(), image->GetDepth() );
	deoglRImage * const rimage = ( ( deoglImage* )image->GetPeerGraphic() )->GetRImage();
	
	// WARNING! the required component count is adjusted to the image component count not the
	//          actually required component count. this is required since the content of the
	//          pixelbuffer uploaded to the deoglRImage might be also used for canvas rendering
	//          or other uses. if the component count is less than the image component count
	//          information is cut and can result in strange problems. for skin textures it is
	//          not a problem to have more components than required since only the required
	//          components are accessed in shaders
	switch( channelType ){
	case deoglSkinPropertyMap::eptColor:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		if( imageSize.z == 6 ){
			if( image->GetWidth() != image->GetHeight() ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				if( pRequiredComponentCount < componentCount ){
					pRequiredComponentCount = componentCount;
				}
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		/*
		// if combined with color
		if( componentCount == 1 ){
			if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				pRequiredComponentCount = 4;
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageIncompatibleComponentCount( property, componentCount );
		}
		*/
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptNormal:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptHeight:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( pRequiredComponentCount < 2 ){ // room for cone map appended to it
				pRequiredComponentCount = 2;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptReflectivity:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		/*
		// if combined with reflectivity
		if( componentCount < 4 ){
			if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				pRequiredComponentCount = 4;
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageIncompatibleComponentCount( property, componentCount );
		}
		*/
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		if( imageSize.z == 6 ){
			if( image->GetWidth() != image->GetHeight() ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				if( pRequiredComponentCount < componentCount ){
					pRequiredComponentCount = componentCount;
				}
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		if( imageSize.z == 6 ){
			if( image->GetWidth() != image->GetHeight() ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				if( pRequiredComponentCount < componentCount ){
					pRequiredComponentCount = componentCount;
				}
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		if( imageSize.z == 6 ){
			if( image->GetWidth() != image->GetHeight() ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( imageSize ) ){
				SetSharedImage( rimage );
				pIsDefined = true;
				if( pRequiredComponentCount < componentCount ){
					pRequiredComponentCount = componentCount;
				}
				if( requiresFloat && ! pRequiresFloat ){
					pRequiresFloat = true;
				}
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptAbsorption:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		if( SetRequiredSize( imageSize ) ){
			SetSharedImage( rimage );
			pIsDefined = true;
			
			if( pRequiredComponentCount < componentCount ){
				pRequiredComponentCount = componentCount;
			}
			if( requiresFloat && ! pRequiresFloat ){
				pRequiresFloat = true;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyColorVideo( const deoglSkinPropertyMap::ePropertyTypes channelType ){
	switch( channelType ){
	case deoglSkinPropertyMap::eptColor:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		pSharedImage = NULL;
		pIsDefined = true;
		//pRequiredComponentCount = 4; // if combined with color
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptNormal:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptHeight:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 2; // cone map appended
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 2;
		break;
		
	case deoglSkinPropertyMap::eptReflectivity:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		pSharedImage = NULL;
		pIsDefined = true;
		//pRequiredComponentCount = 4; // if usinb combined
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptAbsorption:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyValue( const deoglSkinPropertyMap::ePropertyTypes channelType ){
	switch( channelType ){
	case deoglSkinPropertyMap::eptColor:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		pSharedImage = NULL;
		pIsDefined = true;
		//pRequiredComponentCount = 4; // if combined with color
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptNormal:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptHeight:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 2; // cone map appended
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 2;
		break;
		
	case deoglSkinPropertyMap::eptReflectivity:
		pSharedImage = NULL;
		pIsDefined = true;
		if( pRequiredComponentCount < 3 ){
			pRequiredComponentCount = 3;
		}
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		pSharedImage = NULL;
		pIsDefined = true;
		//pRequiredComponentCount = 4; // if using combined
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		pRequiredSize.z = 6;
		break;
		
	case deoglSkinPropertyMap::eptAbsorption:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 3;
		break;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		pSharedImage = NULL;
		pIsDefined = true;
		pRequiredComponentCount = 1;
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyConstructed( const deSkinPropertyConstructed &property,
const deoglSkinPropertyMap::ePropertyTypes channelType ){
	const decPoint3 &size = property.GetContent().GetSize();
	
	switch( channelType ){
	case deoglSkinPropertyMap::eptColor:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			if( pRequiredComponentCount < 3 ){
				pRequiredComponentCount = 3;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		if( size.z == 6 ){
			if( size.x != size.y ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( size ) ){
				pSharedImage = NULL;
				pIsDefined = true;
				pRequiredComponentCount = 3;
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		if( size.z == 1 ){
			if( size.x != size.y ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( size ) ){
				pSharedImage = NULL;
				pIsDefined = true;
				pRequiredComponentCount = 3;
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		/*
		// if combined with color
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 4;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		*/
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptNormal:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			if( pRequiredComponentCount < 3 ){
				pRequiredComponentCount = 3;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptHeight:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 2; // room for cone map appended to it
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 3;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 2;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptReflectivity:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			if( pRequiredComponentCount < 3 ){
				pRequiredComponentCount = 3;
			}
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			//pRequiredComponentCount = 4; // if using combined
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		if( size.z == 6 ){
			if( size.x != size.y ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( size ) ){
				pSharedImage = NULL;
				pIsDefined = true;
				pRequiredComponentCount = 3;
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		if( size.z == 6 ){
			if( size.x != size.y ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( size ) ){
				pSharedImage = NULL;
				pIsDefined = true;
				pRequiredComponentCount = 3;
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 1;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		if( size.z == 6 ){
			if( size.x != size.y ){
				WarnImageNotSquareSize( property );
				
			}else if( SetRequiredSize( size ) ){
				pSharedImage = NULL;
				pIsDefined = true;
				pRequiredComponentCount = 3;
				
			}else{
				WarnImageIncompatibleSize( property );
			}
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptAbsorption:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 3;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		if( SetRequiredSize( size ) ){
			pSharedImage = NULL;
			pIsDefined = true;
			pRequiredComponentCount = 3;
			
		}else{
			WarnImageIncompatibleSize( property );
		}
		break;
		
	default:
		break;
	}
}
