/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECSMOOTHVECTOR2_H_
#define _DECSMOOTHVECTOR2_H_

#include "../decMath.h"

/**
 * \brief 2-component vector value with smooth adjusting over time.
 * \details Distance over time is used as change speed.
 */
class decSmoothVector2{
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
	 * \details Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 *          Change speed limit is set to 1 unit per second.
	 */
	decSmoothVector2();
	
	/** \brief Create copy of a smooth vector. */
	decSmoothVector2( const decSmoothVector2 &copy );
	
	/** \brief Clean up smooth vector. */
	~decSmoothVector2();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	inline const decVector2 &GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( const decVector2 &value );
	
	/** \brief Goal value. */
	inline const decVector2 &GetGoal() const{ return pGoal; }
	
	/** \brief Set goal value. */
	void SetGoal( const decVector2 &goal );
	
	/** \brief Adjustment time in seconds. */
	inline float GetAdjustTime() const{ return pAdjustTime; }
	
	/** \brief Adjustment time in seconds. Clamped to 0 or larger. */
	void SetAdjustTime( float adjustTime );
	
	/** \brief Adjustment range in units. */
	inline float GetAdjustRange() const{ return pAdjustRange; }
	
	/** \brief Set adjustment range in units. Clamped to 0 or larger. */
	void SetAdjustRange( float range );
	
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
	 * \brief Determine if another smooth vector equals this smooth vector.
	 * \details Two smooth vectors are equal if their value is equal.
	 */
	bool operator==( const decSmoothVector2 &other ) const;
	
	/**
	 * \brief Determine if two smooth vector are not equal.
	 * \details Two smooth vectors are not equal if their value is not equal.
	 */
	bool operator!=( const decSmoothVector2 &other ) const;
	
	/** \brief Copy another smooth vector to this smooth vector. */
	decSmoothVector2 &operator=( const decSmoothVector2 &other );
	/*@}*/
	
private:
	void pUpdateFactors();
};

#endif
