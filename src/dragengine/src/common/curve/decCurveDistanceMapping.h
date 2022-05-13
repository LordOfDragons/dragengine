/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECCURVEBEZIER3DDISTANCEEVALUATOR_H_
#define _DECCURVEBEZIER3DDISTANCEEVALUATOR_H_

class decCurveBezier3D;


/**
 * \brief Curve distance mapping.
 * 
 * Samples a curve by distance for evaluating curves on a distance
 * basis instead of local coordinates.
 * 
 */
class DE_DLL_EXPORT decCurveDistanceMapping{
private:
	float pResolution;
	float pLength;
	int pLastSegment;
	float *pMappings;
	int pMappingCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create curve distance mapping. */
	decCurveDistanceMapping();
	
	/** \brief Create curve distance mapping. */
	decCurveDistanceMapping( const decCurveDistanceMapping &mapping );
	
	/** \brief Clean up curve distance mapping. */
	~decCurveDistanceMapping();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resolution in meters used for sampling. */
	inline float GetResolution() const{ return pResolution; }
	
	/** \brief Length of sampled curve in meters. */
	inline float GetLength() const{ return pLength; }
	
	/** \brief Get evaluation parameter for distance. */
	void Map( float distance, int &segment, float &blend ) const;
	
	/** \brief Clear mapping. */
	void Clear();
	
	/**
	 * \brief Initialize mapping using a 3d bezier curve.
	 * 
	 * Previous mapping parameters are overwritten. The method chooses resolution
	 * and segment sample count by examining the curve before sampling it.
	 */
	void Init( const decCurveBezier3D &curve );
	
	/**
	 * \brief Initialize mapping using a 3d bezier curve.
	 * 
	 * Previous mapping parameters are overwritten.
	 * \param[in] resolution Sample resolution in meters clamped to 1mm or larger. Smaller values
	 * increase memory consumption for the mapping table a lot for long
	 * curves. A good value is 0.1 resolution for curves less than 1m and
	 * larger for longer curves. Curves which are equally spaced already
	 * can use large resolution values while strongly not equally spaced
	 * curves require smaller resolution values to give good results.
	 * \param[in] segmentSamples Number of samples to take along curve segments. Higher sample
	 * counts increase accuracy of the mapping especially the estimated
	 * curve length. Higher sample counts increase processing time but
	 * do not increase memory consumption. Curves which are equally
	 * spaced can use small sample counts while strongly not equally
	 * spaced curves require higher sample counter to give good results.
	 * For curves which are mostly equally spaced or close to it a samples
	 * count of 10 is a good choice. For not so equally spaced curves
	 * higher values of 20 or more may be required.
	 * 
	 * \throws deeInvalidParam \em segmentSamples is less than 0.
	 */
	void Init( const decCurveBezier3D &curve, float resolution, int segmentSamples );
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another mapping to this mapping. */
	decCurveDistanceMapping &operator=( const decCurveDistanceMapping &mapping );
	
	/** \brief Mapping is equal to another mapping. */
	bool operator==( const decCurveDistanceMapping &mapping ) const;
	/*@}*/
	/*@}*/
};

#endif
