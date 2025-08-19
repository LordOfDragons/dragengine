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
pSharedImage( NULL ),
pTextureType(deoglSkinChannel::ett2d){
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
	pTextureType = deoglSkinChannel::ett2d;
	
	switch( channel ){
	case deoglSkinChannel::ectColorOmnidirCube:
	case deoglSkinChannel::ectEnvironmentMap:
	case deoglSkinChannel::ectEnvironmentRoom:
	case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		pRequiredSize.z = 6;
		pTextureType = deoglSkinChannel::ettCube;
		break;
		
	default:
		if(pOglTex.GetVariationU() || pOglTex.GetVariationV()){
			pTextureType = deoglSkinChannel::ettArray;
			
		}else{
			pTextureType = deoglSkinChannel::ett2d;
		}
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
		// opengl adds thick artifacts lines across the borders of the texture if mip mapping
		// is used. these textures to not required mip mapping anyways since they are sampled
		// independent of the model size on screen. this also reduces GPU RAM consumption
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
		
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		return pOglChanType == deoglSkinChannel::ectNonPbrAlbedo;
		
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		return pOglChanType == deoglSkinChannel::ectNonPbrMetalness;
		
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
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptRefractionDistort:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		pSetFromImage( property, rimage );
		break;
		
	case deoglSkinPropertyMap::eptHeight: // room for cone map appended to it
		pSetFromImage( property, rimage, 2 );
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSetFromImageCube( property, rimage );
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
// 		pSetFromImage( property, rimage, 4 ); // if combined with color
		pSetFromImage( property, rimage );
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
// 		pSetFromImage( property, rimage, 4 ); // if combined with reflectivity
		pSetFromImage( property, rimage );
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyColorVideo( const deoglSkinPropertyMap::ePropertyTypes channelType ){
	switch( channelType ){
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		pSetFromNoSize( 1 );
		break;
		
	case deoglSkinPropertyMap::eptHeight: // cone map appended
	case deoglSkinPropertyMap::eptRefractionDistort:
		pSetFromNoSize( 2 );
		break;
		
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		pSetFromNoSize( 3 );
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSetFromNoSize( 3, 6 );
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
// 		pSetFromNoSize( 4 ); // if combined with color
		pSetFromNoSize( 1 );
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
// 		pSetFromNoSize( 4 ); // if using combined
		pSetFromNoSize( 1 );
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyValue( const deoglSkinPropertyMap::ePropertyTypes channelType ){
	switch( channelType ){
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		pSetFromNoSize( 1 );
		break;
		
	case deoglSkinPropertyMap::eptHeight: // cone map appended
	case deoglSkinPropertyMap::eptRefractionDistort:
		pSetFromNoSize( 2 );
		break;
		
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		pSetFromNoSize( 3 );
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSetFromNoSize( 3, 6 );
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
// 		pSetFromNoSize( 4 ); // if combined with color
		pSetFromNoSize( 1 );
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
// 		pSetFromNoSize( 4 ); // if using combined
		pSetFromNoSize( 1 );
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pProcessPropertyConstructed( const deSkinPropertyConstructed &property,
const deoglSkinPropertyMap::ePropertyTypes channelType ){
	const decPoint3 &size = property.GetContent().GetSize();
	
	switch( channelType ){
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		pSetFromConstructed( property, 1 );
		break;
		
	case deoglSkinPropertyMap::eptHeight: // room for cone map appended to it
	case deoglSkinPropertyMap::eptRefractionDistort:
		pSetFromConstructed( property, 2 );
		break;
		
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		pSetFromConstructed( property, 3 );
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		pSetFromConstructedCube( property, 3 );
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		if( size.z == 1 ){
			pSetFromConstructed( property, 3 );
			
		}else{
			WarnImageNotOmnidirectional( property );
		}
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
// 		pSetFromConstructed( property, 4 ); // if combined with color
		pSetFromConstructed( property, 1 );
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
// 		pSetFromConstructed( property, 4 ); // if using combines
		pSetFromConstructed( property, 1 );
		break;
		
	default:
		break;
	}
}

void deoglVSDetermineChannelFormat::pSetFromNoSize( int requiredComponentCount, int requiredDepth ){
	pSharedImage = nullptr;
	pIsDefined = true;
	if( pRequiredComponentCount < requiredComponentCount ){
		pRequiredComponentCount = requiredComponentCount;
	}
	if( pRequiredSize.z < requiredDepth ){
		pRequiredSize.z = requiredDepth;
	}
}

void deoglVSDetermineChannelFormat::pSetFromImage( const deSkinPropertyImage &property,
deoglRImage *image, int requiredComponentCount ){
	if( SetRequiredSize( decPoint3( image->GetWidth(), image->GetHeight(), image->GetDepth() ) ) ){
		SetSharedImage( image );
		pIsDefined = true;
		
		const int componentCount = image->GetComponentCount();
		if( pRequiredComponentCount < componentCount ){
			pRequiredComponentCount = componentCount;
		}
		if( pRequiredComponentCount < requiredComponentCount ){
			pRequiredComponentCount = requiredComponentCount;
		}
		if( image->GetBitCount() > 8 && ! pRequiresFloat ){
			pRequiresFloat = true;
		}
		
	}else{
		WarnImageIncompatibleSize( property );
	}
}

void deoglVSDetermineChannelFormat::pSetFromImageCube( const deSkinPropertyImage &property,
deoglRImage* image, int requiredComponentCount ){
	if( image->GetDepth() == 6 ){
		if( image->GetWidth() != image->GetHeight() ){
			WarnImageNotSquareSize( property );
			
		}else{
			pSetFromImage( property, image, requiredComponentCount );
		}
		
	}else{
		WarnImageNotOmnidirectional( property );
	}
}

void deoglVSDetermineChannelFormat::pSetFromConstructed(
const deSkinPropertyConstructed &property, int requiredComponentCount ){
	if( SetRequiredSize( property.GetContent().GetSize() ) ){
		pSharedImage = nullptr;
		pIsDefined = true;
		if( pRequiredComponentCount < requiredComponentCount ){
			pRequiredComponentCount = requiredComponentCount;
		}
		pRequiresFloat |= property.GetBitCount() > 8;
		
	}else{
		WarnImageIncompatibleSize( property );
	}
}

void deoglVSDetermineChannelFormat::pSetFromConstructedCube(
const deSkinPropertyConstructed &property, int requiredComponentCount ){
	const decPoint3 &size = property.GetContent().GetSize();
	if( size.z == 6 ){
		if( size.x != size.y ){
			WarnImageNotSquareSize( property );
			
		}else{
			pSetFromConstructed( property, requiredComponentCount );
		}
		
	}else{
		WarnImageNotOmnidirectional( property );
	}
}
