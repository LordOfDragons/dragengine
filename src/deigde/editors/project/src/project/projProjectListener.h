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

#ifndef _PROJPROJECTLISTENER_H_
#define _PROJPROJECTLISTENER_H_

#include "../project/remote/projRemoteClient.h"

#include <dragengine/deObject.h>

class projProject;
class projProfile;


/**
 * \brief Project listener.
 */
class projProjectListener : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<projProjectListener> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project listener. */
	projProjectListener();
	
	/** \brief Clean up project listener. */
	~projProjectListener() override;
	/*@}*/
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged(projProject *project);
	
	/** \brief Undos changed. */
	virtual void UndoChanged(projProject *project);
	
	/** \brief Project changed. */
	virtual void ProjectChanged(projProject *project);
	
	
	/** \brief Profile count or order changed. */
	virtual void ProfileStructureChanged(projProject *project);
	
	/** \brief Profile changed. */
	virtual void ProfileChanged(projProject *project, projProfile *profile);
	
	/** \brief Profile name changed. */
	virtual void ProfileNameChanged(projProject *project, projProfile *profile);
	
	/** \brief Active profile changed. */
	virtual void ActiveProfileChanged(projProject *project);
	
	
	/** \brief Active launch profile changed. */
	virtual void ActiveLaunchProfileChanged(projProject *project);
	
	
	/** \brief Remote client connected. */
	virtual void RemoteClientConnected(projProject *project, const projRemoteClient::Ref &client);
	/*@}*/
};

#endif
