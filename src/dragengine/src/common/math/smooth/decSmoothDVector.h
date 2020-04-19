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

#ifndef _DECSMOOTHDVECTOR_H_
#define _DECSMOOTHDVECTOR_H_

#include "../decMath.h"

/**
 * \brief DVector value with smooth adjusting over time.
 * 
 * Distance over time is used as change speed.
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
	 * 
	 * Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 * Change speed limit is set to 1 unit per second.
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
	 * 
	 * Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( double elapsed );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth dvector equals this smooth dvector.
	 * 
	 * Two smooth dvectors are equal if their value is equal.
	 */
	bool operator==( const decSmoothDVector &other ) const;
	
	/**
	 * \brief Determine if two smooth dvector are not equal.
	 * 
	 * Two smooth dvectors are not equal if their value is not equal.
	 */
	bool operator!=( const decSmoothDVector &other ) const;
	
	/** \brief Copy another smooth dvector to this smooth dvector. */
	decSmoothDVector &operator=( const decSmoothDVector &other );
	/*@}*/
	
	
	
private:
	void pUpdateFactors();
};

#endif
