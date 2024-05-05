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

#include <stdlib.h>

#include "deoglRenderTaskConfigTexture.h"
#include "../shared/deoglRenderTaskSharedInstance.h"
#include "../shared/deoglRenderTaskSharedTexture.h"
#include "../shared/deoglRenderTaskSharedVAO.h"
#include "../../../pipeline/deoglPipeline.h"


// Class deoglRenderTaskConfigTexture
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskConfigTexture::deoglRenderTaskConfigTexture(){
	Clear();
}

deoglRenderTaskConfigTexture::~deoglRenderTaskConfigTexture(){
}



// Management
///////////////

void deoglRenderTaskConfigTexture::SetRenderTaskFilter( int filter ){
	pRenderTaskFilter = filter;
}

void deoglRenderTaskConfigTexture::SetPipeline( const deoglPipeline *pipeline ){
	pPipeline = pipeline;
	pPipelineIndex = pipeline ? pipeline->GetRTSIndex() : -1;
}

void deoglRenderTaskConfigTexture::SetTexture( const deoglRenderTaskSharedTexture *texture ){
	pTexture = texture;
	pTextureIndex = texture ? texture->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetVAO( const deoglRenderTaskSharedVAO *vao ){
	pVAO = vao;
	pVAOIndex = vao ? vao->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetInstance( const deoglRenderTaskSharedInstance *instance ){
	pInstance = instance;
	pInstanceIndex = instance ? instance->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetGroupIndex( int groupIndex ){
	pGroupIndex = groupIndex;
}



void deoglRenderTaskConfigTexture::Clear(){
	pRenderTaskFilter = 0;
	pPipeline = nullptr;
	pTexture = nullptr;
	pVAO = nullptr;
	pInstance = nullptr;
	pPipelineIndex = -1;
	pTextureIndex = 0;
	pVAOIndex = 0;
	pInstanceIndex = 0;
	pGroupIndex = 0;
}
