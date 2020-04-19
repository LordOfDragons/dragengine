/* 
 * Drag[en]gine IGDE
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

// include only once
#ifndef _IGDESMPROPFIELD_H_
#define _IGDESMPROPFIELD_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingPropField.h>



/**
 * @brief IGDE Script Module Prop Field Peer.
 *
 * Prop Field peer for the IGDRE Script Module. If a delegee is assigned
 * all events are forwarded to the delegee.
 */
class igdeSMPropField : public deBaseScriptingPropField{
private:
	deBaseScriptingPropField *pDelegee;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new gui peer. */
	igdeSMPropField();
	/** Cleans up the peer. */
	virtual ~igdeSMPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the delegee or NULL. */
	inline deBaseScriptingPropField *GetDelegee() const{ return pDelegee; }
	/** Sets the delegee or NULL. */
	void SetDelegee( deBaseScriptingPropField *delegee );
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * Requests the game scripts to fill the prop field with instances. The density
	 * indicates the percentage of instances to use compared to a fully populated
	 * prop field.
	 */
	virtual void CreateInstances( float density );
	/*@}*/
};

// end of include only once
#endif
