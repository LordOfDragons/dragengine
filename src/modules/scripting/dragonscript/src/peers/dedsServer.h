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

#ifndef _DEDSSERVER_H_
#define _DEDSSERVER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingServer.h>

class deScriptingDragonScript;
class dsValue;
class dsRealObject;



/**
 * \brief Server peer.
 */
class dedsServer : public deBaseScriptingServer{
private:
	deScriptingDragonScript *pDS;
	deServer *pServer;
	dsValue *pValCB;
	bool pHasCB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsServer( deScriptingDragonScript *ds, deServer *server );
	
	/** \brief Clean up peer. */
	virtual ~dedsServer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Callback object or NULL if not set. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or NULL if not set. */
	void SetCallback( dsRealObject *object );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Remote host connected.
	 */
	virtual void ClientConnected( deConnection *connection );
	/*@}*/
};

#endif
