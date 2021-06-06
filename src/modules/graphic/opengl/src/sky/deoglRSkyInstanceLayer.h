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

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class deoglLightShader;
class deoglLightShaderConfig;
class deoglRenderThread;
class deoglRSkyInstance;
class deoglSPBlockUBO;
class deoglSkyLayerTracker;
class deoglShadowCaster;
class deoglSkyLayerGIState;
class deoglGIState;
class deoglRComponent;



/**
 * Render sky Layer.
 */
class deoglRSkyInstanceLayer{
public:
	/** Shader Types. */
	enum eShaderTypes{
		/** No shadow casting. */
		estNoShadow,
		
		/** Ambient light only. */
		estAmbient,
		
		/** Solid shadow. */
		estSolid,
		
		/** No shadow casting. */
		estGIRayNoShadow,
		
		/** Solid shadow using one texture. */
		estGIRaySolid1,
		
		/** Solid shadow using two textures. */
		estGIRaySolid2,
		
		/** Number of shaders. */
		EST_COUNT
	};
	
	
	
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
	
	deoglLightShader *pShaders[ EST_COUNT ];
	deoglSPBlockUBO *pParamBlockLight;
	deoglSPBlockUBO *pParamBlockInstance;
	
	deoglShadowCaster *pShadowCaster;
	
	decPointerList pGIStates;
	
	
	
	
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
	
	
	
	/** Shader for shader type. */
	deoglLightShader *GetShaderFor( int shaderType );
	
	/** Shader configuration for shader type. */
	bool GetShaderConfigFor( int shaderType, deoglLightShaderConfig &config );
	
	/** Light parameter block. */
	deoglSPBlockUBO *GetLightParameterBlock();
	
	/** Instance parameter block. */
	deoglSPBlockUBO *GetInstanceParameterBlock();
	
	/** Shadow caster. */
	inline deoglShadowCaster &GetShadowCaster() const{ return *pShadowCaster; }
	
	/** Notify skies render static component changed requiring updates. */
	void NotifyUpdateStaticComponent( deoglRComponent *component );
	
	
	
	/** Count of GI States. */
	int GetGIStateCount() const;
	
	/** GI State or NULL if not found. */
	deoglSkyLayerGIState *GetGIState( const deoglGIState *giState ) const;
	
	/** Add GI State if absent. */
	deoglSkyLayerGIState *AddGIState( const deoglGIState *giState );
	
	/** Remove GI State if present. */
	void RemoveGIState( const deoglGIState *giState );
	
	/** Remove all GI States. */
	void RemoveAllGIStates();
	/*@}*/
	
	
	
private:
	void pUpdateParameters();
	void pCheckTracker();
};

#endif
