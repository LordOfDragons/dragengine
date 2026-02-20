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

#ifndef _DEOALSKINTEXTURE_H_
#define _DEOALSKINTEXTURE_H_

#include <dragengine/common/string/decString.h>

class deSkinTexture;



/**
 * \brief Skin texture.
 * 
 * \todo Add Per-Geometry-Thickness parameters
 */
class deoalSkinTexture{
private:
	decString pName;
	
	bool pNoSound;
	float pAbsorption;
	float pAbsorptionLower;
	float pAbsorptionUpper;
	float pTransmission;
	float pTransmissionLower;
	float pTransmissionUpper;
	
	bool pAffectsSound;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin texture. */
	deoalSkinTexture();
	
	/** \brief Clean up skin texture. */
	~deoalSkinTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Material does not affect sound. */
	inline bool GetNoSound() const{ return pNoSound; }
	
	/** \brief Absorption as percentage of incoming sound volume. */
	inline float GetAbsorption() const{ return pAbsorption; }
	
	/** \brief Absorption scale factor for low frequencies. */
	inline float GetAbsorptionLower() const{ return pAbsorptionLower; }
	
	/** \brief Absorption scale factor for high frequencies. */
	inline float GetAbsorptionUpper() const{ return pAbsorptionUpper; }
	
	/** \brief Transmission range. */
	inline float GetTransmission() const{ return pTransmission; }
	
	/** \brief Transmission range scale factor for low frequencies. */
	inline float GetTransmissionLower() const{ return pTransmissionLower; }
	
	/** \brief Transmission range scale factor for high frequencies. */
	inline float GetTransmissionUpper() const{ return pTransmissionUpper; }
	
	
	
	/** \brief Material affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	
	
	
	/** \brief Process properties. */
	void ProcessProperties(const deSkinTexture &texture);
	/*@}*/
};

#endif
