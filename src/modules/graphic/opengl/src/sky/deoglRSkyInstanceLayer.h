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

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class deoglLightShader;
class deoglLightShaderConfig;
class deoglRenderThread;
class deoglRSkyInstance;
class deoglSPBlockUBO;
class deoglSkyLayerShadowCaster;
class deoglSkyLayerTracker;



/**
 * \brief Render sky Layer.
 */
class deoglRSkyInstanceLayer{
public:
	/** \brief Shader Types. */
	enum eShaderTypes{
		/** \brief No shadow casting. */
		estNoShadow,
		
		/** \brief Ambient light only. */
		estAmbient,
		
		/** \brief Solid shadow. */
		estSolid,
		
		/** \brief No shadow casting. */
		estGIRayNoShadow,
		
		/** \brief Solid shadow. */
		estGIRaySolid,
		
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
	
	deoglSkyLayerShadowCaster *pShadowCaster;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance layer. */
	deoglRSkyInstanceLayer( deoglRSkyInstance &instance, int index );
	
	/** \brief Clean up sky instance layer. */
	~deoglRSkyInstanceLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent sky instance, */
	inline deoglRSkyInstance &GetInstance() const{ return pInstance; }
	
	/** \brief Update. */
	void Update();
	
	/** \brief Apply multiply by total values modifiers. */
	void UpdateWithModifiers();
	
	/** \brief Sky needs update. */
	inline bool GetSkyNeedsUpdate() const{ return pSkyNeedsUpdate; }
	
	
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	/** \brief Blending color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** \brief Layer is visible. */
	bool GetVisible() const;
	
	/** \brief Orientation of the sky light. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	
	/** \brief Color of the sky light. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** \brief Intensity of the direct sky light. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** \brief Intensity of the diffuse sky light. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	/** \brief Layer has direct light. */
	bool GetHasLightDirect() const;
	
	/** \brief Layer has diffuse light. */
	bool GetHasLightAmbient() const;
	
	
	
	/** \brief Shader for shader type. */
	deoglLightShader *GetShaderFor( int shaderType );
	
	/** \brief Shader configuration for shader type. */
	bool GetShaderConfigFor( int shaderType, deoglLightShaderConfig &config );
	
	/** \brief Light parameter block. */
	deoglSPBlockUBO *GetLightParameterBlock();
	
	/** \brief Instance parameter block. */
	deoglSPBlockUBO *GetInstanceParameterBlock();
	
	/** \brief Shadow caster. */
	inline deoglSkyLayerShadowCaster &GetShadowCaster() const{ return *pShadowCaster; }
	/*@}*/
	
	
	
private:
	void pUpdateParameters();
	void pCheckTracker();
};

#endif
