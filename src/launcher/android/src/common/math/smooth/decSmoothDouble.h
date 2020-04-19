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

#ifndef _DECSMOOTHFLOAT_H_
#define _DECSMOOTHFLOAT_H_

/**
 * \brief Double floating point value with smooth adjusting over time.
 */
class decSmoothDouble{
private:
	double pValue;
	double pGoal;
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
	 * \brief Create a new smooth double.
	 * \details Value, goal and change speed are set to 0. Adjust time is set to 1s.
	 *          Change speed limit is set to 1 unit per second.
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
	 * \details Goal is not modified.
	 * \param[in] elapsed Elapsed time in seconds. If less than 0.001s update is skipped.
	 */
	void Update( double elapsed );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if another smooth double equals this smooth double.
	 * \details Two smooth doubles are equal if their value difference is less than \em DOUBLE_SAFE_EPSILON.
	 */
	bool operator==( const decSmoothDouble &other ) const;
	
	/**
	 * \brief Determine if two smooth double are not equal.
	 * \details Two smooth doubles are not equal if their value difference is larger than or equal to \em DOUBLE_SAFE_EPSILON.
	 */
	bool operator!=( const decSmoothDouble &other ) const;
	
	/** \brief Copy another smooth double to this smooth double. */
	decSmoothDouble &operator=( const decSmoothDouble &other );
	/*@}*/
	
private:
	void pUpdateFactors();
};

#endif
