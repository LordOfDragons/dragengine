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

#include "deImage.h"
#include "deImageManager.h"
#include "../../common/exceptions.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/utils/decXpmImage.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/image/deBaseImageModule.h"
#include "../../systems/modules/image/deBaseImageInfo.h"
#include "../../systems/modules/graphic/deBaseGraphicImage.h"
#include "../../threading/deMutexGuard.h"



// Class deImage
//////////////////

// Constructor, destructor
////////////////////////////

deImage::deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, int width, int height, int depth,
	int componentCount, int bitCount ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( width ),
pHeight( height ),
pDepth( depth ),
pComponentCount( componentCount ),
pBitCount( bitCount ),
pData( NULL ),
pRetainImageData( GetFilename().IsEmpty() ? 1 : 0 ),
pPeerGraphic( NULL )
{
	DEASSERT_TRUE( width >= 1 )
	DEASSERT_TRUE( height >= 1 )
	DEASSERT_TRUE( depth >= 1 )
	DEASSERT_TRUE( componentCount >= 1 && componentCount <= 4 )
	DEASSERT_TRUE( bitCount == 8 || bitCount == 16 || bitCount == 32 )
	
	pData = new unsigned char[ width * height * depth * componentCount * ( bitCount >> 3 ) ];
}

deImage::deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime, decXpmImage *image ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( 0 ),
pHeight( 0 ),
pDepth( 1 ),
pComponentCount( 3 ),
pBitCount( 8 ),
pData( NULL ),
pRetainImageData( 1 ),
pPeerGraphic( NULL )
{
	DEASSERT_NOTNULL( image )
	
	pWidth = image->GetWidth();
	pHeight = image->GetHeight();
	
	pData = new unsigned char[ pWidth * pHeight * pComponentCount * ( pBitCount >> 3 ) ];
	
	int x, y;
	for( y=0; y<pHeight; y++ ){
		const sRGBA8 * const xpmData = ( sRGBA8* )( image->GetData() ) + pWidth * ( pHeight - 1 - y );
		sRGB8 * const imgData = ( sRGB8* )( pData ) + pWidth * y;
		
		for( x=0; x<pWidth; x++ ){
			imgData[ x ].red = xpmData[ x ].red;
			imgData[ x ].green = xpmData[ x ].green;
			imgData[ x ].blue = xpmData[ x ].blue;
		}
	}
}

deImage::deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pWidth( 0 ),
pHeight( 0 ),
pDepth( 0 ),
pComponentCount( 0 ),
pBitCount( 0 ),
pData( NULL ),
pRetainImageData( GetFilename().IsEmpty() ? 1 : 0 ),
pPeerGraphic( NULL ){
}

deImage::~deImage(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	if( pData ){
		delete [] pData;
	}
}



// Management
///////////////

sGrayscale8 *deImage::GetDataGrayscale8() const{
	if( pComponentCount != 1 || pBitCount != 8 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscale8* )pData;
}

sGrayscale16 *deImage::GetDataGrayscale16() const{
	if( pComponentCount != 1 || pBitCount != 16 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscale16* )pData;
}

sGrayscale32 *deImage::GetDataGrayscale32() const{
	if( pComponentCount != 1 || pBitCount != 32 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscale32* )pData;
}

sGrayscaleAlpha8 *deImage::GetDataGrayscaleAlpha8() const{
	if( pComponentCount != 2 || pBitCount != 8 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscaleAlpha8* )pData;
}

sGrayscaleAlpha16 *deImage::GetDataGrayscaleAlpha16() const{
	if( pComponentCount != 2 || pBitCount != 16 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscaleAlpha16* )pData;
}

sGrayscaleAlpha32 *deImage::GetDataGrayscaleAlpha32() const{
	if( pComponentCount != 2 || pBitCount != 32 ){
		DETHROW( deeInvalidParam );
	}
	return ( sGrayscaleAlpha32* )pData;
}

sRGB8 *deImage::GetDataRGB8() const{
	if( pComponentCount != 3 || pBitCount != 8 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGB8* )pData;
}

sRGB16 *deImage::GetDataRGB16() const{
	if( pComponentCount != 3 || pBitCount != 16 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGB16* )pData;
}

sRGB32 *deImage::GetDataRGB32() const{
	if( pComponentCount != 3 || pBitCount != 32 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGB32* )pData;
}

sRGBA8 *deImage::GetDataRGBA8() const{
	if( pComponentCount != 4 || pBitCount != 8 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGBA8* )pData;
}

sRGBA16 *deImage::GetDataRGBA16() const{
	if( pComponentCount != 4 || pBitCount != 16 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGBA16* )pData;
}

sRGBA32 *deImage::GetDataRGBA32() const{
	if( pComponentCount != 4 || pBitCount != 32 ){
		DETHROW( deeInvalidParam );
	}
	return ( sRGBA32* )pData;
}

void deImage::NotifyImageDataChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ImageDataChanged();
	}
}



void deImage::RetainImageData(){
	const deMutexGuard guard( pMutex );
	
	if( pRetainImageData > 0 ){
		pRetainImageData++;
		return;
	}
	
	// dynamic images are always retained
	if( GetFilename().IsEmpty() ){
		return;
	}
	
	// load image data
	if( ! pData ){
		deImageManager &manager = *( ( deImageManager* )GetResourceManager() );
// 		manager.LogInfoFormat( "Retain image '%s'", GetFilename().GetString() );
		decBaseFileReader::Ref fileReader;
		deBaseImageInfo *info = NULL;
		
		try{
			deBaseImageModule * const module = ( deBaseImageModule* )manager.GetModuleSystem()->
				GetModuleAbleToLoad( deModuleSystem::emtImage, GetFilename() );
			
			fileReader.TakeOver( manager.OpenFileForReading( *GetVirtualFileSystem(), GetFilename() ) );
			
			info = module->InitLoadImage( fileReader );
			if( ! info ){
				DETHROW( deeInvalidParam );
			}
			
			if( info->GetWidth() != pWidth || info->GetHeight() != pHeight || info->GetDepth() != pDepth
			|| info->GetComponentCount() != pComponentCount || info->GetBitCount() != pBitCount ){
				DETHROW( deeInvalidParam );
			}
			
			pData = new unsigned char[ pWidth * pHeight * pDepth * pComponentCount * ( pBitCount >> 3 ) ];
			
			module->LoadImage( fileReader, *this, *info );
			delete info;
			
		}catch( const deException & ){
			if( info ){
				delete info;
			}
			throw;
		}
	}
	
	pRetainImageData++; // to make sure it is incremented before notifying peers
	
	if( pPeerGraphic ){
		pPeerGraphic->ImageDataRestored();
	}
}

void deImage::ReleaseImageData(){
	const deMutexGuard guard( pMutex );
	
	if( pRetainImageData == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pRetainImageData--;
	if( pRetainImageData > 0 ){
		return;
	}
	
	// dynamic images are always retained
	if( GetFilename().IsEmpty() ){
		return;
	}
	
	// unload image data
	if( pData ){
// 		( ( deImageManager* )GetResourceManager() )->LogInfoFormat(
// 			"Release image '%s'", GetFilename().GetString() );
		delete [] pData;
		pData = NULL;
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->ImageDataReleased();
	}
}



// System Peers
/////////////////

void deImage::SetPeerGraphic( deBaseGraphicImage *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		if( ! GetAsynchron() && pPeerGraphic->RetainImageData() ){
			ReleaseImageData();
		}
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
	
	if( peer ){
		if( ! GetAsynchron() && peer->RetainImageData() ){
			RetainImageData();
		}
	}
}



// Special
////////////

void deImage::FinalizeConstruction( int width, int height, int depth, int componentCount, int bitCount ){
	DEASSERT_TRUE( width >= 1 )
	DEASSERT_TRUE( height >= 1 )
	DEASSERT_TRUE( depth >= 1 )
	DEASSERT_TRUE( componentCount >= 1 && componentCount <= 4 )
	DEASSERT_TRUE( bitCount == 8 || bitCount == 16 || bitCount == 32 )
	
	pWidth = width;
	pHeight = height;
	pDepth = depth;
	pComponentCount = componentCount;
	pBitCount = bitCount;
	
	pData = new unsigned char[ width * height * depth * componentCount * ( bitCount >> 3 ) ];
}

void deImage::PeersRetainImageData(){
	const deMutexGuard guard( pMutex );
	
	if( GetAsynchron() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic && pPeerGraphic->RetainImageData() ){
		pRetainImageData++;
	}
	
	// dynamic images are always retained
	if( GetFilename().IsEmpty() ){
		return;
	}
	
	// unload image data if retain count is zero
	if( pRetainImageData == 0 && pData ){
// 		( ( deImageManager* )GetResourceManager() )->LogInfoFormat(
// 			"(PRID) Release image '%s'", GetFilename().GetString() );
		delete [] pData;
		pData = NULL;
	}
}
