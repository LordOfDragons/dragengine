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

#include "deoglSTPipelinesBillboard.h"
#include "../deoglSkinTexture.h"
#include "../shader/deoglSkinShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesBillboard
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesBillboard::deoglSTPipelinesBillboard( const deoglSkinTexture &texture ) :
deoglSkinTexturePipelines( texture ){
}

deoglSTPipelinesBillboard::~deoglSTPipelinesBillboard(){
}



// Management
///////////////

const char *deoglSTPipelinesBillboard::GetDebugName() const{
	return "deoglSTPipelinesBillboard";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesBillboard::pPreparePipelines( const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetSharedSPB( true );
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmBillboard );
	
	pPrepareGeometry( baseShaderConfig, cinfo, batched);
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo, batched);
	pPrepareAllDepth( baseShaderConfig, cinfo, batched);
	pPrepareAllCounter( baseShaderConfig, cinfo, batched);
	pPrepareMask( baseShaderConfig, cinfo, batched);
	// pPrepareAllShadow( baseShaderConfig, cinfo, batched);
	// pPrepareEnvMap( baseShaderConfig, cinfo, batched);
	// pPrepareLuminance( baseShaderConfig, cinfo, batched);
	// pPrepareGIMaterial( baseShaderConfig, cinfo, batched);
}
