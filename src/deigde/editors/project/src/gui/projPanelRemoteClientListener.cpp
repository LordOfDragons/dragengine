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

#include "projPanelRemoteClient.h"
#include "projPanelRemoteClientListener.h"
#include "projPanelTestRun.h"
#include "../project/remote/projRemoteClient.h"


// Class projPanelRemoteClientListener
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelRemoteClientListener::projPanelRemoteClientListener(projPanelRemoteClient &panel) :
pPanel(panel){
}


// Notifications
//////////////////

void projPanelRemoteClientListener::ClientChanged(projRemoteClient *client){
}

void projPanelRemoteClientListener::ClientDisconnected(projRemoteClient *client){
	const projRemoteClient::Ref ref(client->GetRefInServer());
	if(ref){
		pPanel.GetPanelTestRun().RemoveRemoteClient(ref);
	}
}

void projPanelRemoteClientListener::LaunchProfilesChanged(projRemoteClient *client){
	pPanel.UpdateLaunchProfiles();
}

void projPanelRemoteClientListener::ActiveLaunchProfileChanged(projRemoteClient *client){
	pPanel.SelectLaunchProfile(client->GetActiveLaunchProfile());
	if(pPanel.GetSelectedLaunchProfile().IsEmpty()){
		pPanel.SelectLaunchProfile(client->GetDefaultLaunchProfile());
	}
}

void projPanelRemoteClientListener::DefaultLaunchProfileChanged(projRemoteClient *client){
	if(client->GetActiveLaunchProfile().IsEmpty()){
		pPanel.SelectLaunchProfile(client->GetDefaultLaunchProfile());
	}
}
