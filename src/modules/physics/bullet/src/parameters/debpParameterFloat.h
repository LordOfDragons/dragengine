/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPPARAMETERFLOAT_H_
#define _DEBPPARAMETERFLOAT_H_

#include "debpParameter.h"


/**
 * \brief Floatean parameter.
 */
class debpParameterFloat : public debpParameter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter. */
	debpParameterFloat( dePhysicsBullet &bullet );
	
	/** \brief Clean up parameter. */
	virtual ~debpParameterFloat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	virtual decString GetParameterValue();
	
	/** \brief Set current value. */
	virtual void SetParameterValue( const char *value );
	
	/** \brief Current value. */
	virtual float GetParameterFloat() = 0;
	
	/** \brief Set current value. */
	virtual void SetParameterFloat( float value ) = 0;
	/*@}*/
};

#endif
