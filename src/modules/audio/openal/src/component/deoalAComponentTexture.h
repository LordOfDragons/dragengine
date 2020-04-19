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

#ifndef _DEOALACOMPONENTTEXTURE_H_
#define _DEOALACOMPONENTTEXTURE_H_

#include <dragengine/deObject.h>

class deoalAComponent;
class deoalASkin;
class deoalSkinTexture;



/**
 * \brief Component texture.
 */
class deoalAComponentTexture : public deObject{
private:
	deoalAComponent &pComponent;
	const int pIndex;
	
	deoalASkin *pSkin;
	
	const deoalASkin *pUseSkin;
	const deoalSkinTexture *pUseTexture;
	
	bool pAffectsSound;
	float pAbsorptionLow;
	float pAbsorptionMedium;
	float pAbsorptionHigh;
	float pTransmissionLow;
	float pTransmissionMedium;
	float pTransmissionHigh;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component texture. */
	deoalAComponentTexture( deoalAComponent &component, int index );
	
protected:
	/** \brief Clean up component texture. */
	virtual ~deoalAComponentTexture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Component. */
	inline deoalAComponent &GetComponent() const{ return pComponent; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** \brief Skin. */
	inline deoalASkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( deoalASkin *skin );
	
	
	
	/** \brief Use skin or NULL. */
	inline const deoalASkin *GetUseSkin() const{ return pUseSkin; }
	
	/** \brief Use texture or NULL. */
	inline const deoalSkinTexture *GetUseTexture() const{ return pUseTexture; }
	
	/** \brief Component texture affects sound. */
	inline bool GetAffectsSound() const{ return pAffectsSound; }
	
	
	
	/** \brief Absorption for low frequencies. */
	inline float GetAbsorptionLow() const{ return pAbsorptionLow; }
	
	/** \brief Absorption for medium frequencies. */
	inline float GetAbsorptionMedium() const{ return pAbsorptionMedium; }
	
	/** \brief Absorption for high frequencies. */
	inline float GetAbsorptionHigh() const{ return pAbsorptionHigh; }
	
	/** \brief Transmission range for low frequencies. */
	inline float GetTransmissionLow() const{ return pTransmissionLow; }
	
	/** \brief Transmission range for medium frequencies. */
	inline float GetTransmissionMedium() const{ return pTransmissionMedium; }
	
	/** \brief Transmission range for high frequencies. */
	inline float GetTransmissionHigh() const{ return pTransmissionHigh; }
	
	
	
	/** \brief Update use texture. */
	void UpdateUseTexture();
	
	/** \brief Update sound parameters from use texture. */
	void UpdateSoundParameters();
	/*@}*/
};

#endif
