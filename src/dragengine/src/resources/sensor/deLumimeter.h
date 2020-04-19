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

#ifndef _DELUMIMETER_H_
#define _DELUMIMETER_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"


class deLumimeterManager;
class deBaseGraphicLumimeter;
class deWorld;


/**
 * \brief Lumimeter Resource.
 *
 * Lumimeters are sensors to measure the light conditions.
 * This services is provided by the graphic module. A lumimeter
 * measures light at a point in space with an optional cone
 * restriction. Lumimeters without a cone restriction are omni
 * direction lumimeters measuring light from all directions.
 * Using a cone a lumi meter only measures the light coming
 * from source located in the cone. An additional drop off
 * parameter can be specified to determine the attenutation
 * of light sources near the cone boundaries. A lumimeter is
 * considered omni directional if the cone outer angle is
 * PI or above. Lumimeters store the luminance as well as the
 * color.
 */
class deLumimeter : public deResource{
private:
	decDVector pPosition;
	decVector pDirection;
	float pConeInnerAngle;
	float pConeOuterAngle;
	float pConeExponent;
	
	deBaseGraphicLumimeter *pPeerGraphic;
	
	deWorld *pParentWorld;
	deLumimeter *pLLWorldPrev;
	deLumimeter *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new lumimeter. */
	deLumimeter( deLumimeterManager *manager );
	
protected:
	/**
	 * \brief Clean up lumimeter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLumimeter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set direction. */
	void SetDirection( const decVector &direction );
	
	/** \brief Cone inner angle. */
	inline float GetConeInnerAngle() const{ return pConeInnerAngle; }
	
	/** \brief Set cone inner angle. */
	void SetConeInnerAngle( float angle );
	
	/** \brief Cone outer angle. */
	inline float GetConeOuterAngle() const{ return pConeOuterAngle; }
	
	/** \brief Set cone outer angle. */
	void SetConeOuterAngle( float angle );
	
	/** \brief Cone exponent. */
	inline float GetConeExponent() const{ return pConeExponent; }
	
	/** \brief Set cone exponent. */
	void SetConeExponent( float exponent );
	/*@}*/
	
	
	
	/** \name Measuring */
	/*@{*/
	/** \brief Measures the luminance. */
	float MeasureLuminance();
	
	/** \brief Measures the color. */
	decColor MeasureColor();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicLumimeter *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicLumimeter *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous lumimeter in the parent world linked list. */
	inline deLumimeter *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next lumimeter in the parent world linked list. */
	void SetLLWorldPrev( deLumimeter *lumimeter );
	
	/** \brief Next lumimeter in the parent world linked list. */
	inline deLumimeter *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next lumimeter in the parent world linked list. */
	void SetLLWorldNext( deLumimeter *lumimeter );
	/*@}*/
};

#endif
