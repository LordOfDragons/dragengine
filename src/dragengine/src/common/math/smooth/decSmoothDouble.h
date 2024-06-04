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

#ifndef _DECSMOOTHFLOAT_H_
#define _DECSMOOTHFLOAT_H_

#include "../../../dragengine_export.h"

/**
 * \brief Double floating point value with smooth adjusting over time.
 */
class DE_DLL_EXPORT decSmoothDouble{
private:
	double pValue;
	double pGoal;
	double pAdjustTime;
	double pAdjustRange;
	double pChangeSpeed;
	double pFactorTime;
	double pFactorLimit;
	double pFactorDrop;
	bool pSmoothed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new smooth double.
	 * 
	 * Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 * Change speed limit is set to 1 unit per second.
	 */
	decSmoothDouble();
	
	/** \brief Create copy of a smooth double. */
	decSmoothDouble( const decSmoothDouble &copy );
	
	/** \brief Clean up smooth double. */
	~decSmoothDouble();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	inline double GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( double value );
	
	/** \brief Goal value. */
	inline double GetGoal() const{ return pGoal; }
	
	/** \brief Set goal value. */
	void SetGoal( double goal );
	
	/** \brief Adjustment time in seconds. */
	inline double GetAdjustTime() const{ return pAdjustTime; }
	
	/** \brief Adjustment time in seconds. Clamped to 0 or larger. */
	void SetAdjustTime( double adjustTime );
	
	/** \brief Adjustment range in units. */
	inline double GetAdjustRange() const{ return pAdjustRange; }
	
	/** \brief Set adjustment range in units. Clamped to 0 or larger. */
	void SetAdjustRange( double range );
	
	/** \brief Change speed of the value in units per second. */
	inline double GetChangeSpeed() const{ return pChangeSpeed; }
	
	/** \brief Set change speed of the value in units per second. */
	void SetChangeSpeed( double changeSpeed );
	
	
	
	/** \brief Reset value, goal and change speed to 0. */
	void Reset();
	
	/**
	 * \brief Update value with goal using current smoothing parameters.
	 * 
	 * Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( double elapsed );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth double equals this smooth double.
	 * 
	 * Two smooth doubles are equal if their value difference is less than \em DOUBLE_SAFE_EPSILON.
	 */
	bool operator==( const decSmoothDouble &other ) const;
	
	/**
	 * \brief Determine if two smooth double are not equal.
	 * 
	 * Two smooth doubles are not equal if their value difference is larger than or equal to \em DOUBLE_SAFE_EPSILON.
	 */
	bool operator!=( const decSmoothDouble &other ) const;
	
	/** \brief Copy another smooth double to this smooth double. */
	decSmoothDouble &operator=( const decSmoothDouble &other );
	/*@}*/
	
	
	
private:
	void pUpdateFactors();
};

#endif
