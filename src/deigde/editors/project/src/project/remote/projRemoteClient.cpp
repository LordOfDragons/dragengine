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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "projRemoteClient.h"
#include "../projProject.h"



// Class projRemoteClient
///////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteClient::projRemoteClient(projProject &project, derlServer &server,
const derlRemoteClientConnection::Ref &connection) :
derlRemoteClient(server, connection),
pProject(project)
{
	//SetLogger();
}

projRemoteClient::~projRemoteClient()
{
}

// Management
///////////////

void projRemoteClient::OnConnectionEstablished(){
}

void projRemoteClient::OnConnectionClosed(){
}

void projRemoteClient::OnSynchronizeBegin(){
}

void projRemoteClient::OnSynchronizeUpdate(){
}

void projRemoteClient::OnSynchronizeFinished(){
}

void projRemoteClient::OnRunStatusChanged(){
}

void projRemoteClient::OnSystemProperty(const std::string &property, const std::string &value){
}
