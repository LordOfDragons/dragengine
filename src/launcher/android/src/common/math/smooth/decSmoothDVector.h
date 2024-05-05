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

#ifndef _DECSMOOTHDVECTOR_H_
#define _DECSMOOTHDVECTOR_H_

#include "../decMath.h"

/**
 * \brief DVector value with smooth adjusting over time.
 * \details Distance over time is used as change speed.
 */
class decSmoothDVector{
private:
	decDVector pValue;
	decDVector pGoal;
	double pAdjustTime;
	double pAdjustRange;
	double pChangeSpeed;
	double pFactorTime;
	double pFactorLimit;
	float pFactorDrop;
	bool pSmoothed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new smooth dvector.
	 * \details Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 *          Change speed limit is set to 1 unit per second.
	 */
	decSmoothDVector();
	
	/** \brief Create copy of a smooth dvector. */
	decSmoothDVector( const decSmoothDVector &copy );
	
	/** \brief Clean up smooth dvector. */
	~decSmoothDVector();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	inline const decDVector &GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( const decDVector &value );
	
	/** \brief Goal value. */
	inline const decDVector &GetGoal() const{ return pGoal; }
	
	/** \brief Set goal value. */
	void SetGoal( const decDVector &goal );
	
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
	 * \details Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( double elapsed );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth dvector equals this smooth dvector.
	 * \details Two smooth dvectors are equal if their value is equal.
	 */
	bool operator==( const decSmoothDVector &other ) const;
	
	/**
	 * \brief Determine if two smooth dvector are not equal.
	 * \details Two smooth dvectors are not equal if their value is not equal.
	 */
	bool operator!=( const decSmoothDVector &other ) const;
	
	/** \brief Copy another smooth dvector to this smooth dvector. */
	decSmoothDVector &operator=( const decSmoothDVector &other );
	/*@}*/
	
private:
	void pUpdateFactors();
};

#endif
