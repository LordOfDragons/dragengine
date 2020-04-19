/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSPROPFIELD_H_
#define _DEDSPROPFIELD_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>

class deScriptingDragonScript;
class dePropField;
class dsValue;
class dsRealObject;



/**
 * \brief Prop field peer.
 */
class dedsPropField : public deBaseScriptingPropField{
private:
	deScriptingDragonScript *pDS;
	dePropField *pPropField;
	dsValue *pValCB;
	bool pHasCB;
	deBaseScriptingPropField *pDelegee;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new peer. */
	dedsPropField( deScriptingDragonScript *ds, dePropField *propField );
	/** \brief Cleans up the peer. */
	virtual ~dedsPropField();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the callback object. */
	dsRealObject *GetCallback() const;
	/** \brief Sets the callback object. */
	void SetCallback( dsRealObject *object );
	/** \brief Retrieves the delegee or NULL. */
	inline deBaseScriptingPropField *GetDelegee() const{ return pDelegee; }
	/** \brief Sets the delegee or NULL. */
	void SetDelegee( deBaseScriptingPropField *delegee );
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Requests the game scripts to fill the prop field with instances.
	 * \details The density indicates the percentage of instances to use compared
	 *          to a fully populated prop field.
	 */
	virtual void CreateInstances( float density );
	/*@}*/
};

#endif
