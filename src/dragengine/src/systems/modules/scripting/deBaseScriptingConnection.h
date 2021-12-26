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

#ifndef _DEBASESCRIPTINGCONNECTION_H_
#define _DEBASESCRIPTINGCONNECTION_H_

#include <stdint.h>

#include "../../../dragengine_export.h"

class deNetworkState;
class deNetworkMessage;


/**
 * \brief Scripting Module Connection Peer.
 */
class DE_DLL_EXPORT deBaseScriptingConnection{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseScriptingConnection();
	
	/** \brief Clean up peer. */
	virtual ~deBaseScriptingConnection();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Connection closed by local or remote host. */
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
};

#endif
