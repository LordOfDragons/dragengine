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

#include "projRemoteServerThread.h"
#include "projRemoteServer.h"
#include "../projProject.h"
#include "../../project.h"

#include <dragengine/common/utils/decTimer.h>
#include <dragengine/threading/deMutexGuard.h>


// Class projRemoteServerThread
/////////////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteServerThread::projRemoteServerThread(projRemoteServer &server) :
pServer(server),
pExit(false){
}

// Management
///////////////

void projRemoteServerThread::Run(){
	pServer.GetProject().GetLogger()->LogInfo(LOGSOURCE, "Start Remote Server Thread");
	
	decTimer timer;
	while(true){
		{
		const deMutexGuard guard(pMutexExit);
		if(pExit){
			break;
		}
		}
		
		pServer.Update(timer.GetElapsedTime());
	}
	
	pServer.GetProject().GetLogger()->LogInfo(LOGSOURCE, "Exiting Remote Server Thread");
	
	pServer.StopListening();
	pServer.WaitAllClientsDisconnected();
	
	pServer.GetProject().GetLogger()->LogInfo(LOGSOURCE, "Remote Server Thread Exited");
}

void projRemoteServerThread::ExitThread(){
	const deMutexGuard guard(pMutexExit);
	pExit = true;
}
