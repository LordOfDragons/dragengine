/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DESYNSYNTHESIZERCURVE_H_
#define _DESYNSYNTHESIZERCURVE_H_

#include <dragengine/common/math/decMath.h>

class decCurveBezier;



/**
 * \brief Synthesizer curve.
 * 
 * Stores a bezier curve for fast evaluating.
 * 
 */
class desynSynthesizerCurve{
public:
	/** \brief Evaluation type. */
	enum eEvaluationType{
		/** \brief Constant value. */
		eetConstant,
		
		/** \brief Linear interpolation between unevenly spaced points. */
		eetLinear,
		
		/**
		 * \brief Linear interpolation between evenly sampled points.
		 * \details Curve is sampled into points with equal distance. Used for complex curves.
		 */
		eetSampled
	};
	
	
	
private:
	eEvaluationType pType;
	
	decVector2 *pPoints;
	float *pSamples;
	float *pFactors;
	int pCount;
	float pFirst;
	float pLast;
	float pStep;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create curve. */
	desynSynthesizerCurve();
	
	/** \brief Create curve. */
	desynSynthesizerCurve( const decCurveBezier &curve );
	
	/** \brief Create curve. */
	desynSynthesizerCurve( const decCurveBezier &curve, float ymin, float ymax );
	
	/** \brief Clean up curve. */
	virtual ~desynSynthesizerCurve();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set from curve.
	 * \details Determines automatically best evaluation type to use.
	 */
	void SetCurve( const decCurveBezier &curve );
	
	/**
	 * \brief Set from curve.
	 * \details Determines automatically best evaluation type to use.
	 */
	void SetCurve( const decCurveBezier &curve, float ymin, float ymax );
	
	/** \brief Evluate curve. */
	float Evaluate( float position ) const;
	/*@}*/
	/*@}*/
	
	
	
private:
	void pClear();
	
	void pSetCurveConstant( const decCurveBezier &curve, float yoffset, float yscale );
	void pSetCurveLinear( const decCurveBezier &curve, float yoffset, float yscale );
	void pSetCurveSampled( const decCurveBezier &curve, float yoffset, float yscale );
	
	float pEvaluateConstant( float position ) const;
	float pEvaluateLinear( float position ) const;
	float pEvaluateSampled( float position ) const;
};

#endif
