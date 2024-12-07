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

#ifndef _PROJREMOTESERVER_H_
#define _PROJREMOTESERVER_H_

#include <deremotelauncher/derlServer.h>
#include <dragengine/logger/deLogger.h>

class projProject;
class igdeEnvironment;
class projRemoteServerThread;


/**
 * \brief Remote connection server.
 */
class projRemoteServer : public derlServer{
public:
	typedef std::shared_ptr<projRemoteServer> Ref;
	
	
private:
	projProject &pProject;
	projRemoteServerThread *pThreadUpdate;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create remote server. */
	projRemoteServer(projProject &project, igdeEnvironment &environment);
	
	/** \brief Clean up remote server. */
	~projRemoteServer() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Project. */
	inline projProject &GetProject() const{ return pProject; }
	
	/** \brief Create client for connection. */
	derlRemoteClient::Ref CreateClient(const derlRemoteClientConnection::Ref &connection) override;
	
	/** \brief Listen for client connections. */
	void ListenForClientConnections(const decString &address);
	
	/** \brief Stop listening for client connections. */
	void StopListenClientConnections();
	/*@}*/
	
	
	
private:
	void pExitThread();
};

#endif
