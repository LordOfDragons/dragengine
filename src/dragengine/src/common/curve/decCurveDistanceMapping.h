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

#ifndef _DECCURVEBEZIER3DDISTANCEEVALUATOR_H_
#define _DECCURVEBEZIER3DDISTANCEEVALUATOR_H_

#include "../../dragengine_export.h"

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
