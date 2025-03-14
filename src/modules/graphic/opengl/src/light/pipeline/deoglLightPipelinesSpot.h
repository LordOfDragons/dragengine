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

#ifndef _DEOGLLIGHTPIPELINESSPOT_H_
#define _DEOGLLIGHTPIPELINESSPOT_H_

#include "deoglLightPipelinesRanged.h"


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesSpot : public deoglLightPipelinesRanged{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesSpot( const deoglRLight &light );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesSpot();
	/*@}*/
	
	
	
public:
	/** Debug name. */
	virtual const char *GetDebugName() const;
	
	
	
protected:
	virtual void pPreparePipelines(deoglBatchedShaderLoading &batched);
	
	
	
private:
	void pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid1Transp1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid2Transp1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid2Transp2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareLumSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareLumSolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pSetSpot( deoglLightShaderConfig &shaconf );
};

#endif
