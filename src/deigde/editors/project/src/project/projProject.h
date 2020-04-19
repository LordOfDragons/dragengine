/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJDISTRIBUTOR_H_
#define _PROJDISTRIBUTOR_H_

#include "profile/projProfileList.h"

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
	projProfile *pActiveProfile;
	
	decString pActiveLaunchProfile;
	
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project. */
	projProject( igdeEnvironment *environment );
	
	/** \brief Clean up project. */
	virtual ~projProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script directory. */
	inline const decString &GetScriptDirectory() const{ return pScriptDirectory; }
	
	/** \brief Set script directory. */
	void SetScriptDirectory( const char *directory );
	
	/** \brief Game object. */
	inline const decString &GetGameObject() const{ return pGameObject; }
	
	/** \brief Set game object. */
	void SetGameObject( const char *gameObject );
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set config path. */
	void SetPathConfig( const char *path );
	
	/** \brief Capture path. */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	
	/** \brief Set capture path. */
	void SetPathCapture( const char *path );
	/*@}*/
	
	
	
	/** \name Profiles */
	/*@{*/
	/** \brief Profiles. */
	const projProfileList &GetProfiles() const{ return pProfiles; }
	
	/** \brief Add profile. */
	void AddProfile( projProfile *profile );
	
	/** \brief Remove profile. */
	void RemoveProfile( projProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAllProfiles();
	
	
	
	/** \brief Active profile. */
	inline projProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile. */
	void SetActiveProfile( projProfile *profile );
	
	
	
	/** \brief Notify listeners profile structure changed. */
	void NotifyProfileStructureChanged();
	
	/** \brief Notify listeners profile changed. */
	void NotifyProfileChanged( projProfile *profile );
	
	/** \brief Notify listeners profile name changed. */
	void NotifyProfileNameChanged( projProfile *profile );
	
	/** \brief Notify listeners active profile changed. */
	void NotifyActiveProfileChanged();
	/*@}*/
	
	
	
	/** \name Test-runner */
	/*@{*/
	/** \brief Active launch profile. */
	inline const decString &GetActiveLaunchProfile() const{ return pActiveLaunchProfile; }
	
	/** \brief Set active launch profile. */
	void SetActiveLaunchProfile( const char *profile );
	
	
	
	/** \brief Notify listeners active launcher profile changed. */
	void NotifyActiveLaunchProfileChanged();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** \brief Add listener. */
	void AddListener( projProjectListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( projProjectListener *listener );
	
	
	
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
