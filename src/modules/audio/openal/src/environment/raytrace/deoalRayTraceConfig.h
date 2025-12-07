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

#ifndef _DEOALENVPROBECONFIG_H_
#define _DEOALENVPROBECONFIG_H_

#include <dragengine/common/math/decMath.h>

class deoalIcoSphere;


/**
 * \brief Environment probe configuration.
 * 
 * Stores parameters used by probes to collect and process probe data.
 */
class deoalRayTraceConfig{
private:
	int pRayCount;
	decVector *pRayDirections;
	
	float pRayUnitVolume;
	float pRayUnitSurface;
	float pOpeningAngle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace config. */
	deoalRayTraceConfig();
	
	/** \brief Create ray trace config. */
	deoalRayTraceConfig(const deoalRayTraceConfig &config);
	
	/** \brief Clean up ray trace config. */
	~deoalRayTraceConfig();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of rays. */
	inline int GetRayCount() const{return pRayCount;}
	
	/** \brief Ray directions. */
	inline const decVector *GetRayDirections() const{return pRayDirections;}
	
	/**
	 * \brief Ray unit volume.
	 * 
	 * Multiply by distance raised to the power of 3.
	 */
	inline float GetRayUnitVolume() const{return pRayUnitVolume;}
	
	/**
	 * \brief Ray unit surface.
	 * 
	 * Multiply by distance raised to the power of 2.
	 */
	inline float GetRayUnitSurface() const{return pRayUnitSurface;}
	
	/**
	 * \brief Ray beam opening angle.
	 */
	inline float GetOpeningAngle() const{return pOpeningAngle;}
	
	
	
	/** \brief Set from vertices (creates copy). */
	void SetFromVertices(const decVector *vertices, int count, float openingAngle);
	
	/**
	 * \brief Rotate rays.
	 * \param[in] rotation Rotation in degrees.
	 */
	void Rotate(float rx, float ry, float rz);
	
	/** \brief Set ray directions approximatly equally spaces points on a sphere. */
	void SetRaysEquallySpaced(int rayCount);
	
	/** \brief Set from ico sphere vertices. */
	void SetFromIcoSphere(const deoalIcoSphere &icoSphere);
	/*@}*/
};

#endif
