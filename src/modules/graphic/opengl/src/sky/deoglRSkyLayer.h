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

#ifndef _DEOGLRSKYLAYER_H_
#define _DEOGLRSKYLAYER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sky/deSkyLayer.h>

class deoglRSkin;
class deoglRSkyControllerTarget;



/**
 * Render sky Layer.
 */
class deoglRSkyLayer{
public:
	/** Sky layer body. */
	struct sBody {
		deoglRSkin *skin;
		decVector vertex[ 4 ];
		decVector2 texCoords[ 4 ];
		decColor color;
		decVector2 size;
		decQuaternion orientation;
	};
	
	/** Layer types. */
	enum eLayerTypes{
		/** Unknown. */
		eltUnknown,
		
		/** Sky box. */
		eltSkyBox,
		
		/** Sky Sphere. */
		eltSkySphere
	};
	
	/** Images. */
	enum eImages{
		/** Sky box front. */
		eiBoxFront = 0,
		
		/** Sky box right. */
		eiBoxRight,
		
		/** Sky box back. */
		eiBoxBack,
		
		/** Sky box left. */
		eiBoxLeft,
		
		/** Sky box top. */
		eiBoxTop,
		
		/** Sky box bottom. */
		eiBoxBottom,
		
		/** Sky sphere. */
		eiSphere = 0,
	};
	
	
	
private:
	eLayerTypes pLayerType;
	
	int pTextures[ 6 ];
	deoglRSkin *pSkin;
	
	sBody *pBodies;
	int pBodyCount;
	
	deoglRSkyControllerTarget *pTargets[ deSkyLayer::etAmbientIntensity + 1 ];
	
	const decVector pOffset;
	const decVector pOrientation;
	const decColor pColor;
	const float pIntensity;
	const float pTransparency;
	
	const decQuaternion pLightOrientation;
	const decColor pLightColor;
	const float pLightIntensity;
	const float pAmbientIntensity;
	
	const bool pMulBySkyLight;
	const bool pMulBySkyColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky layer. */
	deoglRSkyLayer( const deSkyLayer &layer );
	
	/** Clean up sky. */
	~deoglRSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Layer type. */
	inline eLayerTypes GetLayerType() const{ return pLayerType; }
	
	/** Textures. */
	inline const int *GetTextures() const{ return &pTextures[ 0 ]; }
	
	/** Render skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	
	
	/** Bodies. */
	inline const sBody *GetBodies() const{ return pBodies; }
	
	/** Number of bodies. */
	inline int GetBodyCount() const{ return pBodyCount; }
	
	
	
	/** Target. */
	inline const deoglRSkyControllerTarget **GetTargets() const{
		return ( const deoglRSkyControllerTarget ** )pTargets; }
	
	
	
	/** Offset. */
	inline const decVector &GetOffset() const{ return pOffset; }
	
	/** Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** Blending color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	
	
	/** Orientation of the sky light. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	
	/** Color of the sky light. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** Intensity of the direct sky light. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** Intensity of the diffuse sky light. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	
	
	/** Layer intensity is multiplied by total sky light intensity. */
	inline bool GetMuliplyBySkyLight() const{ return pMulBySkyLight; }
	
	/** Layer color is multiplied by total sky light color. */
	inline bool GetMuliplyBySkyColor() const{ return pMulBySkyColor; }
	/*@}*/
	
	
	
private:
	void pUpdateSkins();
	void pInitBodies( const deSkyLayer &layer );
};

#endif
