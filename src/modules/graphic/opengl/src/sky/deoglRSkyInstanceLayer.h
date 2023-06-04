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
