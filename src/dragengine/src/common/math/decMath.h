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

/**
 * \brief Class for mathematical calculations.
 *
 * @file decMath.h
 */


#ifndef _DECMATH_H_
#define _DECMATH_H_

#include <math.h>
#include <stdlib.h>

#include "../../dragengine_export.h"


/** \brief Value of pi. */
#ifdef PI
#	undef PI
#endif
#define PI 3.14159265f

/** \brief Value of half pi. */
#ifdef HALF_PI
#	undef HALF_PI
#endif
#define HALF_PI (PI / 2.0f)

/** \brief Value of two pi. */
#ifdef TWO_PI
#	undef TWO_PI
#endif
#define TWO_PI (PI * 2.0f)

/** \brief Degree to radians. */
#ifdef DEG2RAD
#	undef DEG2RAD
#endif
#define	DEG2RAD (PI / 180.0f)

/** \brief Radians to degree. */
#ifdef RAD2DEG
#	undef RAD2DEG
#endif
#define	RAD2DEG (180.0f / PI)

/** \brief Floating point thresholds (epsilons). */
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
	inline DE_DLL_EXPORT float min( float a, float b ){
		return ( a < b ) ? a : b;
	}
	
	/** \brief Smaller of two values. */
	inline DE_DLL_EXPORT double min( double a, double b ){
		return ( a < b ) ? a : b;
	}
	
	/** \brief Smaller of two values. */
	inline DE_DLL_EXPORT int min( int a, int b ){
		return ( a < b ) ? a : b;
	}
	
	/** \brief Smaller of three values. */
	inline DE_DLL_EXPORT float min( float a, float b, float c ){
		return min( min( a, b ), c );
	}
	
	/** \brief Smaller of three values. */
	inline DE_DLL_EXPORT double min( double a, double b, double c ){
		return min( min( a, b ), c );
	}
	
	/** \brief Smaller of three values. */
	inline DE_DLL_EXPORT int min( int a, int b, int c ){
		return min( min( a, b ), c );
	}
	
	/** \brief Smaller of four values. */
	inline DE_DLL_EXPORT float min( float a, float b, float c, float d ){
		return min( min( a, b ), min( c, d ) );
	}
	
	/** \brief Smaller of four values. */
	inline DE_DLL_EXPORT double min( double a, double b, double c, double d ){
		return min( min( a, b ), min( c, d ) );
	}
	
	/** \brief Smaller of four values. */
	inline DE_DLL_EXPORT int min( int a, int b, int c, int d ){
		return min( min( a, b ), min( c, d ) );
	}
	
	
	
	/** \brief Larger of two values. */
	inline DE_DLL_EXPORT float max( float a, float b ){
		return ( a > b ) ? a : b;
	}
	
	/** \brief Larger of two values. */
	inline DE_DLL_EXPORT double max( double a, double b ){
		return ( a > b ) ? a : b;
	}
	
	/** \brief Larger of two values. */
	inline DE_DLL_EXPORT int max( int a, int b ){
		return ( a > b ) ? a : b;
	}
	
	/** \brief Larger of three values. */
	inline DE_DLL_EXPORT float max( float a, float b, float c ){
		return max( max( a, b ), c );
	}
	
	/** \brief Larger of three values. */
	inline DE_DLL_EXPORT double max( double a, double b, double c ){
		return max( max( a, b ), c );
	}
	
	/** \brief Larger of three values. */
	inline DE_DLL_EXPORT int max( int a, int b, int c ){
		return max( max( a, b ), c );
	}
	
	/** \brief Larger of four values. */
	inline DE_DLL_EXPORT float max( float a, float b, float c, float d ){
		return max( max( a, b ), max( c, d ) );
	}
	
	/** \brief Larger of four values. */
	inline DE_DLL_EXPORT double max( double a, double b, double c, double d ){
		return max( max( a, b ), max( c, d ) );
	}
	
	/** \brief Larger of four values. */
	inline DE_DLL_EXPORT int max( int a, int b, int c, int d ){
		return max( max( a, b ), max( c, d ) );
	}
	
	
	
	/** \brief Clamp value to range. */
	inline DE_DLL_EXPORT float clamp( float x, float lower, float upper ){
		if( x <= lower ){
			return lower;
			
		}else if( x >= upper ){
			return upper;
			
		}else{
			return x;
		}
	}
	
	/** \brief Clamp value to range. */
	inline DE_DLL_EXPORT double clamp( double x, double lower, double upper ){
		if( x <= lower ){
			return lower;
			
		}else if( x >= upper ){
			return upper;
			
		}else{
			return x;
		}
	}
	
	/** \brief Clamp value to range. */
	inline DE_DLL_EXPORT int clamp( int x, int lower, int upper ){
		if( x <= lower ){
			return lower;
			
		}else if( x >= upper ){
			return upper;
			
		}else{
			return x;
		}
	}
	
	
	
	/** \brief Normalize value. */
	inline DE_DLL_EXPORT int normalize( int x, int lower, int upper ){
		if( lower < upper ){
			return lower + ( ( x - lower ) % ( upper - lower + 1 ) );
			
		}else{
			return lower;
		}
	}
	
	/** \brief Normalize value. */
	inline DE_DLL_EXPORT float normalize( float x, float lower, float upper ){
		if( lower < upper ){
			const float range = upper - lower;
			const float offset = x - lower;
			return ( offset - floorf( offset / range ) * range ) + lower;
			
		}else{
			return lower;
		}
	}
	
	/** \brief Normalize value. */
	inline DE_DLL_EXPORT double normalize( double x, double lower, double upper ){
		if( lower < upper ){
			const double range = upper - lower;
			const double offset = x - lower;
			return ( offset - floor( offset / range ) * range ) + lower;
			
		}else{
			return lower;
		}
	}
	
	/**
	 * \brief Normalize value.
	 * 
	 * Same as normalize(float,float,float) with lower set to 0.
	 */
	inline DE_DLL_EXPORT float normalize( float x, float upper ){
		if( upper > 0.0f ){
			return ( x - floorf( x / upper ) * upper );
			
		}else{
			return 0.0f;
		}
	}
	
	/**
	 * \brief Normalize value.
	 * 
	 * Same as normalize(double,double,double) with lower set to 0.
	 */
	inline DE_DLL_EXPORT double normalize( double x, double upper ){
		if( upper > 0.0 ){
			return ( x - floor( x / upper ) * upper );
			
		}else{
			return 0.0;
		}
	}
	
	
	
	/**
	 * \brief Blend between two values.
	 * 
	 * \em blend is clamped to the range between 0 and 1 before mixing.
	 */
	inline DE_DLL_EXPORT float mix( float a, float b, float blend ){
		if( blend <= 0.0f ){
			return a;
			
		}else if( blend >= 1.0f ){
			return b;
			
		}else{
			return a * ( 1.0f - blend ) + b * blend;
		}
	}
	
	/**
	 * \brief Blend between two values.
	 * 
	 * \em blend is clamped to the range between 0 and 1 before mixing.
	 */
	inline DE_DLL_EXPORT double mix( double a, double b, double blend ){
		if( blend <= 0.0 ){
			return a;
			
		}else if( blend >= 1.0 ){
			return b;
			
		}else{
			return a * ( 1.0 - blend ) + b * blend;
		}
	}
	
	
	
	/** \brief Linear map x to the range from 0 to 1. */
	inline DE_DLL_EXPORT float linearStep( float x, float lower, float upper ){
		if( upper > lower ){
			if( x <= lower ){
				return 0.0f;
				
			}else if( x >= upper ){
				return 1.0f;
				
			}else{
				return ( x - lower ) / ( upper - lower );
			}
			
		}else{
			return 0.0f;
		}
	}
	
	/** \brief Linear map x to the range from 0 to 1. */
	inline DE_DLL_EXPORT double linearStep( double x, double lower, double upper ){
		if( upper > lower ){
			if( x <= lower ){
				return 0.0;
				
			}else if( x >= upper ){
				return 1.0;
				
			}else{
				return ( x - lower ) / ( upper - lower );
			}
			
		}else{
			return 0.0;
		}
	}
	
	/** \brief Linear map x from one range to another range. */
	inline DE_DLL_EXPORT float linearStep( float x, float lower, float upper, float mapLower, float mapUpper ){
		if( upper > lower ){
			if( x <= lower ){
				return mapLower;
				
			}else if( x >= upper ){
				return mapUpper;
				
			}else{
				return mapLower + ( mapUpper - mapLower ) * ( x - lower ) / ( upper - lower );
			}
			
		}else{
			return mapLower;
		}
	}
	
	/** \brief Linear map x from one range to another range. */
	inline DE_DLL_EXPORT double linearStep( double x, double lower, double upper, double mapLower, double mapUpper ){
		if( upper > lower ){
			if( x <= lower ){
				return mapLower;
				
			}else if( x >= upper ){
				return mapUpper;
				
			}else{
				return mapLower + ( mapUpper - mapLower ) * ( x - lower ) / ( upper - lower );
			}
			
		}else{
			return mapLower;
		}
	}
	
	
	
	/** \brief Smooth map x to the range from 0 to 1. */
	inline DE_DLL_EXPORT float smoothStep( float x, float lower, float upper ){
		if( upper > lower ){
			if( x <= lower ){
				return 0.0f;
				
			}else if( x >= upper ){
				return 1.0f;
				
			}else{
				const float normalized = ( x - lower ) / ( upper - lower );
				return normalized * normalized * normalized * ( normalized * ( normalized * 6.0f - 15.0f ) + 10.0f );
			}
			
		}else{
			return 0.0f;
		}
	}
	
	/** \brief Smooth map x to the range from 0 to 1. */
	inline DE_DLL_EXPORT double smoothStep( double x, double lower, double upper ){
		if( upper > lower ){
			if( x <= lower ){
				return 0.0;
				
			}else if( x >= upper ){
				return 1.0;
				
			}else{
				const double normalized = ( x - lower ) / ( upper - lower );
				return normalized * normalized * normalized * ( normalized * ( normalized * 6.0 - 15.0 ) + 10.0 );
			}
			
		}else{
			return 0.0;
		}
	}
	
	/** \brief Smooth map x from one range to another range. */
	inline DE_DLL_EXPORT float smoothStep( float x, float lower, float upper, float mapLower, float mapUpper ){
		if( upper > lower ){
			if( x <= lower ){
				return mapLower;
				
			}else if( x >= upper ){
				return mapUpper;
				
			}else{
				const float normalized = ( x - lower ) / ( upper - lower );
				const float smoothed = normalized * normalized * normalized * ( normalized * ( normalized * 6.0f - 15.0f ) + 10.0f );
				return mapLower + ( mapUpper - mapLower ) * smoothed;
			}
			
		}else{
			return mapLower;
		}
	}
	
	/** \brief Smooth map x from one range to another range. */
	inline DE_DLL_EXPORT double smoothStep( double x, double lower, double upper, double mapLower, double mapUpper ){
		if( upper > lower ){
			if( x <= lower ){
				return mapLower;
				
			}else if( x >= upper ){
				return mapUpper;
				
			}else{
				const double normalized = ( x - lower ) / ( upper - lower );
				const double smoothed = normalized * normalized * normalized * ( normalized * ( normalized * 6.0 - 15.0 ) + 10.0 );
				return mapLower + ( mapUpper - mapLower ) * smoothed;
			}
			
		}else{
			return mapLower;
		}
	}
	
	
	
	/** \brief Random value. */
	inline DE_DLL_EXPORT int random( int lower, int upper ){
		if( upper > lower ){
			const float factor2 = ( float )rand() / ( float )RAND_MAX;
			const float factor1 = 1.0f - factor2;
			return clamp( ( int )( ( float )lower * factor1 + ( float )( upper + 1 ) * factor2 ), lower, upper );
			
		}else{
			return lower;
		}
	}
	
	/** \brief Random value. */
	inline DE_DLL_EXPORT float random( float lower, float upper ){
		if( upper > lower ){
			const float factor2 = ( float )rand() / ( float )RAND_MAX;
			const float factor1 = 1.0f - factor2;
			return clamp( lower * factor1 + upper * factor2, lower, upper );
			
		}else{
			return lower;
		}
	}
}

#endif
