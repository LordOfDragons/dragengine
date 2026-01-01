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

#ifndef _PROJPANELREMOTECLIENTLISTENER_H_
#define _PROJPANELREMOTECLIENTLISTENER_H_

#include "../project/remote/projRemoteClientListener.h"

class projPanelRemoteClient;


/**
 * \brief Panel test-run listener.
 */
class projPanelRemoteClientListener : public projRemoteClientListener{
private:
	projPanelRemoteClient &pPanel;
	
	
public:
	typedef deTObjectReference<projPanelRemoteClientListener> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	projPanelRemoteClientListener(projPanelRemoteClient &panel);
	/*@}*/
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameters changed. */
	void ClientChanged(projRemoteClient *client) override;
	
	/** \brief Client disconnected. */
	void ClientDisconnected(projRemoteClient *client) override;
	
	/** \brief Launch profiles changed. */
	void LaunchProfilesChanged(projRemoteClient *client) override;
	
	/** \brief Active launch profile changed. */
	void ActiveLaunchProfileChanged(projRemoteClient *client) override;
	
	/** \brief Default launch profile changed. */
	void DefaultLaunchProfileChanged(projRemoteClient *client) override;
	/*@}*/
};

#endif
