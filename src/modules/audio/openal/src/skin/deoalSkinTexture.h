/* 
 * Drag[en]gine OpenAL Audio Module
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
	void ProcessProperties( const deSkinTexture &texture );
	/*@}*/
};

#endif
