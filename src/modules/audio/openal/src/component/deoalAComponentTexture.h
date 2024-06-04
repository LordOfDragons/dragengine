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
