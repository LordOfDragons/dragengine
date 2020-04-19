/* 
 * Drag[en]gine OpenAL Audio Module
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
	deoalRayTraceConfig( const deoalRayTraceConfig &config );
	
	/** \brief Clean up ray trace config. */
	~deoalRayTraceConfig();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of rays. */
	inline int GetRayCount() const{ return pRayCount; }
	
	/** \brief Ray directions. */
	inline const decVector *GetRayDirections() const{ return pRayDirections; }
	
	/**
	 * \brief Ray unit volume.
	 * 
	 * Multiply by distance raised to the power of 3.
	 */
	inline float GetRayUnitVolume() const{ return pRayUnitVolume; }
	
	/**
	 * \brief Ray unit surface.
	 * 
	 * Multiply by distance raised to the power of 2.
	 */
	inline float GetRayUnitSurface() const{ return pRayUnitSurface; }
	
	/**
	 * \brief Ray beam opening angle.
	 */
	inline float GetOpeningAngle() const{ return pOpeningAngle; }
	
	
	
	/** \brief Set from vertices (creates copy). */
	void SetFromVertices( const decVector *vertices, int count, float openingAngle );
	
	/**
	 * \brief Rotate rays.
	 * \param[in] rotation Rotation in degrees.
	 */
	void Rotate( float rx, float ry, float rz );
	
	/** \brief Set ray directions approximatly equally spaces points on a sphere. */
	void SetRaysEquallySpaced( int rayCount );
	
	/** \brief Set from ico sphere vertices. */
	void SetFromIcoSphere( const deoalIcoSphere &icoSphere );
	/*@}*/
};

#endif
