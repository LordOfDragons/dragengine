/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
