/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRDPVIVETRACKER_H_
#define _DEOXRDPVIVETRACKER_H_

#include "deoxrDeviceProfile.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/utils/decTimer.h>


/**
 * Vive Tracker profile.
 */
class deoxrDPHtcViveTracker : public deoxrDeviceProfile{
private:
	class Tracker : public deObject{
	public:
		typedef deTObjectReference<Tracker> Ref;
		
		const deoxrPath path;
		deoxrPath pathRole;
		deoxrDevice::Ref device;
		deoxrAction::Ref action;
		int number;
		
		Tracker( const deoxrPath &path, int number );
		virtual ~Tracker();
	};
	
	class RoleAction : public deObject{
	public:
		typedef deTObjectReference<RoleAction> Ref;
		
		const deoxrPath path;
		const deoxrAction::Ref action;
		
		RoleAction( const deoxrPath &path, const deoxrAction::Ref &action );
		virtual ~RoleAction();
	};
	
	decObjectOrderedSet pTrackers;
	decObjectOrderedSet pRoleActions;
	decTimer pTimerCheckAttached;
	float pTimeoutCheckAttached;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPHtcViveTracker( deoxrInstance &instance );
	
protected:
	/** Clean up device profile. */
	virtual ~deoxrDPHtcViveTracker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** On session state changed. */
	virtual void OnSessionStateChanged();
	
	/** On actions synced. */
	virtual void OnActionsSynced();
	
	/** Check attached. */
	virtual void CheckAttached();
	
	/** Create actions for action set. */
	virtual void CreateActions( deoxrActionSet &actionSet );
	
	/** Suggest bindings. */
	virtual void SuggestBindings();
	
	/** Clear actions. */
	virtual void ClearActions();
	/*@}*/
	
	
	
private:
	void pAddRoleAction( deoxrActionSet &actionSet, const char *roleType, const char *localizedNameSuffix );
	const RoleAction *pGetRoleActionWith( const deoxrPath &path ) const;
	
	Tracker *pGetTrackerWith( deoxrDevice *device ) const;
	Tracker *pGetTrackerWith( XrPath path ) const;
	Tracker *pGetTrackerWithRole( XrPath path ) const;
	decString pSerialFromPath( const deoxrPath &path ) const;
	decString pNameForTracker( const Tracker &tracker ) const;
	decString pLocalizedNameForTracker( const Tracker &tracker ) const;
	
	void pRemoveAllDevices();
	void pLoadTrackerDatabase();
	void pSaveTrackerDatabase();
	
	void pAddDevice( Tracker &tracker );
	void pTrySuggestBindings( int restrictCount );
};

#endif

