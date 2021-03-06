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

#include "deoglDelayedOperations.h"
#include "deoglDelayedFileWrite.h"
#include "deoglDelayedSaveImage.h"
#include "deoglDelayedDeletion.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../extensions/deoglExtResult.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/combinedTexture/deoglCombinedTexture.h"
#include "../skin/combinedTexture/deoglCombinedTextureList.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/pixelbuffer/deoglPixelBufferMipMap.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/deoglRImage.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRCamera.h"
#include "../font/deoglRFont.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/threading/deMutexGuard.h>



// Definitions
////////////////

enum eSPGenConeMap{
	spgcmWidth,
	spgcmSrcTCTransform,
	spgcmDestTCTransform,
	spgcmStepCount,
	spgcmStepFactor,
	spgcmLayer
};



// Class deoglDelayedOperations
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDelayedOperations::deoglDelayedOperations( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pHasAsyncResInitOperations( false ),
pHasInitOperations( false ),

pHasFreeOperations( false ),
pRootDeletion( NULL ),
pTailDeletion( NULL ),
pDeletionCount( 0 ),
pHasSynchronizeOperations( false ),

pShaderGenConeMap( NULL ),
pShaderGenConeMapLayer( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Generate ConeMap" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		pShaderGenConeMap = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		defines.AddDefine( "WITH_LAYER", "1" );
		pShaderGenConeMapLayer = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDelayedOperations::~deoglDelayedOperations(){
	pCleanUp();
}



// Delayed main thread asynchronous resources initialization
//////////////////////////////////////////////////////////////

void deoglDelayedOperations::ProcessAsyncResInitOperations(){
	const deMutexGuard guard( pMutexAsyncResInit );
	
	if( ! pHasAsyncResInitOperations ){
		return;
	}
	
	int i, count;
	
	// async res initialize skins
	count = pAsyncResInitSkinList.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRSkin* )pAsyncResInitSkinList.GetAt( i ) )->FinalizeAsyncResLoading();
	}
	pAsyncResInitSkinList.RemoveAll();
	
	// async res initialize skins
	count = pAsyncResInitFontList.GetCount();
	for( i=0; i<count; i++ ){
		( ( deoglRFont* )pAsyncResInitFontList.GetAt( i ) )->FinalizeAsyncResLoading();
	}
	pAsyncResInitFontList.RemoveAll();
	
	// finished
	pHasAsyncResInitOperations = false;
}



void deoglDelayedOperations::AddAsyncResInitSkin( deoglRSkin *skin ){
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexAsyncResInit );
	
	pAsyncResInitSkinList.AddIfAbsent( skin );
	pHasAsyncResInitOperations = true;
}

void deoglDelayedOperations::RemoveAsyncResInitSkin( deoglRSkin *skin ){
	const deMutexGuard guard( pMutexAsyncResInit );
	
	pAsyncResInitSkinList.RemoveIfPresent( skin );
}



void deoglDelayedOperations::AddAsyncResInitFont( deoglRFont *font ){
	if( ! font ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexAsyncResInit );
	
	pAsyncResInitFontList.AddIfAbsent( font );
	pHasAsyncResInitOperations = true;
}

void deoglDelayedOperations::RemoveAsyncResInitFont( deoglRFont *font ){
	deMutexGuard guard( pMutexAsyncResInit );
	
	pAsyncResInitFontList.RemoveIfPresent( font );
}



// Delayed render thread OpengGL initialization
/////////////////////////////////////////////////

void deoglDelayedOperations::ProcessInitOperations(){
	const deMutexGuard guard( pMutexInit );
	
	if( ! pHasInitOperations ){
		return;
	}
	
	int i, count;
	
	// initialize images
	count = pInitImageList.GetCount();
	for( i=0; i<count; i++ ){
		pProcessImage( *( ( deoglRImage* )pInitImageList.GetAt( i ) ) );
	}
	pInitImageList.RemoveAll();
	
	// initialize skins
	count = pInitSkinList.GetCount();
	for( i=0; i<count; i++ ){
		pProcessSkin( *( ( deoglRSkin* )pInitSkinList.GetAt( i ) ) );
	}
	pInitSkinList.RemoveAll();
	
	// initialize models
	count = pInitModelList.GetCount();
	for( i=0; i<count; i++ ){
		pProcessModel( *( ( deoglRModel* )pInitModelList.GetAt( i ) ) );
	}
	pInitModelList.RemoveAll();
	
	// finished
	pHasInitOperations = false;
}



void deoglDelayedOperations::AddInitImage( deoglRImage *image ){
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexInit );
	
	pInitImageList.AddIfAbsent( image );
	pHasInitOperations = true;
}

void deoglDelayedOperations::RemoveInitImage( deoglRImage *image ){
	const deMutexGuard guard( pMutexInit );
	
	pInitImageList.RemoveIfPresent( image );
}



void deoglDelayedOperations::AddInitSkin( deoglRSkin *skin ){
	if( ! skin ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexInit );
	
	pInitSkinList.AddIfAbsent( skin );
	pHasInitOperations = true;
}

void deoglDelayedOperations::RemoveInitSkin( deoglRSkin *skin ){
	const deMutexGuard guard( pMutexInit );
	
	pInitSkinList.RemoveIfPresent( skin );
}



void deoglDelayedOperations::AddInitModel( deoglRModel *model ){
	if( ! model ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexInit );
	
	pInitModelList.AddIfAbsent( model );
	pHasInitOperations = true;
}

void deoglDelayedOperations::RemoveInitModel( deoglRModel *model ){
	const deMutexGuard guard( pMutexInit );
	
	pInitModelList.RemoveIfPresent( model );
}



// Delayed render thread OpengGL deletion
///////////////////////////////////////////

void deoglDelayedOperations::ProcessFreeOperations( bool deleteAll ){
	// break reference loop: camera -> world -> component -> dynamic skin -> canvas or camera -> camera
	// this has to be done before free operations are processed since breaking these links can produce
	// free operations and adding free operations is not allowed while they are processed
	// 
	// NOTE amount of time required to clean up a camera depends on the world the camera is
	//      located in. with complex worlds this can be 35ms inaverage
	/*
	pMutexFree.Lock();
 	pRenderThread.GetLogger().LogInfoFormat( "deoglDelayedOperations.ProcessFreeOperations:"
 		" camera=%d deletion=%d", pCleanUpCameraList.GetCount(), pDeletionCount );
	pMutexFree.Unlock();
	*/
// 	decTimer timer;
	
	int count = pCleanUpCameraList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRCamera* )pCleanUpCameraList.GetAt( i ) )->SetParentWorld( NULL );
	}
	pCleanUpCameraList.RemoveAll();
// 	const float accumCamera = timer.GetElapsedTime();
	
	// process free operations. empirical values indicate 1000 deletion tasks consume on
	// average 1ms. to avoid hickups during deletion time the maximum time spend on deletion
	// is capped to 1ms on average thus 1000 deletions per call. distributing deletions across
	// multiple frame renders is not a problem since this kind of large deletion tasks happen
	// only in specific situations. caller can force deleting all tasks
	// 
	// NOTE deletion task complexity varies a lot. to create a better distribution across
	//      frames and to cut spikes a complexity value could be added to the deletion task.
	//      this would be returned by the sub-class of the deletion task and would indicate
	//      how expensive the deletion task is. then instead of the task count limit a
	//      complexity limit coudl be used. whenever a task is done its complexity is added
	//      to the complexity limit. this would allow to process tons of cheap deletion tasks
	//      while not spiking if complex ones end up in the mix.
	const deMutexGuard guard( pMutexFree );
	
	if( ! pHasFreeOperations ){
		return;
	}
	
	int countThreshold = 1000;
	
	while( pRootDeletion && ( deleteAll || countThreshold-- > 0 ) ){
		deoglDelayedDeletion * const deletion = pRootDeletion;
		pRootDeletion = pRootDeletion->GetLLNext();
		if( pRootDeletion ){
			pRootDeletion->SetLLPrev( NULL );
		}
		pDeletionCount--;
		
		deletion->DeleteObjects( pRenderThread );
		delete deletion;
	}
	
	if( pDeletionCount == 0 ){
		pTailDeletion = NULL;
	}
	
	pHasFreeOperations = pDeletionCount > 0;
	
// 	pRenderThread.GetLogger().LogInfoFormat( "deoglDelayedOperations.ProcessFreeOperations cam=%d del=%d",
// 		(int)(accumCamera * 1e6f), (int)(timer.GetElapsedTime() * 1e6f) );
}



void deoglDelayedOperations::AddDeletion( deoglDelayedDeletion *deletion ){
	if( ! deletion ){
		DETHROW( deeInvalidParam );
	}
	
	
	if( ! pMutexFree.TryLock() ){
		pRenderThread.GetLogger().LogInfo( "deoglDelayedDeletion.AddDeletion:"
			" Called while MutexFree is locked. This is a debug check only."
			" If graphic module dead-locks this could be the reason." );
		
		const decStringList output( deeInvalidAction( __FILE__, __LINE__ ).FormatOutput() );
		const int count = output.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			pRenderThread.GetLogger().LogInfo( output.GetAt( i ) );
		}
		
		pMutexFree.Lock();
	}
	
	if( pTailDeletion ){
		pTailDeletion->SetLLNext( deletion );
		deletion->SetLLPrev( pTailDeletion );
		pTailDeletion = deletion;
		
	}else{
		pRootDeletion = deletion;
		pTailDeletion = deletion;
	}
	
	pDeletionCount++;
	
	pHasFreeOperations = true;
	
	pMutexFree.Unlock();
}



// Delayed main thread synchronization
////////////////////////////////////////

void deoglDelayedOperations::ProcessSynchronizeOperations(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const deMutexGuard guard( pMutexSynchronize );
	
	if( ! pHasSynchronizeOperations ){
		return;
	}
	
	int count;
	
	count = pFileWriteList.GetCount();
	while( count > 0 ){
		deoglDelayedFileWrite * const fileWrite = ( deoglDelayedFileWrite* )pFileWriteList.GetAt( 0 );
		fileWrite->SaveFile( ogl.GetVFS() );
		pFileWriteList.RemoveFrom( 0 );
		delete fileWrite;
		count--;
	}
	
	count = pSaveImageList.GetCount();
	while( count > 0 ){
		deoglDelayedSaveImage * const saveImage = ( deoglDelayedSaveImage* )pSaveImageList.GetAt( 0 );
		saveImage->SaveImage( ogl, ogl.GetVFS() );
		pSaveImageList.RemoveFrom( 0 );
		delete saveImage;
		count--;
	}
	
	pHasSynchronizeOperations = false;
}



void deoglDelayedOperations::AddFileWrite( deoglDelayedFileWrite *fileWrite ){
	if( ! fileWrite ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexSynchronize );
	
	pFileWriteList.Add( fileWrite );
	pHasSynchronizeOperations = true;
}

void deoglDelayedOperations::AddSaveImage( deoglDelayedSaveImage *saveImage ){
	if( ! saveImage ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexSynchronize );
	
	pSaveImageList.Add( saveImage );
	pHasSynchronizeOperations = true;
}

void deoglDelayedOperations::AddCleanUpCamera( deoglRCamera *camera ){
	if( ! camera ){
		DETHROW( deeInvalidParam );
	}
	
	const deMutexGuard guard( pMutexFree );
	
	pCleanUpCameraList.AddIfAbsent( camera );
}



// Private Functions
//////////////////////

void deoglDelayedOperations::pCleanUp(){
	// free lists. this can produce deletion objects which we can report as problems
	pInitImageList.RemoveAll();
	pInitSkinList.RemoveAll();
	pInitModelList.RemoveAll();
	
	pAsyncResInitSkinList.RemoveAll();
	
	pCleanUpCameraList.RemoveAll();
	
	// free deletion objects
	if( pDeletionCount > 0 ){
		pRenderThread.GetOgl().LogWarnFormat( "%i unprocessed delayed deletions dropped", pDeletionCount );
	}
	while( pRootDeletion ){
		pTailDeletion = pRootDeletion;
		pRootDeletion = pRootDeletion->GetLLNext();
		delete pTailDeletion;
	}
	pTailDeletion = NULL;
	
	// process outstanding synchronization actions
	ProcessSynchronizeOperations();
	
	// clean up the rest
	if( pShaderGenConeMapLayer ){
		pShaderGenConeMapLayer->RemoveUsage();
		pShaderGenConeMapLayer = NULL;
	}
	if( pShaderGenConeMap ){
		pShaderGenConeMap->RemoveUsage();
		pShaderGenConeMap = NULL;
	}
}



void deoglDelayedOperations::pProcessImage( deoglRImage &image ){
	image.PrepareForRender();
}

void deoglDelayedOperations::pProcessSkin( deoglRSkin &skin ){
	const int textureCount = skin.GetTextureCount();
	int t, c;
	
	for( t=0; t<textureCount; t++ ){
		const deoglSkinTexture &skinTexture = skin.GetTextureAt( t );
		
		for( c=0; c<deoglSkinChannel::CHANNEL_COUNT; c++ ){
			deoglSkinChannel * const skinChannel = skinTexture.GetChannelAt( ( deoglSkinChannel::eChannelTypes )c );
			if( ! skinChannel ){
				continue;
			}
			
			deoglPixelBufferMipMap * const pixelBufferMipMap = skinChannel->GetPixelBufferMipMap();
			if( ! pixelBufferMipMap ){
				continue;
			}
			
			deoglPixelBuffer &basePixelBuffer = *pixelBufferMipMap->GetPixelBuffer( 0 );
			const int pixelBufferCount = pixelBufferMipMap->GetPixelBufferCount();
			deoglArrayTexture * const arrayTexture = skinChannel->GetArrayTexture();
			deoglCubeMap * const cubemap = skinChannel->GetCubeMap();
			deoglTexture * const texture = skinChannel->GetTexture();
			int l;
			
			// textures are usually compressed. if the texture compression is supported the data in the pixel buffer
			// mip map is compressed and can be send to the texture in compressed form for each mip map level. if
			// the texture compression format is though not supported a problem happens. the data has to be uploaded
			// uncompressed and the driver compresses it. for some reason certain drivers do not allow sending
			// uncompressed mip map level data and compressing them. the base level works like this but all other
			// mip map levels are considered invalid disabling mip mapping altogether. in this situation only the
			// base mip map level has to be set and the hardware asked to create mip map levels. in the worst case
			// drivers even crash. this is especially the case if the base texture is not power of two size.
			const bool compressedData = basePixelBuffer.GetCompressed();
			
			// upload texture data
			#ifdef OS_ANDROID
			/*
			pRenderThread.GetLogger().LogInfoFormat(
				"DelayedOperations SetPixels %s(%s)(%s)",
				skin.GetFilename().GetString(), skinTexture.GetName().GetString(),
				deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )c ) );
			*/
			#endif
			
			if( texture ){
				bool useCombined = false;
				
				/*
				if( c == deoglSkinChannel::ectColor ){
					if( basePixelBuffer.GetFormat() == deoglPixelBuffer::epfByte4
					|| basePixelBuffer.GetFormat() == deoglPixelBuffer::epfFloat4 ){
						useCombined = true;
					}
					
				}else if( c == deoglSkinChannel::ectReflectivityRoughness ){
					if( basePixelBuffer.GetFormat() == deoglPixelBuffer::epfByte4
					|| basePixelBuffer.GetFormat() == deoglPixelBuffer::epfFloat4 ){
						useCombined = true;
					}
				}
				*/
				
				if( useCombined ){
					const decColor &color = skinChannel->GetUniformColor();
					deoglCombinedTexture *combinedTexture;
					deoglRImage *images[ 4 ];
					
					images[ 0 ] = skinChannel->GetCombinedImage1();
					images[ 1 ] = skinChannel->GetCombinedImage1();
					images[ 2 ] = skinChannel->GetCombinedImage1();
					images[ 3 ] = skinChannel->GetCombinedImage2();
					
					combinedTexture = pRenderThread.GetTexture().GetCombinedTexture().GetWith( color, images );
					if( combinedTexture->GetUsageCount() == 1 ){
						deoglTexture * const cttexture = new deoglTexture( pRenderThread );
						
						cttexture->SetSize( texture->GetWidth(), texture->GetHeight() );
						cttexture->SetFormat( texture->GetFormat() );
						cttexture->SetMipMapped( texture->GetMipMapped() );
						
						if( compressedData ){
							//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations texture combined compressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
							for( l=0; l<pixelBufferCount; l++ ){
								cttexture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
							}
							
						}else{
							//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations texture combined uncompressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
							for( l=0; l<pixelBufferCount; l++ ){
								cttexture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
							}
						}
						
						combinedTexture->SetTexture( cttexture );
					}
					
					skinChannel->SetCombinedTexture( combinedTexture );
					texture->DestroyTexture();
					
				}else{
					if( compressedData ){
						//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations texture compressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
						for( l=0; l<pixelBufferCount; l++ ){
							texture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
						}
						
					}else{
						//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations texture uncompressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
						for( l=0; l<pixelBufferCount; l++ ){
							texture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
						}
					}
					
					/*
					if( c == deoglSkinChannel::ectHeight ){
						for( l=0; l<pixelBufferCount; l++ ){
							decString fname;
							fname.Format( "%s_height_l%i", skinTexture.GetTexture()->GetName(), l );
							pRenderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *texture, l, fname, deoglDebugSaveTexture::ecNoConversion );
						}
					}
					*/
					/*
					if( c == deoglSkinChannel::ectEmissivity || c == deoglSkinChannel::ectSolidity ){
						for( l=0; l<pixelBufferCount; l++ ){
							decString fname;
							fname.Format( "%s_c%i_l%i", skinTexture.GetTexture()->GetName(), c, l );
							pRenderThread.GetDebug().SaveTextureLevelConversion( *texture, l, fname, deoglDebugSaveTexture::ecNoConversion );
						}
					}
					*/
				}
				
			}else if( cubemap ){
				if( compressedData ){
					//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations cubemap compressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
					for( l=0; l<pixelBufferCount; l++ ){
						cubemap->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
					}
					
				}else{
					//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations cubemap uncompressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
					for( l=0; l<pixelBufferCount; l++ ){
						cubemap->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
					}
				}
				
			}else if( arrayTexture ){
				if( compressedData ){
					//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations array compressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
					for( l=0; l<pixelBufferCount; l++ ){
						arrayTexture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
					}
					
				}else{
					//pRenderThread.GetLogger().LogInfoFormat( "DelayedOperations array uncompressed %s(%i)(%i)\n", skin.GetFilename().GetString(), t, c );
					for( l=0; l<pixelBufferCount; l++ ){
						arrayTexture->SetPixelsLevel( l, *pixelBufferMipMap->GetPixelBuffer( l ) );
					}
				}
			}
			
			// if shared image is present set created texture if required and mark image in use
			if( skinChannel->GetImage() ) {
				#ifdef OS_ANDROID
				/*
				if( skinChannel->GetImage()->GetSkinUse() ){
					pRenderThread.GetLogger().LogInfoFormat(
						"DelayedOperations AlreadyShared %s(%s)(%s) %s",
						skin.GetFilename().GetString(), skinTexture.GetName().GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )c ),
						skinChannel->GetImage()->GetFilename().GetString() );
					
				}else{
					pRenderThread.GetLogger().LogInfoFormat(
						"DelayedOperations MoveTextureToSharedImage %s(%s)(%s) %s",
						skin.GetFilename().GetString(), skinTexture.GetName().GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )c ),
						skinChannel->GetImage()->GetFilename().GetString() );
				}
				*/
				#endif
				
				skinChannel->UseSharedImage();
			}
			
			/*
			// forget this... it's fast for small textures but as soon as you step it up a bit
			// the GPU just fries though and runs for ages. furthermore the results are lacking.
			// it might help with the stepping but the result is bad.
			if( c == deoglSkinChannel::ectHeight ){
				pGenerateConeMap( skin, skinTexture );
			}
			*/
			
			skinChannel->SetPixelBufferMipMap( NULL );
		}
	}
	
	skin.UpdateMemoryUsage();
	
	for( t=0; t<textureCount; t++ ){
		deoglSkinTexture &skinTexture = skin.GetTextureAt( t );
		skinTexture.PrepareParamBlock();
		skinTexture.PrepareShaders();
	}
}

void deoglDelayedOperations::pProcessModel( deoglRModel &model ){
	const int lodCount = model.GetLODCount();
	int i;
	
	//model.DebugVCOptimize();
	
	for( i=0; i<lodCount; i++ ){
		model.GetLODAt( i ).GetVBOBlock();
	}
	
	//model.PrintDebugInfo();
}

void deoglDelayedOperations::pGenerateConeMap( deoglRSkin &skin, const deoglSkinTexture &texture ){
	decTimer timer;
	
	deoglSkinChannel &channel = *texture.GetChannelAt( deoglSkinChannel::ectHeight );
	deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	deoglFramebuffer * const oldFBO = pRenderThread.GetFramebuffer().GetActive();
	const decPoint3 &size = channel.GetSize();
	deoglArrayTexture *coneMapArr = NULL;
	deoglTexture *coneMap = NULL;
	deoglFramebuffer *fbo = NULL;
	deoglShaderCompiled *shader;
	int stepCount;
	
	deoglArrayTexture * const channelArrayTexture = channel.GetArrayTexture();
	deoglTexture * const channelTexture = channel.GetTexture();
	if( ! channelTexture || ! channelArrayTexture ){
		return; // should not happen
	}
	
	deoglPixelBuffer pbConeMap( deoglPixelBuffer::epfByte1, size.x, size.y, size.z );
	pbConeMap.SetToIntColor( 255, 255, 255, 255 );
	
	stepCount = 128;
	
	OGL_CHECK( pRenderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( pRenderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( pRenderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( pRenderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( pRenderThread, pglBlendEquation( GL_MIN ) );
	OGL_CHECK( pRenderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	
	OGL_CHECK( pRenderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	try{
		// create cone map texture
		if( channelTexture ){
			coneMap = new deoglTexture( pRenderThread );
			coneMap->SetMipMapped( false );
			coneMap->SetSize( size.x, size.y );
			coneMap->SetFBOFormat( 1, false );
			coneMap->SetPixels( pbConeMap );
			
		}else{
			coneMapArr = new deoglArrayTexture( pRenderThread );
			coneMapArr->SetMipMapped( false );
			coneMapArr->SetSize( size.x, size.y, size.z );
			coneMapArr->SetFBOFormat( 1, false );
			coneMapArr->SetPixels( pbConeMap );
		}
		
		// obtain a temporary fbo and set it up
		fbo = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size.x, size.y );
		
		pRenderThread.GetFramebuffer().Activate( fbo );
		
		fbo->DetachAllImages();
		
		if( channelTexture ){
			fbo->AttachColorTexture( 0, coneMap );
			
		}else{
			fbo->AttachColorArrayTextureLayer( 0, coneMapArr, 0 );
		}
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		fbo->Verify();
		
		OGL_CHECK( pRenderThread, glViewport( 0, 0, size.x, size.y ) );
		
		// set up shader
		if( channelTexture ){
			pRenderThread.GetShader().ActivateShader( pShaderGenConeMap );
			shader = pShaderGenConeMap->GetCompiled();
			
		}else{
			pRenderThread.GetShader().ActivateShader( pShaderGenConeMapLayer );
			shader = pShaderGenConeMapLayer->GetCompiled();
		}
		
		shader->SetParameterInt( spgcmWidth, size.x );
		shader->SetParameterFloat( spgcmSrcTCTransform, 0.5f, 0.5f, 0.5f, 0.5f );
			//0.5f * ( float )( width - 1 ) / ( float )size.x,
			//0.5f * ( float )( height - 1 ) / ( float )size.y,
			//0.5f, 0.5f );
		shader->SetParameterFloat( spgcmDestTCTransform,
			1.0f / ( float )( size.x - 1 ), 1.0f / ( float )( size.y - 1 ), 0.0f, 0.0f );
			//1.0f / ( float )width, 1.0f / ( float )height,
			//0.5f / ( float )width, 0.5f / ( float )height );
		shader->SetParameterInt( spgcmStepCount, stepCount );
		shader->SetParameterFloat( spgcmStepFactor, 1.0 / ( float )stepCount );
		
		if( channelTexture ){
			tsmgr.EnableTexture( 0, *channelTexture, *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
			OGL_CHECK( pRenderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
			OGL_CHECK( pRenderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, size.x * size.y ) );
			OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
			
		}else{
			tsmgr.EnableArrayTexture( 0, *channelArrayTexture, *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
			OGL_CHECK( pRenderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
			
			int i;
			for( i=0; i<size.z; i++ ){
				// NOTE according to specs gl_Layer exists only in the geometry shader. if layered-rendering
				//      is used this requires a pass-through geometry shader setting gl_Layer which seems
				//      like a waste of processing time. On the other hand this would avoid switching the
				//      FBO target all time. check this problem out if it is really used and shows to be
				//      a problem. Using layer-rebinding is the simpler solution
				fbo->DetachAllImages();
				fbo->AttachColorArrayTextureLayer( 0, coneMapArr, i );
				shader->SetParameterFloat( spgcmLayer, ( float )i );
				OGL_CHECK( pRenderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, size.x * size.y ) );
			}
			
			OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
		}
		
		// clean up
		pRenderThread.GetFramebuffer().Activate( oldFBO );
		
		fbo->DecreaseUsageCount();
		fbo = NULL;
		
		if( coneMap ){
			pRenderThread.GetDebug().GetDebugSaveTexture().SaveTextureConversion(
				*coneMap, "generate_conemap", deoglDebugSaveTexture::ecNoConversion );
			delete coneMap;
			coneMap = NULL;
			
		}else{
			pRenderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureConversion(
				*coneMapArr, "generate_conemap", deoglDebugSaveTexture::ecNoConversion );
			delete coneMapArr;
			coneMapArr = NULL;
		}
		
		OGL_CHECK( pRenderThread, pglBlendEquation( GL_FUNC_ADD ) ); // important, reset it!
		
	}catch( const deException & ){
		pRenderThread.GetFramebuffer().Activate( oldFBO );
		
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		if( coneMapArr ){
			delete coneMapArr;
		}
		if( coneMap ){
			delete coneMap;
		}
		OGL_CHECK( pRenderThread, pglBlendEquation( GL_FUNC_ADD ) ); // important, reset it!
		throw;
	}
	
	pRenderThread.GetLogger().LogInfoFormat( "deoglDelayedOperations.pGenerateConeMap: Generated cone map in %ims",
		( int )( timer.GetElapsedTime() * 1000.0f ) );
}
