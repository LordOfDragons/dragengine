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

#ifndef _DEOGLRENDERBASE_H_
#define _DEOGLRENDERBASE_H_

#include "../deoglBasics.h"
#include "../debug/deoglDebugInformation.h"
#include "../pipeline/deoglPipeline.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglSPBSingleUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

class deoglCollideList;
class deoglRenderPlan;
class deoglRenderThread;
class deoglShaderCompiled;
class deoglTexSamplerConfig;
class deoglShaderDefines;



/**
 * Base Renderer Class.
 */
class deoglRenderBase{
public:
	struct sRenderParameters{
		deoglRenderPlan *plan;
		const deoglCollideList *collideList;
		decDMatrix matrixProjection;
		decDMatrix matrixCamera;
		deoglShaderCompiled *shader;
		bool solid;
		bool withHoles;
		bool withDepthGamma;
		bool skipShadowNone;
		bool maskedOnly;
		bool clipDepth;
		decVector clipNormal;
		float clipDistance;
	};
	
	static bool anyPipelineShaderFailed;
	
	
protected:
	deoglRenderThread &pRenderThread;
	
	const deoglPipeline *pPipelineClearBuffers;
	
	decTimer pDebugTimer[ 4 ];
	
	
private:
	class cPipelineGetProgramListener : public deoglShaderManager::cGetProgramListener{
	private:
		deoglRenderThread &pRenderThread;
		deoglPipelineConfiguration pConfig;
		const deoglPipeline *&pPipeline;
		bool pAssignRTSIndex;
		
	public:
		cPipelineGetProgramListener(deoglRenderThread &renderThread, const deoglPipeline *&pipeline,
			const deoglPipelineConfiguration &config, bool assignRTSIndex);
		
		void GetProgramFinished(const deoglShaderProgram *program) override;
	};
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create base renderer. */
	deoglRenderBase( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	virtual ~deoglRenderBase();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Clear buffers pipeline. */
	inline const deoglPipeline *GetPipelineClearBuffers() const{ return pPipelineClearBuffers; }
	
	
	
	/** Add basic defines for shaders. */
	void AddBasicDefines( deoglShaderDefines &defines );
	
	/** Add shared SPB defines for shaders. */
	void AddSharedSPBDefines( deoglShaderDefines &defines );
	
	/** Clamped nearest sampler. */
	deoglTexSamplerConfig &GetSamplerClampNearest() const;
	
	/** Clamped linear sampler. */
	deoglTexSamplerConfig &GetSamplerClampLinear() const;
	
	/** Clamped nearest mipmap sampler. */
	deoglTexSamplerConfig &GetSamplerClampNearestMipMap() const;
	
	/** Clamped linear mipmap sampler. */
	deoglTexSamplerConfig &GetSamplerClampLinearMipMap() const;
	
	/** Clamped linear mipmap nearest sampler. */
	deoglTexSamplerConfig &GetSamplerClampLinearMipMapNearest() const;
	
	/** Repeat nearest sampler. */
	deoglTexSamplerConfig &GetSamplerRepeatNearest() const;
	
	/** Repeat linear sampler. */
	deoglTexSamplerConfig &GetSamplerRepeatLinear() const;
	
	/** Repeat linear mipmap sampler. */
	deoglTexSamplerConfig &GetSamplerRepeatLinearMipMap() const;
	
	/** Shadow test clamped linear sampler. */
	deoglTexSamplerConfig &GetSamplerShadowClampLinear() const;
	
	/** Shadow test clamped linear sampler for inverse depth testing if enabled. */
	deoglTexSamplerConfig &GetSamplerShadowClampLinearInverse() const;
	
	
	/** Set viewport and scissor to render plan size. */
	void SetViewport( const deoglRenderPlan &plan ) const;
	
	/** Set viewport and scissor size. */
	void SetViewport( int width, int height ) const;
	void SetViewport( const decPoint &point ) const;
	void SetViewport( const decPoint3 &point ) const;
	void SetViewport( int x, int y, int width, int height ) const;
	void SetViewport( const decPoint &offset, const decPoint &size ) const;
	
	/** Render full screen quad without changing VAO. Requires GetVAOFullScreenQuad() to be active. */
	void RenderFullScreenQuad() const;
	
	/**
	 * Render full screen quad without changing VAO. Requires GetVAOFullScreenQuad() to
	 * be active. If stereo rendering is active and specific hardware support is present
	 * renders two quads using multi-draw instead of one.
	 */
	void RenderFullScreenQuad( const deoglRenderPlan &plan ) const;
	
	/** Render full screen quad with changing VAO. */
	void RenderFullScreenQuadVAO() const;
	
	/**
	 * Render full screen quad with changing VAO. If stereo rendering is active and specific
	 * hardware support is present renders two quads using multi-draw instead of one.
	 */
	void RenderFullScreenQuadVAO( const deoglRenderPlan &plan ) const;
	void RenderFullScreenQuadVAO( bool useStereo ) const;
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Direct access debug timer. */
	inline decTimer& GetDebugTimerAt( int index ){ return pDebugTimer[ index ]; }
	
	/** Reset all debug timers if plan has debug enabled. */
	void DebugTimersReset( const deoglRenderPlan &plan, bool waitGPU );
	
	/** Reset first debug timer if plan has debug enabled. */
	void DebugTimer1Reset( const deoglRenderPlan &plan, bool waitGPU );
	
	/** Sample first debug timer if plan has debug enabled updating debug information. */
	void DebugTimer1Sample( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		bool waitGPU );
	
	/** Sample first debug timer if plan has debug enabled updating debug information. */
	void DebugTimer1SampleCount( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		int count, bool waitGPU );
	
	/** Reset second debug timer if plan has debug enabled. */
	void DebugTimer2Reset( const deoglRenderPlan &plan, bool waitGPU );
	
	/** Sample second debug timer if plan has debug enabled updating debug information. */
	void DebugTimer2Sample( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		bool waitGPU );
	
	/** Sample second debug timer if plan has debug enabled updating debug information. */
	void DebugTimer2SampleCount( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		int count, bool waitGPU );
	
	/** Reset third debug timer if plan has debug enabled. */
	void DebugTimer3Reset( const deoglRenderPlan &plan, bool waitGPU );
	
	/** Sample third debug timer if plan has debug enabled updating debug information. */
	void DebugTimer3Sample( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		bool waitGPU );
	
	/** Sample third debug timer if plan has debug enabled updating debug information. */
	void DebugTimer3SampleCount( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		int count, bool waitGPU );
	
	/** Reset fourthed debug timer if plan has debug enabled. */
	void DebugTimer4Reset( const deoglRenderPlan &plan, bool waitGPU );
	
	/** Sample fourthed debug timer if plan has debug enabled updating debug information. */
	void DebugTimer4Sample( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		bool waitGPU );
	
	/** Sample fourthed debug timer if plan has debug enabled updating debug information. */
	void DebugTimer4SampleCount( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		int count, bool waitGPU );
	
	/** Increment debug timer. */
	void DebugTimerIncrement( const deoglRenderPlan &plan, deoglDebugInformation &debugInfo,
		float elapsed, int count );
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	
	/** Clear render doc debug texture if enabled. */
	void ClearRenderDocDebugTexture();
	
	
	
protected:
	void pAsyncGetPipeline(const deoglPipeline *&pipeline, deoglPipelineConfiguration &config,
		const char *sources, const deoglShaderDefines &defines, bool assignRTSIndex = false);
	
	void pAsyncGetPipeline(const deoglPipeline *&pipeline, deoglPipelineConfiguration &config,
		const deoglShaderSources *sources, const deoglShaderDefines &defines,
		bool assignRTSIndex = false);
	
	
	
private:
	void pDebugTimerReset( decTimer &timer, const deoglRenderPlan &plan, bool waitGPU );
	
	void pDebugTimerSample( decTimer &timer, const deoglRenderPlan &plan,
		deoglDebugInformation &debugInfo, bool waitGPU );
	
	void pDebugTimerSampleCount( decTimer &timer, const deoglRenderPlan &plan,
		deoglDebugInformation &debugInfo, int count, bool waitGPU );
	/*@}*/
};

#endif
