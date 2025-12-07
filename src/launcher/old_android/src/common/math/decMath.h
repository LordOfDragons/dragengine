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

/**
 * @brief Class for mathematical calculations.
 *
 * @file decMath.h
 */



#ifndef _DECMATH_H_
#define _DECMATH_H_

#include <math.h>


/** Value of pi. */
#ifdef PI
#	undef PI
#endif
#define PI 3.14159265f

/** Value of half pi. */
#ifdef HALF_PI
#	undef HALF_PI
#endif
#define HALF_PI (PI / 2.0f)

/** One degree in radians. */
#ifdef DEG2RAD
#	undef DEG2RAD
#endif
#define	DEG2RAD (PI / 180.0f)

/** Floating point thresholds (epsilons). */
#define FLOAT_EPSILON				1.1929093e-7f
#define FLOAT_SAFE_EPSILON			1e-6f

#define DOUBLE_EPSILON				2.220446049250313e-16
#define DOUBLE_SAFE_EPSILON			1e-15

#define COLOR_THRESHOLD				1e-5f
#define VECTOR_THRESHOLD			1e-6f
#define DVECTOR_THRESHOLD			1e-6
#define QUATERNION_THRESHOLD		1e-5f
#define TEXEL_THRESHOLD				1e-5f
#define CURVE_THRESHOLD				1e-6f



// predefinitions
class decVector;
class decVector2;
class decVector4;
class decDVector;
class decDVector4;
class decQuaternion;
class decPoint;
class decPoint3;
class decColor;
class decMatrix;
class decDMatrix;
class decTexMatrix;
class decTexMatrix2;
class decColorMatrix;
class decColorMatrix3;
class decBoundary;

// classes
#include "decVector.h"
#include "decVector2.h"
#include "decVector4.h"
#include "decDVector.h"
#include "decDVector4.h"
#include "decQuaternion.h"
#include "decPoint.h"
#include "decPoint3.h"
#include "decColor.h"
#include "decMatrix.h"
#include "decDMatrix.h"
#include "decTexMatrix.h"
#include "decTexMatrix2.h"
#include "decColorMatrix.h"
#include "decColorMatrix3.h"
#include "decBoundary.h"



// Common math functions
//////////////////////////

namespace decMath{
	/** \brief Smaller of two values. */
	inline float min(float a, float b){
		return (a < b) ? a : b;
	}
	
	/** \brief Smaller of two values. */
	inline double min(double a, double b){
		return (a < b) ? a : b;
	}
	
	/** \brief Smaller of two values. */
	inline int min(int a, int b){
		return (a < b) ? a : b;
	}
	
	
	
	/** \brief Larger of two values. */
	inline float max(float a, float b){
		return (a > b) ? a : b;
	}
	
	/** \brief Larger of two values. */
	inline double max(double a, double b){
		return (a > b) ? a : b;
	}
	
	/** \brief Larger of two values. */
	inline int max(int a, int b){
		return (a > b) ? a : b;
	}
	
	
	
	/** \brief Clamp value to range. */
	inline float clamp(float x, float lower, float upper){
		if(x <= lower){
			return lower;
			
		}else if(x >= upper){
			return upper;
			
		}else{
			return x;
		}
	}
	
	/** \brief Clamp value to range. */
	inline double clamp(double x, double lower, double upper){
		if(x <= lower){
			return lower;
			
		}else if(x >= upper){
			return upper;
			
		}else{
			return x;
		}
	}
	
	/** \brief Clamp value to range. */
	inline int clamp(int x, int lower, int upper){
		if(x <= lower){
			return lower;
			
		}else if(x >= upper){
			return upper;
			
		}else{
			return x;
		}
	}
	
	
	
	/** \brief Normalize value. */
	inline int normalize(int x, int lower, int upper){
		if(lower < upper){
			return lower + ((x - lower) % (upper - lower + 1));
			
		}else{
			return lower;
		}
	}
	
	/** \brief Normalize value. */
	inline float normalize(float x, float lower, float upper){
		if(lower < upper){
			const float range = upper - lower;
			const float offset = x - lower;
			return (offset - floorf(offset / range) * range) + lower;
			
		}else{
			return lower;
		}
	}
	
	/** \brief Normalize value. */
	inline double normalize(double x, double lower, double upper){
		if(lower < upper){
			const double range = upper - lower;
			const double offset = x - lower;
			return (offset - floor(offset / range) * range) + lower;
			
		}else{
			return lower;
		}
	}
	
	/**
	 * \brief Normalize value.
	 * \details Same as normalize(float,float,float) with lower set to 0.
	 */
	inline float normalize(float x, float upper){
		if(upper > 0.0f){
			return (x - floorf(x / upper) * upper);
			
		}else{
			return 0.0f;
		}
	}
	
	/**
	 * \brief Normalize value.
	 * \details Same as normalize(double,double,double) with lower set to 0.
	 */
	inline double normalize(double x, double upper){
		if(upper > 0.0){
			return (x - floor(x / upper) * upper);
			
		}else{
			return 0.0;
		}
	}
	
	
	
	/**
	 * \brief Blend between two values.
	 * \details \em blend is clamped to the range between 0 and 1 before mixing.
	 */
	inline float mix(float a, float b, float blend){
		if(blend <= 0.0f){
			return a;
			
		}else if(blend >= 1.0f){
			return b;
			
		}else{
			return a * (1.0f - blend) + b * blend;
		}
	}
	
	/**
	 * \brief Blend between two values.
	 * \details \em blend is clamped to the range between 0 and 1 before mixing.
	 */
	inline double mix(double a, double b, double blend){
		if(blend <= 0.0){
			return a;
			
		}else if(blend >= 1.0){
			return b;
			
		}else{
			return a * (1.0 - blend) + b * blend;
		}
	}
	
	
	
	/** \brief Linear map x to the range from 0 to 1. */
	inline float linearStep(float x, float lower, float upper){
		if(upper > lower){
			if(x <= lower){
				return 0.0f;
				
			}else if(x >= upper){
				return 1.0f;
				
			}else{
				return (x - lower) / (upper - lower);
			}
			
		}else{
			return 0.0f;
		}
	}
	
	/** \brief Linear map x to the range from 0 to 1. */
	inline double linearStep(double x, double lower, double upper){
		if(upper > lower){
			if(x <= lower){
				return 0.0;
				
			}else if(x >= upper){
				return 1.0;
				
			}else{
				return (x - lower) / (upper - lower);
			}
			
		}else{
			return 0.0;
		}
	}
	
	/** \brief Linear map x from one range to another range. */
	inline float linearStep(float x, float lower, float upper, float mapLower, float mapUpper){
		if(upper > lower){
			if(x <= lower){
				return mapLower;
				
			}else if(x >= upper){
				return mapUpper;
				
			}else{
				return mapLower + (mapUpper - mapLower) * (x - lower) / (upper - lower);
			}
			
		}else{
			return mapLower;
		}
	}
	
	/** \brief Linear map x from one range to another range. */
	inline double linearStep(double x, double lower, double upper, double mapLower, double mapUpper){
		if(upper > lower){
			if(x <= lower){
				return mapLower;
				
			}else if(x >= upper){
				return mapUpper;
				
			}else{
				return mapLower + (mapUpper - mapLower) * (x - lower) / (upper - lower);
			}
			
		}else{
			return mapLower;
		}
	}
	
	
	
	/** \brief Smooth map x to the range from 0 to 1. */
	inline float smoothStep(float x, float lower, float upper){
		if(upper > lower){
			if(x <= lower){
				return 0.0f;
				
			}else if(x >= upper){
				return 1.0f;
				
			}else{
				const float normalized = (x - lower) / (upper - lower);
				return normalized * normalized * normalized * (normalized * (normalized * 6.0f - 15.0f) + 10.0f);
			}
			
		}else{
			return 0.0f;
		}
	}
	
	/** \brief Smooth map x to the range from 0 to 1. */
	inline double smoothStep(double x, double lower, double upper){
		if(upper > lower){
			if(x <= lower){
				return 0.0;
				
			}else if(x >= upper){
				return 1.0;
				
			}else{
				const double normalized = (x - lower) / (upper - lower);
				return normalized * normalized * normalized * (normalized * (normalized * 6.0 - 15.0) + 10.0);
			}
			
		}else{
			return 0.0;
		}
	}
	
	/** \brief Smooth map x from one range to another range. */
	inline float smoothStep(float x, float lower, float upper, float mapLower, float mapUpper){
		if(upper > lower){
			if(x <= lower){
				return mapLower;
				
			}else if(x >= upper){
				return mapUpper;
				
			}else{
				const float normalized = (x - lower) / (upper - lower);
				const float smoothed = normalized * normalized * normalized * (normalized * (normalized * 6.0f - 15.0f) + 10.0f);
				return mapLower + (mapUpper - mapLower) * smoothed;
			}
			
		}else{
			return mapLower;
		}
	}
	
	/** \brief Smooth map x from one range to another range. */
	inline double smoothStep(double x, double lower, double upper, double mapLower, double mapUpper){
		if(upper > lower){
			if(x <= lower){
				return mapLower;
				
			}else if(x >= upper){
				return mapUpper;
				
			}else{
				const double normalized = (x - lower) / (upper - lower);
				const double smoothed = normalized * normalized * normalized * (normalized * (normalized * 6.0 - 15.0) + 10.0);
				return mapLower + (mapUpper - mapLower) * smoothed;
			}
			
		}else{
			return mapLower;
		}
	}
}

#endif
