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

#ifndef _DEBPPARAMETER_H_
#define _DEBPPARAMETER_H_

#include <dragengine/common/string/decStringSet.h>
#include <dragengine/systems/modules/deModuleParameter.h>


class dePhysicsBullet;


/**
 * \brief Parameter.
 * 
 * Base class for all openal parameters. Every parameter stores informations about
 * the parameter itself and provides methods to retrieves or alter the current value.
 */
class debpParameter : public deModuleParameter{
protected:
	dePhysicsBullet &pBullet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter. */
	debpParameter( dePhysicsBullet &bullet );
	
	/** \brief Clean up parameter. */
	virtual ~debpParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Current value. */
	virtual decString GetParameterValue() = 0;
	
	/** \brief Set current value. */
	virtual void SetParameterValue( const char *value ) = 0;
	/*@}*/
};

#endif
