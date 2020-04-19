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
 * \brief Render sky Layer.
 */
class deoglRSkyLayer{
public:
	/** \brief Sky layer body. */
	struct sBody {
		deoglRSkin *skin;
		decVector vertex[ 4 ];
		decVector2 texCoords[ 4 ];
		decColor color;
		decVector2 size;
		decQuaternion orientation;
	};
	
	/** \brief Layer types. */
	enum eLayerTypes{
		/** \brief Unknown. */
		eltUnknown,
		
		/** \brief Sky box. */
		eltSkyBox,
		
		/** \brief Sky Sphere. */
		eltSkySphere
	};
	
	/** \brief Images. */
	enum eImages{
		/** \brief Sky box front. */
		eiBoxFront = 0,
		
		/** \brief Sky box right. */
		eiBoxRight,
		
		/** \brief Sky box back. */
		eiBoxBack,
		
		/** \brief Sky box left. */
		eiBoxLeft,
		
		/** \brief Sky box top. */
		eiBoxTop,
		
		/** \brief Sky box bottom. */
		eiBoxBottom,
		
		/** \brief Sky sphere. */
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
	/** \brief Create sky layer. */
	deoglRSkyLayer( const deSkyLayer &layer );
	
	/** \brief Clean up sky. */
	~deoglRSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Layer type. */
	inline eLayerTypes GetLayerType() const{ return pLayerType; }
	
	/** \brief Textures. */
	inline const int *GetTextures() const{ return &pTextures[ 0 ]; }
	
	/** \brief Render skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	
	
	/** \brief Bodies. */
	inline const sBody *GetBodies() const{ return pBodies; }
	
	/** \brief Number of bodies. */
	inline int GetBodyCount() const{ return pBodyCount; }
	
	
	
	/** \brief Target. */
	inline const deoglRSkyControllerTarget **GetTargets() const{
		return ( const deoglRSkyControllerTarget ** )pTargets; }
	
	
	
	/** \brief Offset. */
	inline const decVector &GetOffset() const{ return pOffset; }
	
	/** \brief Orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Blending color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	
	
	/** \brief Orientation of the sky light. */
	inline const decQuaternion &GetLightOrientation() const{ return pLightOrientation; }
	
	/** \brief Color of the sky light. */
	inline const decColor &GetLightColor() const{ return pLightColor; }
	
	/** \brief Intensity of the direct sky light. */
	inline float GetLightIntensity() const{ return pLightIntensity; }
	
	/** \brief Intensity of the diffuse sky light. */
	inline float GetAmbientIntensity() const{ return pAmbientIntensity; }
	
	
	
	/** \brief Layer intensity is multiplied by total sky light intensity. */
	inline bool GetMuliplyBySkyLight() const{ return pMulBySkyLight; }
	
	/** \brief Layer color is multiplied by total sky light color. */
	inline bool GetMuliplyBySkyColor() const{ return pMulBySkyColor; }
	/*@}*/
	
	
	
private:
	void pUpdateSkins();
	void pInitBodies( const deSkyLayer &layer );
};

#endif
