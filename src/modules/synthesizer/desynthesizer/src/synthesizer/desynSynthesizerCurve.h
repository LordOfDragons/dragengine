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
