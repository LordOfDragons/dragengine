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

#ifndef _DECPRNG_H_
#define _DECPRNG_H_

#include "../../dragengine_export.h"


/** \brief The default upper integer value range. */
#define DE_RANDOM_INT_UPPER 2147483647


/**
 * \brief Psuedo Random Number Generator.
 * 
 * Provides a simple PRNG that works the same across all supported platform
 * no matter the actual random implementation used by the platform.
 */
class DE_DLL_EXPORT decPRNG{
private:
	unsigned int pSeed;
	unsigned int pNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new pseudo random number generator.
	 * 
	 * Uses the current system time as seed.
	 */
	decPRNG();
	
	/**
	 * \brief Create new pseudo random number generator.
	 * \param[in] seed The seed to use.
	 */
	decPRNG( unsigned int seed );
	
	/** \brief Clean up pseudo random number generator. */
	~decPRNG();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resets the pseudo random number generator to the construction state. */
	void Reset();
	
	/** \brief Resets the pseudo random number generator with a new seed. */
	void Reset( unsigned int seed );
	
	/**
	 * \brief Next random number as integer value.
	 * \returns Random number as integer value in the range from 0 to \ref DE_RANDOM_INT_UPPER.
	 */
	int RandomInt();
	
	/**
	 * \brief Next random number as integer value.
	 * \param[in] lower The lower value to map the random value to.
	 * \param[in] upper The upper value to map the random value to. If upper is less than
	 *                  lower upper is considered to be equal to lower.
	 * \returns Random number as integer value in the range from lower to upper.
	 */
	int RandomInt( int lower, int upper );
	
	/**
	 * \brief Next random number as float value.
	 * \returns Random number as float value in the range from 0 to 1.
	 */
	float RandomFloat();
	
	/**
	 * \brief Next random number as float value.
	 * \param[in] lower The lower value to map the random value to.
	 * \param[in] upper The upper value to map the random value to. If upper is less than
	 *                  lower upper is considered to be equal to lower.
	 * \returns Random number as float value in the range from lower to upper.
	 */
	float RandomFloat( float lower, float upper );
	
	/**
	 * \brief Next random number as double value.
	 * \returns Random number as double value in the range from 0 to 1.
	 */
	double RandomDouble();
	
	/**
	 * \brief Next random number as double value.
	 * \param[in] lower The lower value to map the random value to.
	 * \param[in] upper The upper value to map the random value to. If upper is less than
	 *                  lower upper is considered to be equal to lower.
	 * \returns Random number as double value in the range from lower to upper.
	 */
	double RandomDouble( double lower, double upper );
	/*@}*/
	
	
	
private:
	unsigned int pNextRandomValue();
};

#endif
