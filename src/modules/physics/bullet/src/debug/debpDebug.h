/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPDEBUG_H_
#define _DEBPDEBUG_H_

#include "debpDebugInformationList.h"

class dePhysicsBullet;
class debpWorld;



/**
 * \brief Debug support.
 */
class debpDebug{
private:
	dePhysicsBullet &pBullet;
	
	bool pEnabled;
	
	debpDebugInformationList pDebugInfoList;
	
	debpDebugInformation *pDIColliderPrepareDetection;
	debpDebugInformation *pDIColliderDetectCustomCollision;
	debpDebugInformation *pDIColliderUpdateFromBody;
	debpDebugInformation *pDIColliderFinishDetection;
	debpDebugInformation *pDIColliderCollisionTests;
	debpDebugInformation *pDIColliderUpdateOctree;
	debpDebugInformation *pDITouchSensorApplyChanges;
	debpDebugInformation *pDIWorldStepSimulation;
	debpDebugInformation *pDIWorldUpdateOctrees;
	debpDebugInformation *pDIWorldCheckDynamicCollisions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug support. */
	debpDebug( dePhysicsBullet &bullet );
	
	/** \brief Clean up debug support. */
	~debpDebug();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** \brief Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Debug information. */
	inline debpDebugInformation *GetDIColliderPrepareDetection() const{ return pDIColliderPrepareDetection; }
	inline debpDebugInformation *GetDIColliderDetectCustomCollision() const{ return pDIColliderDetectCustomCollision; }
	inline debpDebugInformation *GetDIColliderUpdateFromBody() const{ return pDIColliderUpdateFromBody; }
	inline debpDebugInformation *GetDIColliderFinishDetection() const{ return pDIColliderFinishDetection; }
	inline debpDebugInformation *GetDIColliderCollisionTests() const{ return pDIColliderCollisionTests; }
	inline debpDebugInformation *GetDIColliderUpdateOctree() const{ return pDIColliderUpdateOctree; }
	inline debpDebugInformation *GetDITouchSensorApplyChanges() const{ return pDITouchSensorApplyChanges; }
	inline debpDebugInformation *GetDIWorldStepSimulation() const{ return pDIWorldStepSimulation; }
	inline debpDebugInformation *GetDIWorldUpdateOctrees() const{ return pDIWorldUpdateOctrees; }
	inline debpDebugInformation *GetDIWorldCheckDynamicCollisions() const{ return pDIWorldCheckDynamicCollisions; }
	
	
	
	/** \brief Begin process physics. */
	void BeginProcessPhysics( debpWorld *world );
	
	/** \brief End process physics. */
	void EndProcessPhysics( debpWorld *world );
	/*@}*/
	
	
	
private:
	void pUpdateDebugInformation();
	void pUpdateDebugInformation( debpDebugInformation &debugInfo );
	void pLogDebugInformation();
};

#endif
