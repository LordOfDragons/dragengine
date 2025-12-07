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
		decVector vertex[4];
		decVector2 texCoords[4];
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
	
	int pTextures[6];
	deoglRSkin *pSkin;
	
	sBody *pBodies;
	int pBodyCount;
	
	deoglRSkyControllerTarget *pTargets[deSkyLayer::etAmbientIntensity + 1];
	
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
	deoglRSkyLayer(const deSkyLayer &layer);
	
	/** Clean up sky. */
	~deoglRSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Layer type. */
	inline eLayerTypes GetLayerType() const{ return pLayerType; }
	
	/** Textures. */
	inline const int *GetTextures() const{ return &pTextures[0]; }
	
	/** Render skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	
	
	/** Bodies. */
	inline const sBody *GetBodies() const{ return pBodies; }
	
	/** Number of bodies. */
	inline int GetBodyCount() const{ return pBodyCount; }
	
	
	
	/** Target. */
	inline const deoglRSkyControllerTarget **GetTargets() const{
		return (const deoglRSkyControllerTarget **)pTargets;}
	
	
	
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
	void pInitBodies(const deSkyLayer &layer);
};

#endif
