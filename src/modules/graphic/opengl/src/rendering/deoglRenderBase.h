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

#ifndef _DEOGLRENDERBASE_H_
#define _DEOGLRENDERBASE_H_

#include "../deoglBasics.h"
#include "../debug/deoglDebugInformation.h"
#include "../pipeline/deoglPipeline.h"
#include "../shaders/deoglShaderProgramUsage.h"

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
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decTimer pDebugTimer[ 4 ];
	
	
	
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
	
	
	
	/** Set cull mode. */
	void SetCullMode( bool renderBackFaces );
	
	/** Render full screen quad without changing VAO. Requires GetVAOFullScreenQuad() to be active. */
	void RenderFullScreenQuad();
	
	/**
	 * Render full screen quad without changing VAO. Requires GetVAOFullScreenQuad() to
	 * be active. If stereo rendering is active and specific hardware support is present
	 * renders two quads using multi-draw instead of one.
	 */
	void RenderFullScreenQuad( const deoglRenderPlan &plan );
	
	/** Render full screen quad with changing VAO. */
	void RenderFullScreenQuadVAO();
	
	/**
	 * Render full screen quad with changing VAO. If stereo rendering is active and specific
	 * hardware support is present renders two quads using multi-draw instead of one.
	 */
	void RenderFullScreenQuadVAO( const deoglRenderPlan &plan );
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
	
	
	
private:
	void pDebugTimerReset( decTimer &timer, const deoglRenderPlan &plan, bool waitGPU );
	
	void pDebugTimerSample( decTimer &timer, const deoglRenderPlan &plan,
		deoglDebugInformation &debugInfo, bool waitGPU );
	
	void pDebugTimerSampleCount( decTimer &timer, const deoglRenderPlan &plan,
		deoglDebugInformation &debugInfo, int count, bool waitGPU );
	/*@}*/
};

#endif
