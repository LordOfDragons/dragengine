/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNCONNECTION_H_
#define _DEBNCONNECTION_H_

#include "deNetworkBasic.h"

#include <dragengine/systems/modules/network/deBaseNetworkConnection.h>

class deConnection;
class deNetworkBasic;
class debnMessageManager;
class debnSocket;
class debnAddress;
class debnState;
class debnStateLinkManager;
class debnStateLinkList;
class debnMessageAckList;
class deNetworkMessage;
class decBaseFileReader;



/**
 * \brief Connection peer class.
 */
class debnConnection : public deBaseNetworkConnection{
public:
	/** \brief State of the connection. */
	enum eConnectionState{
		/** \brief Disconnected. */
		ecsDisconnected,
		
		/** \brief Connecting. */
		ecsConnecting,
		
		/** \brief Connected. */
		ecsConnected
	};
	
	
	
private:
	deNetworkBasic *pNetBasic;
	deConnection *pConnection;
	
	debnSocket *pSocket;
	debnAddress *pRemoteAddress;
	int pConnectionState;
	int pIdentifier;
	
	eProtocols pProtocol;
	debnStateLinkManager *pStateLinks;
	debnStateLinkList *pModifiedStateLinks;
	
	// reliable messages
	debnMessageManager *pReliableMessagesSend;
	debnMessageManager *pReliableMessagesRecv;
	int pReliableNumberSend;
	int pReliableNumberRecv;
	int pReliableWindowSize;
	
	debnConnection *pPreviousConnection;
	debnConnection *pNextConnection;
	bool pIsRegistered;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	debnConnection( deNetworkBasic *netBasic, deConnection *connection );
	
	/** \brief Clean up connection. */
	virtual ~debnConnection();
	/*@}*/
	
	
	
	/** \name management */
	/*@{*/
	/** \brief Connection. */
	inline deConnection *GetConnection() const{ return pConnection; }
	
	/** \brief Socket. */
	inline debnSocket *GetSocket() const{ return pSocket; }
	
	/** \brief Remote address. */
	inline const debnAddress *GetRemoteAddress() const{ return pRemoteAddress; }
	
	/** \brief Connection identifier. */
	inline int GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set connection identifier. */
	void SetIdentifier( int identifier );
	
	/** \brief Protocol to use. */
	inline eProtocols GetProtocol() const{ return pProtocol; }
	
	
	
	/** \brief List of modified state links. */
	debnStateLinkList *GetModifiedStateLinks() const{ return pModifiedStateLinks; }
	
	/** \brief Process connection. */
	void Process( float elapsedTime );
	
	/** \brief Invalidate network state. */
	void InvalidateState( debnState *state );
	
	/** \brief Message matches connection. */
	bool Matches( const debnSocket *bnSocket, const debnAddress *address ) const;
	
	/** \brief Accept connection. */
	void AcceptConnection( debnSocket *bnSocket, debnAddress *address, eProtocols protocol );
	
	/** \brief Process connection ack message. */
	void ProcessConnectionAck( decBaseFileReader &reader );
	
	/** \brief Process connection close message. */
	void ProcessConnectionClose( decBaseFileReader &reader );
	
	/** \brief Process message. */
	void ProcessMessage( decBaseFileReader &reader );
	
	/** \brief Process reliable message. */
	void ProcessReliableMessage( decBaseFileReader &reader );
	
	/** \brief Process reliable link state. */
	void ProcessReliableLinkState( decBaseFileReader &reader );
	
	/** \brief Process reliable ack. */
	void ProcessReliableAck( decBaseFileReader &reader );
	
	/** \brief Process link up. */
	void ProcessLinkUp( decBaseFileReader &reader );
	
	/** \brief Process link down. */
	void ProcessLinkDown( decBaseFileReader &reader );
	
	/** \brief Process link update. */
	void ProcessLinkUpdate( decBaseFileReader &reader );
	
	/** \brief Connect to connection object on host. */
	virtual bool ConnectTo( const char *address );
	
	/** \brief Disconnect from remote connection if connected. */
	virtual void Disconnect();
	
	/** \brief Send message to remote connection if connected. */
	virtual void SendMessage( deNetworkMessage *message, int maxDelay );
	
	/** \brief Send reliable message to remote connection if connected. */
	virtual void SendReliableMessage( deNetworkMessage *message );
	
	/** \brief Link network state to remote network state. */
	virtual void LinkState( deNetworkMessage *message, deNetworkState *state, bool readOnly );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Previous connection. */
	inline debnConnection *GetPreviousConnection() const{ return pPreviousConnection; }
	
	/** \brief Set previous connection. */
	void SetPreviousConnection( debnConnection *connection );
	
	/** \brief Next connection. */
	inline debnConnection *GetNextConnection() const{ return pNextConnection; }
	
	/** \brief Set next connection. */
	void SetNextConnection( debnConnection *connection );
	
	/** \brief Connection is registered. */
	inline bool GetIsRegistered() const{ return pIsRegistered; }
	
	/** \brief Set if connection is registered. */
	void SetIsRegistered( bool isRegistered );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pDisconnect();
	void pUpdateStates();
	void pUpdateTimeouts( float elapsedTime );
	void pProcessQueuedMessages();
	void pProcessReliableMessage( int number, decBaseFileReader &reader );
	void pProcessLinkState( int number, decBaseFileReader &reader );
	void pAddReliableReceive( int type, int number, decBaseFileReader &reader );
	void pRemoveSendReliablesDone();
	void pSendPendingReliables();
};

#endif
