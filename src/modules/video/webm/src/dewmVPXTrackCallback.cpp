/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include <string.h>

#include "deVideoWebm.h"
#include "dewmVPXTrackCallback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class dewmVPXTrackCallback
///////////////////////////////

// Constructor, destructor
////////////////////////////

dewmVPXTrackCallback::dewmVPXTrackCallback( deVideoWebm &module ) :
dewmTrackCallback( module ),
pInterface( nullptr ),
pWidth( 0 ),
pHeight( 0 ),
pHasTransparency( false ),
pStride( 3 ),
pContext( nullptr ),
pIterator( nullptr ),
pContextTransparency( nullptr ),
pIteratorTransparency( nullptr ),
pResBuffer( nullptr ){
}

dewmVPXTrackCallback::~dewmVPXTrackCallback(){
	if( pContextTransparency ){
		vpx_codec_destroy( pContextTransparency );
	}
	if( pContext ){
		vpx_codec_destroy( pContext );
	}
}



// Management
///////////////

void dewmVPXTrackCallback::SetResBuffer( void *buffer ){
	pResBuffer = buffer;
}



// Protected Functions
////////////////////////

bool dewmVPXTrackCallback::pOpenTrack( const webm::TrackEntry &track ){
	if( track.track_type.value() != webm::TrackType::kVideo ){
		return false;
	}
	if( pInterface || pContext ){
		return false;
	}
	
	if( track.codec_id.value() == "V_VP9" ){
		pInterface = vpx_codec_vp9_dx();
		
	}else if( track.codec_id.value() == "V_VP8" ){
		pInterface = vpx_codec_vp8_dx();
		
	}else{
		return false;
	}
	
	const webm::Video &tvideo = track.video.value();
	pWidth = ( int )tvideo.pixel_width.value();
	pHeight = ( int )tvideo.pixel_height.value();
	pHasTransparency = tvideo.alpha_mode.is_present() && tvideo.alpha_mode.value();
	pStride = pHasTransparency ? sizeof( sRGBA8 ) : sizeof( sRGB8 );
	
	// create context
	vpx_codec_dec_cfg_t config;
	config.w = ( unsigned int )pWidth;
	config.h = ( unsigned int )pHeight;
	config.threads = 1; //pModule.GetGameEngine()->GetParallelProcessing().GetCoreCount();
	
	pContext = new vpx_codec_ctx_t;
	memset( pContext, 0, sizeof( vpx_codec_ctx_t ) );
	
	DEASSERT_TRUE( vpx_codec_dec_init( pContext, pInterface, &config, 0 ) == VPX_CODEC_OK )
	pIterator = nullptr;
	
	if( pHasTransparency ){
		pContextTransparency = new vpx_codec_ctx_t;
		memset( pContextTransparency, 0, sizeof( vpx_codec_ctx_t ) );
		
		DEASSERT_TRUE( vpx_codec_dec_init( pContextTransparency, pInterface, &config, 0 ) == VPX_CODEC_OK )
		pIteratorTransparency = nullptr;
	}
	
	return true;
}

void dewmVPXTrackCallback::pProcessFrame( webm::Reader &reader, std::uint64_t &bytes_remaining ){
	// decode data. codec requires entire frame to be present so the buffer is
	// automatically enlarged to contain at least the entire frame data.
	// after this call bytes_remaining is set to 0
	const uint64_t frameSize = bytes_remaining;
	pReadFrameData( reader, bytes_remaining );
	DEASSERT_TRUE( vpx_codec_decode( pContext, pGetBuffer(), frameSize, nullptr, 0 ) == VPX_CODEC_OK )
	
	if( ! pResBuffer ){
		// skip frame
		return;
	}
	
	// get frame image and copy it to resource buffers. we do the checks here
	// for each frame to be on the safe side in case files are corrupted
	vpx_image_t * const image = vpx_codec_get_frame( pContext, &pIterator );
	DEASSERT_NOTNULL( image )
	
	DEASSERT_TRUE( image->bit_depth == 8 )
	
	const unsigned char * const ptrY = image->planes[ 0 ];
	const unsigned char * const ptrU = image->planes[ 1 ];
	const unsigned char * const ptrV = image->planes[ 2 ];
	
	const int strideY = image->stride[ 0 ];
	const int strideU = image->stride[ 1 ];
	const int strideV = image->stride[ 2 ];
	
	uint8_t *ptrDest = ( uint8_t* )pResBuffer;
	int x, y, px, py;
	
	switch( image->fmt ){
	case VPX_IMG_FMT_I420:
		for( y=0; y<pHeight; y++ ){
			py = + pHeight - 1 - y;
			const unsigned char * const ptrLineY = ptrY + py * strideY;
			
			py >>= 1;
			const unsigned char * const ptrLineU = ptrU + py * strideU;
			const unsigned char * const ptrLineV = ptrV + py * strideV;
			
			for( x=0; x<pWidth; x++ ){
				sRGB8 &pixel = *( ( sRGB8* )ptrDest );
				ptrDest += pStride;
				
				px = x;
				pixel.red = ptrLineY[ px ];
				
				px >>= 1;
				pixel.green = ptrLineU[ px ];
				pixel.blue = ptrLineV[ px ];
			}
		}
		break;
		
	case VPX_IMG_FMT_I422:
		for( y=0; y<pHeight; y++ ){
			py = pHeight - 1 - y;
			const unsigned char * const ptrLineY = ptrY + py * strideY;
			const unsigned char * const ptrLineU = ptrU + py * strideU;
			const unsigned char * const ptrLineV = ptrV + py * strideV;
			
			for( x=0; x<pWidth; x++ ){
				sRGB8 &pixel = *( ( sRGB8* )ptrDest );
				ptrDest += pStride;
				
				px = x;
				pixel.red = ptrLineY[ px ];
				
				px >>= 1;
				pixel.green = ptrLineU[ px ];
				pixel.blue = ptrLineV[ px ];
			}
		}
		break;
		
	case VPX_IMG_FMT_I444:
		for( y=0; y<pHeight; y++ ){
			py = pHeight - 1 - y;
			const unsigned char * const ptrLineY = ptrY + py * strideY;
			const unsigned char * const ptrLineU = ptrU + py * strideU;
			const unsigned char * const ptrLineV = ptrV + py * strideV;
			
			for( x=0; x<pWidth; x++ ){
				sRGB8 &pixel = *( ( sRGB8* )ptrDest );
				ptrDest += pStride;
				
				px = x;
				pixel.red = ptrLineY[ px ];
				pixel.green = ptrLineU[ px ];
				pixel.blue = ptrLineV[ px ];
			}
		}
		break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "Unsupported video format" );
	}
}

void dewmVPXTrackCallback::pProcessAdditional( const std::vector<unsigned char> &data ){
	// transparency is done using a second stream.
	// 
	// the second stream requires a second context to process. the data of the second
	// stream is contained in additional data.
	// 
	// the additional data is processed after the block it belongs to. hence the color
	// pixels have been written already to the buffer. we can now write the alpha pixel
	// values into the buffer at the right location to complete it. this works since
	// stop processing is done when the next frame is encountered which has to be in
	// the next block.
	// 
	// only one frame can be stored in one block if transparency is used. this is due
	// to the fact that WebM requires blocks to have at most one single additional data.
	// while it would be possible to store the transparency of multiple frames in one
	// additional data it would prevent processing the data per-frame. so right now
	// it is assumed that with transparency each block has only one frame and one
	// additional
	if( ! pHasTransparency ){
		return;
	}
	
	// decode data. codec requires entire frame to be present
	DEASSERT_TRUE( vpx_codec_decode( pContextTransparency, data.data(), data.size(), nullptr, 0 ) == VPX_CODEC_OK )
	
	if( ! pResBuffer ){
		// skip frame
		return;
	}
	
	// get frame image and copy it to resource buffers. we do the checks here
	// for each frame to be on the safe side in case files are corrupted
	vpx_image_t * const image = vpx_codec_get_frame( pContextTransparency, &pIteratorTransparency );
	DEASSERT_NOTNULL( image )
	
	DEASSERT_TRUE( image->bit_depth == 8 )
	
	const unsigned char * const ptrY = image->planes[ 0 ];
	
	const int strideY = image->stride[ 0 ];
	
	uint8_t *ptrDest = ( uint8_t* )pResBuffer;
	int x, y, py;
	
	switch( image->fmt ){
	case VPX_IMG_FMT_I420:
	case VPX_IMG_FMT_I422:
	case VPX_IMG_FMT_I444:
		for( y=0; y<pHeight; y++ ){
			py = pHeight - 1 - y;
			const unsigned char * const ptrLineY = ptrY + py * strideY;
			
			for( x=0; x<pWidth; x++ ){
				( ( sRGBA8* )ptrDest )->alpha = ptrLineY[ x ];
				ptrDest += pStride;
			}
		}
		break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "Unsupported video format" );
	}
}



// Private Functions
//////////////////////
