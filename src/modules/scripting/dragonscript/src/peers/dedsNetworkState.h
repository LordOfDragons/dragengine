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

#ifndef _DEDSNETWORKSTATE_H_
#define _DEDSNETWORKSTATE_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingNetworkState.h>

class deScriptingDragonScript;
class deNetworkState;
class dsValue;
class dsRealObject;


/**
 * \brief Network state peer.
 */
class dedsNetworkState : public deBaseScriptingNetworkState{
private:
	deScriptingDragonScript &pDS;
	deNetworkState *pNetworkState;
	dsValue *pValCB;
	bool pHasCB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new peer. */
	dedsNetworkState( deScriptingDragonScript &ds, deNetworkState *state );
	
	/** \brief Cleans up the peer. */
	virtual ~dedsNetworkState();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief State value changed. */
	virtual void StateValueChanged( int index );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Callback object or NULL if not set. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or NULL if not set. */
	void SetCallback( dsRealObject *object );
	/*@}*/
};

#endif
