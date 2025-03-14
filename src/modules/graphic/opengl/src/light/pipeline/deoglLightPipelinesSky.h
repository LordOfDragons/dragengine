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

#ifndef _DEOGLLIGHTPIPELINESSKY_H_
#define _DEOGLLIGHTPIPELINESSKY_H_

#include "deoglLightPipelines.h"

class deoglRSkyInstanceLayer;
class deoglLightShaderConfig;


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesSky : public deoglLightPipelines{
private:
	const deoglRSkyInstanceLayer &pLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesSky( const deoglRSkyInstanceLayer &layer );
	
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesSky();
	/*@}*/
	
	
	
public:
	/** Debug name. */
	virtual const char *GetDebugName() const;
	
	
	
protected:
	virtual void pPreparePipelines(deoglBatchedShaderLoading &batched);
	
	
	
private:
	void pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareAmbient( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig, deoglBatchedShaderLoading &batched );
	
	void pSetGI( deoglLightShaderConfig &shaconf );
	void pSetNonGI( deoglLightShaderConfig &shaconf );
};

#endif
