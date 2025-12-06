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

