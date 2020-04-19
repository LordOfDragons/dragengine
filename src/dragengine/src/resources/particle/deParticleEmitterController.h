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

#ifndef _DEPARTICLEEMITTERCONTROLLER_H_
#define _DEPARTICLEEMITTERCONTROLLER_H_

#include "../../common/string/decString.h"


/**
 * \brief Particle Emitter Class.
 * 
 * Particle emitter controllers provide the user with control over how the individual particles
 * are cast. Controllers are written in a way that you can use them without having to do scaling
 * or transformation on your own. You can provide a minimum and maximum value matching your setup.
 * The controller takes care of transforming the current value into the proper 0 to 1 range
 * required by the cast curves in parameters. The current value is set by the user and is clamped
 * to the range specified before. The real value is in the range 0 to 1 and is used by the cast
 * curves. The real value can only be read. Furthermore a controller can be frozen. If a
 * controller is frozen the current value can be changed. This is useful to prevent a controller
 * from changing without having to change all code path to account for the frozen state.
 */
class deParticleEmitterController{
private:
	decString pName;
	float pLower;
	float pUpper;
	float pValue;
	bool pClamp;
	bool pFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller with range 0 to 1 and the value 0. */
	deParticleEmitterController( const char *name = "Controller" );
	
	/** \brief Clean up controller. */
	~deParticleEmitterController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Lower value. */
	inline float GetLower() const{ return pLower; }
	
	/** \brief Upper value. */
	inline float GetUpper() const{ return pUpper; }
	
	/** \brief Set value range. */
	void SetRange( float lower, float upper );
	
	/** \brief Current value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set current value. */
	void SetValue( float value );
	
	/** \brief Increment value. */
	void Increment( float amount );
	
	/** \brief Che controller is frozen. */
	inline bool GetFrozen() const{ return pFrozen; }
	
	/** \brief Set if controller is frozen. */
	void SetFrozen( bool frozen );
	
	/** \brief Values passed the range are clamped or wrapped around. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if values passed the range are clamped or wrapped around. */
	void SetClamp( bool clamp );
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller parameters. */
	deParticleEmitterController &operator=( const deParticleEmitterController &controller );
	/*@}*/
	/*@}*/
	
	
	
private:
	void pUpdateValue();
};

#endif
