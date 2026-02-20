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
	decPRNG(unsigned int seed);
	
	/** \brief Clean up pseudo random number generator. */
	~decPRNG();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Resets the pseudo random number generator to the construction state. */
	void Reset();
	
	/** \brief Resets the pseudo random number generator with a new seed. */
	void Reset(unsigned int seed);
	
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
	int RandomInt(int lower, int upper);
	
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
	float RandomFloat(float lower, float upper);
	
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
	double RandomDouble(double lower, double upper);
	/*@}*/
	
	
	
private:
	unsigned int pNextRandomValue();
};

#endif
