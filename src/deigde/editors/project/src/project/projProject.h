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

#ifndef _PROJDISTRIBUTOR_H_
#define _PROJDISTRIBUTOR_H_

#include "profile/projProfileList.h"
#include "remote/projRemoteClient.h"
#include "remote/projRemoteServer.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>

class projProjectListener;

class igdeEnvironment;



/**
 * \brief Project.
 */
class projProject : public igdeEditableEntity{
private:
	decString pScriptDirectory;
	decString pGameObject;
	decString pPathConfig;
	decString pPathCapture;
	
	projProfileList pProfiles;
	projProfile::Ref pActiveProfile;
	
	decString pActiveLaunchProfile;
	
	decObjectSet pListeners;
	
	projRemoteServer::Ref pRemoteServer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project. */
	projProject(igdeEnvironment *environment);
	
	/** \brief Clean up project. */
	virtual ~projProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script directory. */
	inline const decString &GetScriptDirectory() const{ return pScriptDirectory; }
	
	/** \brief Set script directory. */
	void SetScriptDirectory(const char *directory);
	
	/** \brief Game object. */
	inline const decString &GetGameObject() const{ return pGameObject; }
	
	/** \brief Set game object. */
	void SetGameObject(const char *gameObject);
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set config path. */
	void SetPathConfig(const char *path);
	
	/** \brief Capture path. */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	
	/** \brief Set capture path. */
	void SetPathCapture(const char *path);
	
	/** \brief Remote server. */
	inline const projRemoteServer::Ref &GetRemoteServer() const{ return pRemoteServer; }
	/*@}*/
	
	
	
	/** \name Profiles */
	/*@{*/
	/** \brief Profiles. */
	const projProfileList &GetProfiles() const{ return pProfiles; }
	
	/** \brief Add profile. */
	void AddProfile(projProfile::Ref profile);
	
	/** \brief Remove profile. */
	void RemoveProfile(projProfile::Ref profile);
	
	/** \brief Remove all profiles. */
	void RemoveAllProfiles();
	
	
	
	/** \brief Active profile. */
	inline projProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile. */
	void SetActiveProfile(projProfile::Ref profile);
	
	
	
	/** \brief Notify listeners profile structure changed. */
	void NotifyProfileStructureChanged();
	
	/** \brief Notify listeners profile changed. */
	void NotifyProfileChanged(projProfile::Ref profile);
	
	/** \brief Notify listeners profile name changed. */
	void NotifyProfileNameChanged(projProfile::Ref profile);
	
	/** \brief Notify listeners active profile changed. */
	void NotifyActiveProfileChanged();
	
	/** \brief Notify listeners remote client connected. */
	void NotifyRemoteClientConnected(const projRemoteClient::Ref &client);
	/*@}*/
	
	
	
	/** \name Test-runner */
	/*@{*/
	/** \brief Active launch profile. */
	inline const decString &GetActiveLaunchProfile() const{ return pActiveLaunchProfile; }
	
	/** \brief Set active launch profile. */
	void SetActiveLaunchProfile(const char *profile);
	
	
	
	/** \brief Notify listeners active launcher profile changed. */
	void NotifyActiveLaunchProfileChanged();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** \brief Add listener. */
	void AddListener(projProjectListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(projProjectListener *listener);
	
	
	
	/** \brief Notify listeners the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify listeners project changed. */
	void NotifyProjectChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
