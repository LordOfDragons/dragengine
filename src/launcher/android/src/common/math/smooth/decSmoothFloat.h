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

/**
 * \brief Floating point value with smooth adjusting over time.
 */
class decSmoothFloat{
private:
	float pValue;
	float pGoal;
	float pAdjustTime;
	float pAdjustRange;
	float pChangeSpeed;
	float pFactorTime;
	float pFactorLimit;
	float pFactorDrop;
	bool pSmoothed;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new smooth float.
	 * \details Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 *          Change speed limit is set to 1 unit per second.
	 */
	decSmoothFloat();
	
	/** \brief Create copy of a smooth float. */
	decSmoothFloat( const decSmoothFloat &copy );
	
	/** \brief Clean up smooth float. */
	~decSmoothFloat();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( float value );
	
	/** \brief Goal value. */
	inline float GetGoal() const{ return pGoal; }
	
	/** \brief Set goal value. */
	void SetGoal( float goal );
	
	/** \brief Adjustment time in seconds. */
	inline float GetAdjustTime() const{ return pAdjustTime; }
	
	/** \brief Adjustment time in seconds. Clamped to 0 or larger. */
	void SetAdjustTime( float adjustTime );
	
	/** \brief Adjustment range in units. */
	inline float GetAdjustRange() const{ return pAdjustRange; }
	
	/** \brief Set adjustment range in units. Clamped to 0 or larger. */
	void SetAdjustRange( float limit );
	
	/** \brief Change speed of the value in units per second. */
	inline float GetChangeSpeed() const{ return pChangeSpeed; }
	
	/** \brief Set change speed of the value in units per second. */
	void SetChangeSpeed( float changeSpeed );
	
	
	
	/** \brief Reset value, goal and change speed to 0. */
	void Reset();
	
	/**
	 * \brief Update value with goal using current smoothing parameters.
	 * \details Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( float elapsed );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth float equals this smooth float.
	 * \details Two smooth floats are equal if their value difference is less than \em FLOAT_SAFE_EPSILON.
	 */
	bool operator==( const decSmoothFloat &other ) const;
	
	/**
	 * \brief Determine if two smooth float are not equal.
	 * \details Two smooth floats are not equal if their value difference is larger than or equal to \em FLOAT_SAFE_EPSILON.
	 */
	bool operator!=( const decSmoothFloat &other ) const;
	
	/** \brief Copy another smooth float to this smooth float. */
	decSmoothFloat &operator=( const decSmoothFloat &other );
	/*@}*/
	
private:
	void pUpdateFactors();
};

#endif
