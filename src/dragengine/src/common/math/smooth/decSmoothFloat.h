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
	 * 
	 * Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 * Change speed limit is set to 1 unit per second.
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
	 * 
	 * Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( float elapsed );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth float equals this smooth float.
	 * 
	 * Two smooth floats are equal if their value difference is less than \em FLOAT_SAFE_EPSILON.
	 */
	bool operator==( const decSmoothFloat &other ) const;
	
	/**
	 * \brief Determine if two smooth float are not equal.
	 * 
	 * Two smooth floats are not equal if their value difference is larger than or equal to \em FLOAT_SAFE_EPSILON.
	 */
	bool operator!=( const decSmoothFloat &other ) const;
	
	/** \brief Copy another smooth float to this smooth float. */
	decSmoothFloat &operator=( const decSmoothFloat &other );
	/*@}*/
	
	
	
private:
	void pUpdateFactors();
};

#endif
