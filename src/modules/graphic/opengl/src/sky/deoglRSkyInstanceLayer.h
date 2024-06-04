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

#ifndef _DEOGLRSKYINSTANCELAYER_H_
#define _DEOGLRSKYINSTANCELAYER_H_

#include "../light/pipeline/deoglLightPipelinesSky.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class deoglLightShaderConfig;
class deoglRenderThread;
class deoglRSkyInstance;
class deoglSkyLayerTracker;
class deoglShadowCaster;
class deoglSkyLayerGICascade;
class deoglGIState;
class deoglGICascade;
class deoglRComponent;



/**
 * Render sky Layer.
 */
class deoglRSkyInstanceLayer{
private:
	deoglRSkyInstance &pInstance;
	int pIndex;
	
	decVector pOffset;
	decVector pOrientation;
	decVector pRotation;
	decMatrix pMatrix;
	decMatrix pInvMatrix;
	decColor pColor;
	float pIntensity;
	float pTransparency;
	
	decQuaternion pLightOrientation;
	decColor pLightColor;
	float pLightIntensity;
	float pAmbientIntensity;
	
	deoglSkyLayerTracker *pTrackerEnvMap;
	bool pSkyNeedsUpdate;
	
	deoglLightPipelines::Ref pPipelines;
	deoglSPBlockUBO::Ref pParamBlockLight;
	deoglSPBlockUBO::Ref pParamBlockInstance;
	
	deoglShadowCaster *pShadowCaster;
	
	decPointerList pGICascades;
	
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky instance layer. */
	deoglRSkyInstanceLayer( deoglRSkyInstance &instance, int index );
	
	/** Clean up sky instance layer. */
	~deoglRSkyInstanceLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent sky instance, */
	inline deoglRSkyInstance &GetInstance() const{ return pInstance; }
	
	/** Layer index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Update. */
	void Update();
	
	/** Apply multiply by total values modifiers. */
	void UpdateWithModifiers();
	
	/** Sky needs update. */
	inline bool GetSkyNeedsUpdate() const{ return pSkyNeedsUpdate; }
	
	
	
	/** Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse matrix. */
	inline const decMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	/** Blending color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Layer is visible. */
	bool GetVisible() const;
	
	/** Orientation of the sky light. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	
	/** Color of the sky light. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** Intensity of the direct sky light. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** Intensity of the diffuse sky light. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	/** Total light intensity. */
	inline float GetTotalLightIntensity() const{ return pLightIntensity + pAmbientIntensity; }
	
	/** Layer has direct light. */
	bool GetHasLightDirect() const;
	
	/** Layer has diffuse light. */
	bool GetHasLightAmbient() const;
	
	
	
	/** Pipelines. */
	deoglLightPipelines &GetPipelines();
	
	// /** Light parameter block. */
	const deoglSPBlockUBO::Ref &GetLightParameterBlock();
	
	/** Instance parameter block. */
	const deoglSPBlockUBO::Ref &GetInstanceParameterBlock();
	
	/** Shadow caster. */
	inline deoglShadowCaster &GetShadowCaster() const{ return *pShadowCaster; }
	
	/** Notify skies render static component changed requiring updates. */
	void NotifyUpdateStaticComponent( deoglRComponent *component );
	
	
	
	/** Count of GI cascades. */
	int GetGICascadeCount() const;
	
	/** GI Cascade or NULL if not found. */
	deoglSkyLayerGICascade *GetGICascade( const deoglGICascade &cascade ) const;
	
	/** Add GI Cascade if absent. */
	deoglSkyLayerGICascade *AddGICascade( const deoglGICascade &cascade );
	
	/** Remove GI Cascade if present. */
	void RemoveGICascade( const deoglGICascade &cascade );
	
	/** Remove all GI Cascades for GI State if present. */
	void RemoveAllGICascades( const deoglGIState &state );
	
	/** Remove all GI Cascades. */
	void RemoveAllGICascades();
	/*@}*/
	
	
	
private:
	void pUpdateParameters();
	void pCheckTracker();
};

#endif
