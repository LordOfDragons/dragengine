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
#include <string.h>

#include "deoglRImage.h"
#include "deoglTextureStageManager.h"
#include "arraytexture/deoglArrayTexture.h"
#include "cubemap/deoglCubeMap.h"
#include "texsamplerconfig/deoglTexSamplerConfig.h"
#include "texture2d/deoglTexture.h"
#include "texture2d/deoglRenderableColorTexture.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/combinedTexture/deoglCombinedTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>



static GLenum vCubeMapFaceTarget[ 6 ] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};



// Struct deoglTextureStageManager::sStage
////////////////////////////////////////////

deoglTextureStageManager::sStage::sStage() :
texture( 0 ),
type( 0 ),
samplerConfig( 0 ){
}



// Class deoglTextureStageManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTextureStageManager::deoglTextureStageManager( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglTextureStageManager::~deoglTextureStageManager(){
}



// Management
///////////////

int deoglTextureStageManager::GetEnabledStageCount() const{
	int i;
	
	for( i=0; i<OGL_MAX_TEXTURE_STAGES; i++ ){
		if( pStages[ i ].type == 0 ){
			return i;
		}
	}
	
	return OGL_MAX_TEXTURE_STAGES;
}

GLenum deoglTextureStageManager::GetStageType( int stage ) const{
	if( stage < 0 || stage >= OGL_MAX_TEXTURE_STAGES ){
		DETHROW( deeInvalidParam );
	}
	
	return pStages[ stage ].type;
}



void deoglTextureStageManager::EnableBareArrayTexture( int stage, const deoglArrayTexture &texture ){
	BindTexture( stage, texture.GetTexture(), GL_TEXTURE_2D_ARRAY );
}

void deoglTextureStageManager::EnableBareTexture( int stage, const deoglTexture &texture ){
	BindTexture( stage, texture.GetTexture(), GL_TEXTURE_2D );
}

void deoglTextureStageManager::EnableBareCubeMap( int stage, const deoglCubeMap &cubemap ){
	BindTexture( stage, cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP );
}

void deoglTextureStageManager::EnableBareTBO( int stage, GLuint tbo ){
	BindTexture( stage, tbo, GL_TEXTURE_BUFFER );
}

void deoglTextureStageManager::EnableTexture( int stage, const deoglTexture& texture,
deoglTexSamplerConfig &samplerConfig ){
	BindTexture( stage, texture.GetTexture(), GL_TEXTURE_2D, samplerConfig.GetSamplerObject() );
}

void deoglTextureStageManager::EnableCubeMap( int stage, const deoglCubeMap &cubemap,
deoglTexSamplerConfig &samplerConfig ){
	BindTexture( stage, cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, samplerConfig.GetSamplerObject() );
}

void deoglTextureStageManager::EnableCubeMapFace( int stage, const deoglCubeMap &cubemap,
deoglCubeMap::eFaces face, deoglTexSamplerConfig &samplerConfig ){
	BindTexture( stage, cubemap.GetTexture(), vCubeMapFaceTarget[ face ], samplerConfig.GetSamplerObject() );
}

void deoglTextureStageManager::EnableArrayTexture( int stage, const deoglArrayTexture &texture,
deoglTexSamplerConfig &samplerConfig ){
	BindTexture( stage, texture.GetTexture(), GL_TEXTURE_2D_ARRAY, samplerConfig.GetSamplerObject() );
}

void deoglTextureStageManager::EnableTBO( int stage, GLuint tbo, deoglTexSamplerConfig &samplerConfig ){
	BindTexture( stage, tbo, GL_TEXTURE_BUFFER, samplerConfig.GetSamplerObject() );
}

void deoglTextureStageManager::EnableSkin( int stage, const deoglRSkin &skin, int texture,
deoglSkinChannel::eChannelTypes channel, deoglTexture *defaultTexture, deoglTexSamplerConfig &samplerConfig ){
	if( stage < 0 || stage >= OGL_MAX_TEXTURE_STAGES || ! defaultTexture ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSkinChannel * const textureChannel = skin.GetTextureAt( texture ).GetChannelAt( channel );
	deoglTexture *useTexture = defaultTexture;
	
	if( textureChannel ){
		deoglTexture *channelTexture = textureChannel->GetTexture();
		
		if( textureChannel->GetImage() ){
			channelTexture = textureChannel->GetImage()->GetTexture();
			
		}else if( textureChannel->GetCombinedTexture() ){
			channelTexture = textureChannel->GetCombinedTexture()->GetTexture();
		}
		
		if( channelTexture ){
			useTexture = channelTexture;
		}
	}
	
	EnableTexture( stage, *useTexture, samplerConfig );
}



void deoglTextureStageManager::DisableStage( int stage ){
	if( stage < 0 || stage >= OGL_MAX_TEXTURE_STAGES ){
		DETHROW( deeInvalidParam );
	}
	
	const GLenum type = pStages[ stage ].type;
	
	if( type != 0 ){
		if( pStages[ stage ].samplerConfig != 0 ){
			OGL_CHECK( pRenderThread, pglBindSampler( stage, 0 ) );
			pStages[ stage ].samplerConfig = 0;
		}
		
		if( pStages[ stage ].texture != 0 ){
			OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 + stage ) );
			OGL_CHECK( pRenderThread, glBindTexture( type, 0 ) );
			pStages[ stage ].texture = 0;
		}
		
		//OGL_CHECK( pRenderThread, glDisable( type ) );
		pStages[ stage ].type = 0;
	}
}

void deoglTextureStageManager::DisableStagesAbove( int stage ){
	GLenum type;
	int s;
	
	for( s=stage+1; s<OGL_MAX_TEXTURE_STAGES; s++ ){
		type = pStages[ s ].type;
		
		if( type != 0 ){
			if( pStages[ s ].samplerConfig != 0 ){
				OGL_CHECK( pRenderThread, pglBindSampler( s, 0 ) );
				pStages[ s ].samplerConfig = 0;
			}
			
			if( pStages[ s ].texture != 0 ){
				OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 + s ) );
				OGL_CHECK( pRenderThread, glBindTexture( type, 0 ) );
				pStages[ s ].texture = 0;
			}
			
			//OGL_CHECK( pRenderThread, glDisable( type ) );
			pStages[ s ].type = 0;
		}
	}
}

void deoglTextureStageManager::DisableAllStages(){
	GLenum type;
	int s;
	
	for( s=0; s<OGL_MAX_TEXTURE_STAGES; s++ ){
		type = pStages[ s ].type;
		
		if( type != 0 ){
			if( pStages[ s ].samplerConfig != 0 ){
				OGL_CHECK( pRenderThread, pglBindSampler( s, 0 ) );
				pStages[ s ].samplerConfig = 0;
			}
			
			if( pStages[ s ].texture != 0 ){
				OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 + s ) );
				OGL_CHECK( pRenderThread, glBindTexture( type, 0 ) );
				pStages[ s ].texture = 0;
			}
			
			//OGL_CHECK( pRenderThread, glDisable( type ) );
			pStages[ s ].type = 0;
		}
	}
}



void deoglTextureStageManager::BindTexture( int stage, GLuint texture, GLenum type ){
	DEASSERT_TRUE( stage >= 0 )
	DEASSERT_TRUE( stage < OGL_MAX_TEXTURE_STAGES )
	
	OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 + stage ) );
	
	if( texture != pStages[ stage ].texture ){
		if( type != pStages[ stage ].type ){
			/*
			if( pStages[ stage ].texture != 0 ){
				OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 + stage ) );
				OGL_CHECK( pRenderThread, glBindTexture( pStages[ stage ].type, 0 ) );
				pStages[ stage ].texture = 0;
			}
			*/
			
			/* if( pStages[ stage ].texture != 0 ){
				OGL_CHECK( pRenderThread, glBindTexture( pStages[ stage ].type, 0 ) );
			} */
			//if( pStages[ stage ].type != 0 ){
			//	OGL_CHECK( pRenderThread, glDisable( pStages[ stage ].type ) );
			//}
			//OGL_CHECK( pRenderThread, glEnable( type ) );
			pStages[ stage ].type = type;
		}
		
		#ifdef WITH_OPENGLES
		oglClearError();
		glBindTexture( type, texture );
		if(glGetError() == GL_INVALID_ENUM){
			pRenderThread.GetLogger().LogInfoFormat(
				"deoglTextureStageManager::BindTexture Failed: stage=%d texture=0x%x type=0x%x", stage, texture, type );
		}
		#else
		OGL_CHECK( pRenderThread, glBindTexture( type, texture ) );
		#endif
		pStages[ stage ].texture = texture;
	}
}

void deoglTextureStageManager::BindTexture( int stage, GLuint texture, GLenum type, GLuint samplerConfig ){
	BindTexture( stage, texture, type );
	
	if( samplerConfig != pStages[ stage ].samplerConfig ){
		OGL_CHECK( pRenderThread, pglBindSampler( stage, samplerConfig ) );
		pStages[ stage ].samplerConfig = samplerConfig;
	}
}



void deoglTextureStageManager::LogCurrentState() const{
	int i;
	
	for( i=0; i<OGL_MAX_TEXTURE_STAGES; i++ ){
		pRenderThread.GetLogger().LogInfoFormat( "Texture Stage %2i: type=0x%.4x texture=0x%.4x sampler=0x%.4x",
			i, pStages[ i ].type, pStages[ i ].texture, pStages[ i ].samplerConfig );
	}
}
