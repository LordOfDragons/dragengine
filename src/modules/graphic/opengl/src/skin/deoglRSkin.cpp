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

#include "deoglRSkin.h"
#include "deoglSkinBone.h"
#include "deoglSkinMapped.h"
#include "deoglSkinTexture.h"
#include "deoglSkinRenderable.h"
#include "deoglSkinCalculatedProperty.h"
#include "deoglSkinConstructedProperty.h"
#include "channel/deoglSkinChannel.h"
#include "visitor/deoglVSRetainImageData.h"
#include "../deoglBasics.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../memory/deoglMemoryManager.h"
#include "../renderthread/deoglLoaderThread.h"
#include "../renderthread/deoglLoaderThreadTask.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/deoglBatchedShaderLoading.h"
#include "../texture/deoglRImage.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/deSkinMapped.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/threading/deSemaphore.h>



// Class cTaskPrepareTexturePipelines
///////////////////////////////////////

class cTaskPrepareTexturePipelines : public deoglLoaderThreadTask{
private:
	deoglRSkin &pSkin;
	
public:
	cTaskPrepareTexturePipelines( deoglRSkin &skin ) : pSkin( skin ){ }
	
	virtual void Run(){
		pSkin.PrepareTexturePipelines();
	}
};



// Class deoglRSkin
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkin::deoglRSkin( deoglRenderThread &renderThread, deoglSkin &owner, const deSkin &skin ) :
pRenderThread( renderThread ),
pOwnerSkin(&owner),
pFilename( skin.GetFilename() ),

pTextures( NULL ),
pTextureCount( 0 ),

pIsSolid( true ),
pHasHoles( false ),
pHasXRay( false ),
pShadeless( false ),
pHasMirrors( false ),
pHasDynamicChannels( false ),
pHasRenderables( false ),
pReflected( epsAll ),
pShadowNone( epsNobody ),
pShadowImportance( 0 ),
pCastSolidShadow( false ),
pCastTranspShadow( false ),

pVideoPlayerCount( 0 ),

pVSRetainImageData( nullptr ),
pTexturePipelinesReady( false ),
pMemUse( renderThread.GetMemoryManager().GetConsumption().skin )
{
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	
	const int textureCount = skin.GetTextureCount();
	const int mappedCount = skin.GetMappedCount();
	int i;
	
	try{
		// created mapped
		for( i=0; i<mappedCount; i++ ){
			const deSkinMapped &mapped = *skin.GetMappedAt( i );
			const deoglSkinMapped::Ref oglMapped( deoglSkinMapped::Ref::NewWith(mapped) );
			
			if( mapped.GetInputType() == deSkinMapped::eitRenderable && ! mapped.GetRenderable().IsEmpty() ){
				oglMapped->SetRenderable( AddRenderable( mapped.GetRenderable() ) );
			}
			
			pMapped.Add( oglMapped );
		}
		
		// create textures. we create only what does not require an opengl call.
		// these are delayed until a time where we can safely create opengl objects.
		// loads textures from caches. skips already loaded shared images
		pTextures = new deoglSkinTexture*[ textureCount ];
		for( pTextureCount=0; pTextureCount<textureCount; pTextureCount++ ){
			pTextures[ pTextureCount ] = new deoglSkinTexture( renderThread,
				*this, *skin.GetTextureAt( pTextureCount ) );
		}
		
		pHasRenderables = ( pRenderables.GetCount() > 0 );
		
		// optimize if enabled
		if( renderThread.GetConfiguration().GetEnableRetainImageOptimization() ){
			// use retain image data to optimize memory allocation on CPU.
			// if enabled memory is release as soon as possible on the CPU.
			// for constructed textures though this results in reloading data over and over again
			// causing considerable slowdown on loading. the idea is good but the harsch increase
			// in loading time is bad on PC.
			// 
			// use retain image data to optimize memory allocation on CPU.
			// if enabled memory is release as soon as possible on the CPU.
			// for constructed textures though this results in reloading data over and over again
			// causing slowdown on loading.
			// 
			// on weak devices like android this helps to not get hit by the OOM killer.
			// 
			// on the PC use-case enabling the option can reduze CPU memory consumption by 50-60%
			// or between 80-170% of GPU memory used
			// 
			// another problem is with the use of loading caches. to avoid retaining image data for
			// nothing the construction process has to be changed.
			// 
			// first the texture and channels have to be inizialized enough to figure out if cache
			// loading has to be used. while doing so the property images have to be examined if
			// a shared image is present and loaded. in this case neither caching nor building the
			// texture should.
			// 
			// otherwise in cache files are loaded
			// 
			// int he next step build textures. do a retain image pass on all texture not cached.
			// this includes now also property nodes.
			// 
			// this change ensures retaining image data is reduced to the minimum. a problem still
			// persisting is for constructed textures. the image data is released after building the
			// texture. if multiple constructed textures use the same images they are loaded over
			// and over again since there is no texture generated by them and the pixel buffer is
			// dropped always
			pRetainImageData( skin );
		}
		
		// finish create textures
		for( i=0; i<pTextureCount; i++ ){
			// build channel textures not loaded from caches and not provided by already
			// loaded shared images. these are delayed until a time where we can safely
			// create opengl objects.
			pTextures[ i ]->BuildChannels( *this, *skin.GetTextureAt( i ) );
			
			// determine optimization parameters for the entire skin
			
			// TODO add the real test. this one just assumes all transparent
			// textures can be used in a quick transparent way. the real way
			// is to look if ambient, diffuse and specular are black/0.
			if( ! pTextures[ i ]->GetSolid() && ! pTextures[ i ]->GetHasHoles() ){
				pTextures[ i ]->SetQuickTransparency( true );
			}
			
			if( ! pTextures[ i ]->GetSolid() ){
				pIsSolid = false;
			}
			if( pTextures[ i ]->GetHasHoles() ){
				pHasHoles = true;
			}
			if( pTextures[ i ]->GetXRay() ){
				pHasXRay = true;
			}
			if( pTextures[ i ]->GetMirror() ){
				pHasMirrors = true;
			}
			if( pTextures[ i ]->GetDynamicChannels() ){
				pHasDynamicChannels = true;
			}
			if( pTextures[ i ]->GetShadeless() ){
				pShadeless = true;
			}
			
			// shadow states
			if( pTextures[ i ]->GetSolid() ){
				if( ! pTextures[ i ]->GetShadowNone() ){
					pCastSolidShadow = true;
				}
				
			}else{
				if( ! pTextures[ i ]->GetShadowNone() ){
					pCastTranspShadow = true;
				}
			}
			
			if( i == 0 ){
				if( pTextures[ i ]->GetShadowNone() ){
					pShadowNone = epsAll;
					
				}else{
					pShadowNone = epsNobody;
				}
				
				if( pTextures[ i ]->GetReflected() ){
					pReflected = epsAll;
					
				}else{
					pReflected = epsNobody;
				}
				
			}else{
				if( pTextures[ i ]->GetShadowNone() ){
					if( pShadowNone == epsNobody ){
						pShadowNone = epsSome;
					}
					
				}else{
					if( pShadowNone == epsAll ){
						pShadowNone = epsSome;
					}
				}
				
				if( pTextures[ i ]->GetReflected() ){
					if( pReflected == epsNobody ){
						pReflected = epsSome;
					}
					
				}else{
					if( pReflected == epsAll ){
						pReflected = epsSome;
					}
				}
			}
			
			if( pTextures[ i ]->GetShadowImportance() > pShadowImportance ){
				pShadowImportance = pTextures[ i ]->GetShadowImportance();
			}
		}
		
		if( skin.GetAsynchron() ){
			// prepare texture pipelines using the loader thread and wait for the task to
			// finish. if the loader thread is disabled do nothing. in this case delayed
			// operations will prepare the texture pipelines which is less optimal
			pRenderThread.GetLoaderThread().AwaitTask( deoglLoaderThreadTask::Ref::New(
				new cTaskPrepareTexturePipelines( *this ) ) );
			
			// register for delayed async res initialize. we do not call AddInitSkin here since
			// it is possible (albeit highly unlikely) for the render thread to run before the
			// synchronization part. but better safe than sorry
			pRenderThread.GetDelayedOperations().AddAsyncResInitSkin( this );
			
		}else{
			FinalizeAsyncResLoading();
		}
		
	}catch( const deException &e ){
		pRenderThread.GetLogger().LogException( e );
		pCleanUp();
		throw;
	}
	
	LEAK_CHECK_CREATE( renderThread, Skin );
}

deoglRSkin::~deoglRSkin(){
	LEAK_CHECK_FREE( pRenderThread, Skin );
	pCleanUp();
}



// Management
///////////////

void deoglRSkin::DropOwnerSkin(){
	pOwnerSkin = nullptr;
}

void deoglRSkin::FinalizeAsyncResLoading(){
	int i;
	for( i=0; i<pTextureCount; i++ ){
		pTextures[ i ]->FinalizeAsyncResLoading();
	}
	
	// release retained image data
	if( pVSRetainImageData ){
		#ifdef OS_ANDROID
		pRenderThread.GetLogger().LogInfoFormat(
			"RSkin.FinalizeAsyncResLoading: ReleaseRetainedData %s", pFilename.GetString() );
		#endif
		pVSRetainImageData->ReleaseImages();
		delete pVSRetainImageData;
		pVSRetainImageData = NULL;
	}
	
	// now it is safe to init skin in render thread
	pRenderThread.GetDelayedOperations().AddInitSkin( this );
}

void deoglRSkin::PrepareTexturePipelines(){
	if(pTexturePipelinesReady){
		return;
	}
	
	deoglBatchedShaderLoading batched(pRenderThread, 1000.0f, true);
	try{
		int i;
		for(i=0; i<pTextureCount; i++){
			pTextures[i]->GetPipelines().Prepare(batched);
		}
		
	}catch(const deException &){
		batched.WaitAllCompileFinished();
		throw;
	}
	
	batched.WaitAllCompileFinished();
	pTexturePipelinesReady = true;
}



deoglSkinTexture &deoglRSkin::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pTextures[ index ];
}



// Renderables
////////////////

int deoglRSkin::GetRenderableCount() const{
	return pRenderables.GetCount();
}

deoglSkinRenderable &deoglRSkin::GetRenderableAt( int index ){
	return *( ( deoglSkinRenderable* )pRenderables.GetAt( index ) );
}

int deoglRSkin::AddRenderable( const char *name ){
	int index = IndexOfRenderableNamed( name );
	
	if( index == -1 ){
		index = pRenderables.GetCount();
		
		deoglSkinRenderable * const renderable = new deoglSkinRenderable( name );
		pRenderables.Add( renderable );
		renderable->FreeReference();
	}
	
	return index;
}

int deoglRSkin::IndexOfRenderableNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pRenderables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglSkinRenderable* )pRenderables.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}



// Video Players
//////////////////

int deoglRSkin::AddVideoPlayer(){
	return pVideoPlayerCount++;
}



// Calculated properties
//////////////////////////

int deoglRSkin::GetCalculatedPropertyCount() const{
	return pCalculatedProperties.GetCount();
}

deoglSkinCalculatedProperty *deoglRSkin::GetCalculatedPropertyAt( int index ) const{
	return ( deoglSkinCalculatedProperty* )pCalculatedProperties.GetAt( index );
}

int deoglRSkin::AddCalculatedProperty( deoglSkinCalculatedProperty *calculated ){
	if( ! calculated ){
		DETHROW( deeInvalidParam );
	}
	
	pCalculatedProperties.Add( calculated );
	return pCalculatedProperties.GetCount() - 1;
}



// Mapped
///////////

int deoglRSkin::GetMappedCount() const{
	return pMapped.GetCount();
}

deoglSkinMapped *deoglRSkin::GetMappedAt( int index ) const{
	return ( deoglSkinMapped* )pMapped.GetAt( index );
}



// Constructed properties
///////////////////////////

int deoglRSkin::GetConstructedPropertyCount() const{
	return pConstructedProperties.GetCount();
}

deoglSkinConstructedProperty *deoglRSkin::GetConstructedPropertyAt( int index ) const{
	return ( deoglSkinConstructedProperty* )pConstructedProperties.GetAt( index );
}

int deoglRSkin::AddConstructedProperty( deoglSkinConstructedProperty *constructed ){
	DEASSERT_NOTNULL( constructed )
	
	pConstructedProperties.Add( constructed );
	return pConstructedProperties.GetCount() - 1;
}



// Bones
/////////

int deoglRSkin::GetBoneCount() const{
	return pBones.GetCount();
}

deoglSkinBone *deoglRSkin::GetBoneAt( int index ) const{
	return ( deoglSkinBone* )pBones.GetAt( index );
}

int deoglRSkin::AddBone( const char *name ){
	DEASSERT_NOTNULL( name )
	
	pBones.Add( deoglSkinBone::Ref::NewWith(name) );
	return pBones.GetCount() - 1;
}



void deoglRSkin::DropAllCaches(){
	int i;
	for(i=0; i<pTextureCount; i++){
		pTextures[i]->DropAllCaches();
	}
}



// Private Functions
//////////////////////

void deoglRSkin::pCleanUp(){
	if( pVSRetainImageData ){
		delete pVSRetainImageData;
		pVSRetainImageData = nullptr;
	}
	
	deoglDelayedOperations &delop = pRenderThread.GetDelayedOperations();
	delop.RemoveInitSkin(this);
	delop.RemoveAsyncResInitSkin(this);
	
	if( pTextures ){
		int i;
		for( i=0; i<pTextureCount; i++ ){
			if( pTextures[ i ] ){
				delete pTextures[ i ];
			}
		}
		delete [] pTextures;
	}
}



void deoglRSkin::pRetainImageData( const deSkin &skin ){
	if( pVSRetainImageData ){
		return;
	}
	
	#ifdef OS_ANDROID
	pRenderThread.GetLogger().LogInfoFormat( "RSkin.RetainImageData: %s", pFilename.GetString() );
	#endif
	
	deoglSkinChannel::eChannelTypes channelType;
	int i, j;
	
	for( i=0; i<pTextureCount; i++ ){
		const deSkinTexture &texture = *skin.GetTextureAt( i );
		const int propertyCount = texture.GetPropertyCount();
		
		for( j=0; j<propertyCount; j++ ){
			deSkinProperty &property = *texture.GetPropertyAt( j );
			const deoglSkinPropertyMap::ePropertyTypes propertyType =
				deoglSkinPropertyMap::GetTypeFor( property.GetType() );
			
			if( ! deoglSkinChannel::ChannelTypeMatchingPropertyType( propertyType, channelType ) ){
				continue;
			}
			
			deoglSkinChannel * const channel = pTextures[ i ]->GetChannelAt( channelType );
			if( ! channel ){
				continue;
			}
			
			if(channel->GetImage()){
				bool hasUseSkin = false;
				
				switch(channel->GetTextureType()){
				case deoglSkinChannel::ett2d:
					hasUseSkin = channel->GetImage()->GetSkinUseTexture();
					break;
					
				case deoglSkinChannel::ettCube:
					hasUseSkin = channel->GetImage()->GetSkinUseCubeMap();
					break;
					
				case deoglSkinChannel::ettArray:
					hasUseSkin = channel->GetImage()->GetSkinUseArrayTexture();
					break;
				}
				
				if(hasUseSkin){
					continue; // already loaded
				}
			}
			
			if( ! pVSRetainImageData ){
				pVSRetainImageData = new deoglVSRetainImageData;
			}
			
			if( channel->GetDelayedCombinedImage1() && channel->GetDelayedCombinedImage2() ){
				// temporary solution. for combined images we have to force locking otherwise
				// shared images being ready confuses the locking. once combined images are
				// gone this hack is not required anymore
				pVSRetainImageData->RetainPropertyImages( property, true );
				continue;
			}
			
			pVSRetainImageData->RetainPropertyImages( property, false );
		}
	}
}
