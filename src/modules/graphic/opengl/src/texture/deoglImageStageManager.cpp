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
#include "deoglImageStageManager.h"
#include "arraytexture/deoglArrayTexture.h"
#include "cubemap/deoglCubeMap.h"
#include "texsamplerconfig/deoglTexSamplerConfig.h"
#include "texture2d/deoglTexture.h"
#include "texture2d/deoglRenderableColorTexture.h"
#include "../configuration/deoglConfiguration.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/combinedTexture/deoglCombinedTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>



// Struct deoglImageStageManager::sStage
//////////////////////////////////////////

deoglImageStageManager::sStage::sStage() :
texture(0),
type(0),
level(0),
layer(0),
access(eaReadWrite),
layered(false),
format(0){
}



// Class deoglImageStageManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglImageStageManager::deoglImageStageManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglImageStageManager::~deoglImageStageManager(){
}



// Management
///////////////

int deoglImageStageManager::GetEnabledStageCount() const{
	int i;
	for(i=0; i<OGL_MAX_IMAGE_STAGES; i++){
		if(pStages[i].type == 0){
			return i;
		}
	}
	
	return OGL_MAX_IMAGE_STAGES;
}

GLenum deoglImageStageManager::GetStageType(int stage) const{
	if(stage < 0 || stage >= OGL_MAX_IMAGE_STAGES){
		DETHROW(deeInvalidParam);
	}
	
	return pStages[stage].type;
}



void deoglImageStageManager::Enable(int stage, const deoglTexture &texture, int level, eAccess access){
	if(!texture.GetFormat()){
		DETHROW_INFO(deeNullPointer, "texture.format");
	}
	
	BindImage(stage, texture.GetTexture(), GL_TEXTURE_2D, level, 0, access, false,
		(GLenum)texture.GetFormat()->GetFormat());
}

void deoglImageStageManager::Enable(int stage, const deoglCubeMap &cubemap, int level, eAccess access){
	if(!cubemap.GetFormat()){
		DETHROW_INFO(deeNullPointer, "cubemap.format");
	}
	
	BindImage(stage, cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, level, 0, access, true,
		(GLenum)cubemap.GetFormat()->GetFormat());
}

void deoglImageStageManager::Enable(int stage, const deoglCubeMap &cubemap, int level, int face, eAccess access){
	if(!cubemap.GetFormat()){
		DETHROW_INFO(deeNullPointer, "cubemap.format");
	}
	
	BindImage(stage, cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, level, face, access, false,
		(GLenum)cubemap.GetFormat()->GetFormat());
}

void deoglImageStageManager::Enable(int stage, const deoglArrayTexture &texture, int level, eAccess access){
	if(!texture.GetFormat()){
		DETHROW_INFO(deeNullPointer, "texture.format");
	}
	
	BindImage(stage, texture.GetTexture(), GL_TEXTURE_2D_ARRAY, level, 0, access, true,
		(GLenum)texture.GetFormat()->GetFormat());
}

void deoglImageStageManager::Enable(int stage, const deoglArrayTexture &texture, int level, int layer, eAccess access){
	if(!texture.GetFormat()){
		DETHROW_INFO(deeNullPointer, "texture.format");
	}
	
	BindImage(stage, texture.GetTexture(), GL_TEXTURE_2D_ARRAY, level, layer, access, false,
		(GLenum)texture.GetFormat()->GetFormat());
}

void deoglImageStageManager::EnableTBO(int stage, GLuint tbo, eAccess access, GLenum format){
	BindImage(stage, tbo, GL_TEXTURE_BUFFER, 0, 0, access, false, format);
}

void deoglImageStageManager::EnableSkin(int stage, const deoglRSkin &skin, int texture,
deoglSkinChannel::eChannelTypes channel, deoglTexture *defaultTexture, eAccess access){
	if(stage < 0 || stage >= OGL_MAX_IMAGE_STAGES || !defaultTexture){
		DETHROW(deeInvalidParam);
	}
	
	deoglSkinChannel * const textureChannel = skin.GetTextureAt(texture).GetChannelAt(channel);
	deoglTexture *useTexture = defaultTexture;
	
	if(textureChannel){
		deoglTexture *channelTexture = textureChannel->GetTexture();
		
		if(textureChannel->GetImage()){
			channelTexture = textureChannel->GetImage()->GetTexture();
			
		}else if(textureChannel->GetCombinedTexture()){
			channelTexture = textureChannel->GetCombinedTexture()->GetTexture();
		}
		
		if(channelTexture){
			useTexture = channelTexture;
		}
	}
	
	Enable(stage, *useTexture, 0, access);
}

void deoglImageStageManager::EnableRenderDocDebug(int stage, int flag){
	if((pRenderThread.GetChoices().GetRenderDocDebugFlags() & flag) == flag){
		Enable(stage, *pRenderThread.GetDeferredRendering().GetTexRenderDocDebug(),
			0, deoglImageStageManager::eaWrite);
	}
}



void deoglImageStageManager::DisableStage(int stage){
	if(stage < 0 || stage >= OGL_MAX_IMAGE_STAGES){
		DETHROW(deeInvalidParam);
	}
	if(pStages[stage].type == 0){
		return;
	}
	
	if(pStages[stage].texture != 0){
		// unbind seems impossible. just leave the binding and consider it cleared
		pStages[stage].texture = 0;
	}
	
	pStages[stage].type = 0;
	pStages[stage].level = 0;
	pStages[stage].layer = 0;
	pStages[stage].access = eaReadWrite;
	pStages[stage].layered = false;
	pStages[stage].format = 0;
}

void deoglImageStageManager::DisableStagesAbove(int stage){
	int i;
	for(i=stage+1; i<OGL_MAX_IMAGE_STAGES; i++){
		DisableStage(i);
	}
}

void deoglImageStageManager::DisableAllStages(){
	DisableStagesAbove(-1);
}



void deoglImageStageManager::BindImage(int stage, GLuint texture, GLenum type,
int level, int layer, eAccess access, bool layered, GLenum format){
	DEASSERT_TRUE(stage >= 0)
	DEASSERT_TRUE(stage < OGL_MAX_IMAGE_STAGES)
	
	if(pStages[stage].texture == texture
	&& pStages[stage].type == type
	&& pStages[stage].level == level
	&& pStages[stage].layer == layer
	&& pStages[stage].layered == layered
	&& pStages[stage].access == access
	&& pStages[stage].format == format){
		return;
	}
	
	GLenum glaccess = GL_READ_WRITE;
	switch(access){
	case eaRead:
		glaccess = GL_READ_ONLY;
		break;
		
	case eaWrite:
		glaccess = GL_WRITE_ONLY;
		break;
		
	case eaReadWrite:
		glaccess = GL_READ_WRITE;
		break;
	}
	
	OGL_CHECK(pRenderThread, pglBindImageTexture(stage, texture, level, layered, layer, glaccess, format));
	pStages[stage].texture = texture;
	pStages[stage].level = level;
	pStages[stage].layer = layer;
	pStages[stage].layered = layered;
	pStages[stage].access = access;
	pStages[stage].format = format;
}



void deoglImageStageManager::LogCurrentState() const{
	int i;
	for(i=0; i<OGL_MAX_IMAGE_STAGES; i++){
		const char *access = "?";
		switch(pStages[i].access){
		case eaRead:
			access = "r";
			break;
			
		case eaWrite:
			access = "w";
			break;
			
		case eaReadWrite:
			access = "rw";
			break;
		}
		
		pRenderThread.GetLogger().LogInfoFormat(
			"Image Stage %2d: type=0x%.4x texture=0x%.4x level=%d layer=%d layered=%s access=%s format=0x%.4x",
			i, pStages[i].type, pStages[i].texture, pStages[i].level, pStages[i].layer,
			pStages[i].layered ? "true" : "false", access, pStages[i].format);
	}
}
