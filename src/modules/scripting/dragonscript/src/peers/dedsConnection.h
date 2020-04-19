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

#ifndef _DEDSCONNECTION_H_
#define _DEDSCONNECTION_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingConnection.h>

class dsValue;
class dsRealObject;
class deScriptingDragonScript;
class deConnection;


/**
 * \brief Connection peer.
 */
class dedsConnection : public deBaseScriptingConnection{
private:
	deScriptingDragonScript &pDS;
	deConnection *pConnection;
	dsValue *pValCB;
	bool pHasCB;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsConnection( deScriptingDragonScript &ds, deConnection *connection );
	
	/** \brief Clean up peer. */
	virtual ~dedsConnection();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Connection has been closed by the local or remote host. */
	virtual void ConnectionClosed();
	
	/** \brief A long message is in progress of receiving. */
	virtual void MessageProgress( int bytesReceived );
	
	/** \brief A message has been received. */
	virtual void MessageReceived( deNetworkMessage *message );
	
	/**
	 * \brief Host send state to link.
	 * \param networkState Network state to use for this side of the link.
	 * \param message Additional information.
	 * \returns true to accept the link or false to deny it.
	 */
	virtual bool LinkState( deNetworkState *networkState, deNetworkMessage *message );
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
