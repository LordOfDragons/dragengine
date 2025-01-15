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

#ifndef _DEOGLLIGHTPIPELINES_H_
#define _DEOGLLIGHTPIPELINES_H_

#include "deoglLightPipeline.h"
#include "../shader/deoglLightShaderManager.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../utils/deoglDebugNamesEnumSet.h"
#include "../../shaders/deoglBatchedShaderLoading.h"

#include <dragengine/deObject.h>


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelines : public deObject{
public:
	typedef deTObjectReference<deoglLightPipelines> Ref;
	
	/** Type. */
	enum eTypes{
		etNoShadow, //<! No shadow casting.
		etAmbient, //<! Ambient light only.
		etSolid1, //<! Single solid no transparent shadow.
		etSolid1Transp1, //<! Single solid and transparent shadow.
		etSolid2, //<! Double solid no transparent shadow.
		etSolid2Transp1, //<! Double solid and single transparent shadow.
		etSolid2Transp2, //<! Double solid and transparent shadow.
		etLumSolid1, //<! Luminance only single solid (unused).
		etLumSolid2, //<! Luminance only double solid (unused).
		etGIRayNoShadow, //<! GI rays no shadow casting.
		etGIRaySolid1, //<! GI rays single solid shadow.
		etGIRaySolid2 //<! GI rays double solid shadow.
	};
	
	static const deoglDebugNamesEnum DebugNamesTypes;
	
	/** Modifier. */
	enum eModifiers{
		emNoAmbient = 0x1,
		emStereo = 0x2,
		emTransparent = 0x4,
		emFlipCullFace = 0x8,
		emCameraInside = 0x10
	};
	
	static const deoglDebugNamesEnumSet DebugNamesModifiers;
	
	
	
protected:
	class cPipelineGetShaderListener : public deoglLightShaderManager::cGetShaderListener{
	protected:
		deoglBatchedShaderLoading &pBatched;
		deoglLightPipelines &pLightPipelines;
		eTypes pType;
		int pModifier;
		deoglPipelineConfiguration pConfig;
		deoglLightPipeline::Ref &pPipeline;
		
	public:
		cPipelineGetShaderListener(deoglBatchedShaderLoading &batched,
			deoglLightPipelines &skinPipelines, eTypes type, int modifier,
			deoglLightPipeline::Ref &pipeline, const deoglPipelineConfiguration &config);
		
		void GetShaderFinished(deoglLightShader *shader) override;
	};
	
	
	const static int TypeCount = etGIRaySolid2 + 1;
	const static int ModifiersPerType = emCameraInside << 1;
	
	deoglRenderThread &pRenderThread;
	deoglLightPipeline::Ref pPipelines[ TypeCount ][ ModifiersPerType ];
	bool pPrepared;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelines( deoglRenderThread &renderThread );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelines();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pipeline with type and modifiers or nullptr. */
	const deoglLightPipeline *GetWith( eTypes type, int modifiers ) const;
	
	/** Pipeline with type and modifiers throwing exception it nullptr. */
	const deoglLightPipeline &GetWithRef( eTypes type, int modifiers ) const;
	
	/** Prepare pipelines. */
	void Prepare(deoglBatchedShaderLoading &batched);
	
	/** Debug name. */
	virtual const char *GetDebugName() const = 0;
	/*@}*/
	
	
	
protected:
	virtual void pPreparePipelines(deoglBatchedShaderLoading &batched) = 0;
	
	virtual void pBasePipelineConfig(deoglRenderThread &renderThread,
		deoglPipelineConfiguration &config);
	
	virtual void pBasePipelineConfigGI(deoglPipelineConfiguration &config);
	
	virtual void pCreatePipelines(deoglRenderThread &renderThread,
		const deoglPipelineConfiguration &basePipelineConfig,
		const deoglLightShaderConfig &baseShaderConfig, eTypes type, int modifierMask,
		deoglBatchedShaderLoading &batched);
};

#endif
