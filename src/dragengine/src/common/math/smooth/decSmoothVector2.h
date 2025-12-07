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

#ifndef _DECSMOOTHVECTOR2_H_
#define _DECSMOOTHVECTOR2_H_

#include "../decMath.h"

/**
 * \brief 2-component vector value with smooth adjusting over time.
 * 
 * Distance over time is used as change speed.
 */
class DE_DLL_EXPORT decSmoothVector2{
private:
	decVector2 pValue;
	decVector2 pGoal;
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
	 * \brief Create a new smooth vector.
	 * 
	 * Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 * Change speed limit is set to 1 unit per second.
	 */
	decSmoothVector2();
	
	/** \brief Create copy of a smooth vector. */
	decSmoothVector2(const decSmoothVector2 &copy);
	
	/** \brief Clean up smooth vector. */
	~decSmoothVector2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	inline const decVector2 &GetValue() const{return pValue;}
	
	/** \brief Set current value. */
	void SetValue(const decVector2 &value);
	
	/** \brief Goal value. */
	inline const decVector2 &GetGoal() const{return pGoal;}
	
	/** \brief Set goal value. */
	void SetGoal(const decVector2 &goal);
	
	/** \brief Adjustment time in seconds. */
	inline float GetAdjustTime() const{return pAdjustTime;}
	
	/** \brief Adjustment time in seconds. Clamped to 0 or larger. */
	void SetAdjustTime(float adjustTime);
	
	/** \brief Adjustment range in units. */
	inline float GetAdjustRange() const{return pAdjustRange;}
	
	/** \brief Set adjustment range in units. Clamped to 0 or larger. */
	void SetAdjustRange(float range);
	
	/** \brief Change speed of the value in units per second. */
	inline float GetChangeSpeed() const{return pChangeSpeed;}
	
	/** \brief Set change speed of the value in units per second. */
	void SetChangeSpeed(float changeSpeed);
	
	
	
	/** \brief Reset value, goal and change speed to 0. */
	void Reset();
	
	/**
	 * \brief Update value with goal using current smoothing parameters.
	 * 
	 * Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update(float elapsed);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth vector equals this smooth vector.
	 * 
	 * Two smooth vectors are equal if their value is equal.
	 */
	bool operator==(const decSmoothVector2 &other) const;
	
	/**
	 * \brief Determine if two smooth vector are not equal.
	 * 
	 * Two smooth vectors are not equal if their value is not equal.
	 */
	bool operator!=(const decSmoothVector2 &other) const;
	
	/** \brief Copy another smooth vector to this smooth vector. */
	decSmoothVector2 &operator=(const decSmoothVector2 &other);
	/*@}*/
	
	
	
private:
	void pUpdateFactors();
};

#endif
