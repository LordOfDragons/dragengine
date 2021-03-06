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

#ifndef _DENETWORKBASIC_H_
#define _DENETWORKBASIC_H_

#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/network/deNetworkMessageReference.h>
#include <dragengine/systems/modules/network/deBaseNetworkModule.h>

class debnSocket;
class debnServer;
class debnAddress;
class debnConnection;
class deNetworkMessage;



/*
 Protocol definition. Will be moved to a separate file in the game engine
 once finished.
 
 Connection Request:
   [ 0 ] [ protocols ]
   
   protocols:  // list of protocols supported by client
      [ count:uint16 ] [ protocol:uint16 ]{ 1..n }
   
 Connection Ack:
   [ 1 ] [ resultCode:uint8 ]
   
   resultCode:
      0: Connection Accepted
      1: Connection Rejected
      2: Connection Rejected because no common protocols
   
   if connection is accepted the message also contains:
         [ protocol:uint16 ]
   
      protocol:
         The chosen protocol
 
 Close Connection:
   [ 2 ]
 
 Message:
   [ 3 ] [ data ]
 
 Reliable message:
   [ 4 ] [ number:uint16 ] [ data ]
 
 Link state:
   [ 5 ] [ number:uint16 ] [ link_id:uint16 ] [ flags ] [ message ] [ values ]
   
   flags:
      0x1: create read only state
   
   message:
      [ length:uint16 ] [ message_bytes:uint8 ]{ 1..n }
      
   values:
      [ value_count:uint16 ] ( [ value_type:uint8 ] [ value_data:* ] ){ 1..n }
 
 Reliable ack:
   [ 6 ] [ number:uint16 ] [ code:uint8 ]
   
   code:
      [ 0 ] received successfully
      [ 1 ] failed
 
 Link up:
   [ 7 ] [ link_id:uint16 ]
 
 Link down:
   [ 8 ] [ link_id:uint16 ]
 
 Link update: 
   [ 9 ] [ link_count:uint8 ] [ link ]{ 1..link_count }
   
   link:
      [ link_id:uint16 ] [ value_count:uint8 ] [ value ]{ 1..value_count }
   
   value:
      [ value_index:uint16 ] [ value_data:* ]
*/

/*

NOTE Prediction Support Draft

Right now state changes are triggered if the value changes more than the precision value
specified by the user. This can be improved by adding an automatic prediction support in
the network module. A possible way is to track the first and optionally second order
derivative of the value, or put differently: velocity and acceleration. Most values change
in a linear or potentially accelerated/decelerated fashion (hence the speed example).
This can be tracked by the network module and send along the state change to tell the
remote side how to alter the value if no additional state change is send in the near future.

Using this feature the "Link update" message would have to be changed like this:
   [ 9 ] [ link_count:uint8 ] [ link ]{ 1..link_count }
   link:
      [ link_id:uint16 ] [ value_count:uint8 ] [ value ]{ 1..value_count }
   value:
      [ value_index:uint16 ] [ value_data:* ] [ predictLinear:* ] [ predictAccelerated:* ]

Where predictLinear and predictAccelerated value is of the same format as value_data.

Best way to enable this is to use a protocol version V2. Adding some feature to enable
this in the existing protocol could be a problem. V1 needs to be backwards compatible.
*/

enum eCommandCodes{
	eccConnectionRequest,
	eccConnectionAck,
	eccConnectionClose,
	eccMessage,
	eccReliableMessage,
	eccReliableLinkState,
	eccReliableAck,
	eccLinkUp,
	eccLinkDown,
	eccLinkUpdate,
};

enum eConnectionAck{
	ecaAccepted,
	ecaRejected,
	ecaNoCommonProtocol
};

enum eReliableAck{
	eraSuccess,
	eraFailed
};

enum eValueTypes{
	evtSInt8, // integer: signed char ( 8-bit )
	evtUInt8, // integer: unsigned char ( 8-bit )
	evtSInt16, // integer: signed short ( 16-bit )
	evtUInt16, // integer: unsigned short ( 16-bit )
	evtSInt32, // integer: signed long ( 32-bit )
	evtUInt32, // integer: unsigned long ( 32-bit )
	evtSInt64, // integer: signed long ( 64-bit )
	evtUInt64, // integer: unsigned long ( 64-bit )
	evtFloat16, // float: half float ( 16-bit )
	evtFloat32, // float: float ( 32-bit )
	evtFloat64, // float: float ( 32-bit )
	evtString, // string zero terminated
	evtData, // data: length unsigned 8-bit
	evtPoint2S8, // point2: signed 8-bit per component
	evtPoint2U8, // point2: unsigned 8-bit per component
	evtPoint2S16, // point2: signed 16-bit per component
	evtPoint2U16, // point2: unsigned 16-bit per component
	evtPoint2S32, // point2: signed 32-bit per component
	evtPoint2U32, // point2: unsigned 32-bit per component
	evtPoint2S64, // point2: signed 64-bit per component
	evtPoint2U64, // point2: unsigned 64-bit per component
	evtPoint3S8, // point3: signed 8-bit per component
	evtPoint3U8, // point3: unsigned 8-bit per component
	evtPoint3S16, // point3: signed 16-bit per component
	evtPoint3U16, // point3: unsigned 16-bit per component
	evtPoint3S32, // point3: signed 32-bit per component
	evtPoint3U32, // point3: unsigned 32-bit per component
	evtPoint3S64, // point3: signed 64-bit per component
	evtPoint3U64, // point3: unsigned 64-bit per component
	evtVector2F16, // vector2: 16-bit per component
	evtVector2F32, // vector2: 32-bit per component
	evtVector2F64, // vector2: 64-bit per component
	evtVector3F16, // vector3: 16-bit per component
	evtVector3F32, // vector3: 32-bit per component
	evtVector3F64, // vector3: 64-bit per component
	evtQuaternionF16, // quaternion: 16-bit per component
	evtQuaternionF32, // quaternion: 32-bit per component
	evtQuaternionF64, // quaternion: 64-bit per component
};

enum eProtocols{
	epDENetworkProtocol // Drag[en]gine Network Protocol: Version 1
};



/**
 * @brief Basic Network Module.
 * Provides basic network communication.
 */
class deNetworkBasic : public deBaseNetworkModule{
private:
	// objects to monitor
	debnConnection *pHeadConnection;
	debnConnection *pTailConnection;
	debnServer *pHeadServer;
	debnServer *pTailServer;
	debnSocket *pHeadSocket;
	debnSocket *pTailSocket;
	
	// sending and receiving
	deNetworkMessage *pDatagram;
	debnAddress *pAddressReceive;
	deNetworkMessageReference pSharedSendDatagram;
	decBaseFileWriterReference pSharedSendDatagramWriter;
	
	//debnMessageQueue *pMessagesSend;
	//debnMessageQueue *pMessagesReceive;
	
	//deNetworkMessage *pMessage;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new basic network module. */
	deNetworkBasic( deLoadableModule &loadableModule );
	/** Cleans up the basic network module. */
	virtual ~deNetworkBasic();
	/*@}*/
	
	/** @name Module Management */
	/*@{*/
	/**
	 * Called to init the module. Returns true on success or false otherwise.
	 */
	virtual bool Init();
	/**
	 * Called to cleanup the module. All resources have to be freed and running threads
	 * stopped or killed if needed.
	 */
	virtual void CleanUp();
	/** Process network. */
	virtual void ProcessNetwork();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	inline deNetworkMessage *GetSharedSendDatagram() const{ return pSharedSendDatagram; }
	inline decBaseFileWriter &GetSharedSendDatagramWriter() const{ return pSharedSendDatagramWriter; }
	
	/** Register a connection. */
	void RegisterConnection( debnConnection *connection );
	/** Unregister a connection if existing. */
	void UnregisterConnection( debnConnection *connection );
	/** Register a server. */
	void RegisterServer( debnServer *server );
	/** Unregister a server if existing. */
	void UnregisterServer( debnServer *server );
	/** Register a socket. */
	void RegisterSocket( debnSocket *bnSocket );
	/** Unregister a socket if existing. */
	void UnregisterSocket( debnSocket *bnSocket );
	
	/** \brief Find public addresses. */
	void FindPublicAddresses( decStringList &list );
	
	/** Create a peer for the given world. */
	virtual deBaseNetworkWorld *CreateWorld( deWorld *world );
	/** Create a peer for the given server. */
	virtual deBaseNetworkServer *CreateServer( deServer *server );
	/** Create a peer for the given connection. */
	virtual deBaseNetworkConnection *CreateConnection( deConnection *connection );
	/** Create a peer for the given state. */
	virtual deBaseNetworkState *CreateState( deNetworkState *state );
	/*@}*/
	
private:
	debnConnection *pFindConnection( const debnSocket *bnSocket, const debnAddress *address ) const;
	debnServer *pFindServer( const debnSocket *bnSocket ) const;
	
	void pReceiveDatagrams();
	void pProcessConnections( float elapsedTime );
};

// end of include only once
#endif
