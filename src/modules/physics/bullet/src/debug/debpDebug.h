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
	
	debpDebugInformation::Ref pDIColliderPrepareDetection;
	debpDebugInformation::Ref pDIColliderDetectCustomCollision;
	debpDebugInformation::Ref pDIColliderUpdateFromBody;
	debpDebugInformation::Ref pDIColliderFinishDetection;
	debpDebugInformation::Ref pDIColliderCollisionTests;
	debpDebugInformation::Ref pDIColliderUpdateOctree;
	debpDebugInformation::Ref pDITouchSensorApplyChanges;
	debpDebugInformation::Ref pDIWorldStepSimulation;
	debpDebugInformation::Ref pDIWorldUpdateOctrees;
	debpDebugInformation::Ref pDIWorldCheckDynamicCollisions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug support. */
	debpDebug(dePhysicsBullet &bullet);
	
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
	void SetEnabled(bool enabled);
	
	/** \brief Debug information. */
	inline const debpDebugInformation::Ref &GetDIColliderPrepareDetection() const{ return pDIColliderPrepareDetection; }
	inline const debpDebugInformation::Ref &GetDIColliderDetectCustomCollision() const{ return pDIColliderDetectCustomCollision; }
	inline const debpDebugInformation::Ref &GetDIColliderUpdateFromBody() const{ return pDIColliderUpdateFromBody; }
	inline const debpDebugInformation::Ref &GetDIColliderFinishDetection() const{ return pDIColliderFinishDetection; }
	inline const debpDebugInformation::Ref &GetDIColliderCollisionTests() const{ return pDIColliderCollisionTests; }
	inline const debpDebugInformation::Ref &GetDIColliderUpdateOctree() const{ return pDIColliderUpdateOctree; }
	inline const debpDebugInformation::Ref &GetDITouchSensorApplyChanges() const{ return pDITouchSensorApplyChanges; }
	inline const debpDebugInformation::Ref &GetDIWorldStepSimulation() const{ return pDIWorldStepSimulation; }
	inline const debpDebugInformation::Ref &GetDIWorldUpdateOctrees() const{ return pDIWorldUpdateOctrees; }
	inline const debpDebugInformation::Ref &GetDIWorldCheckDynamicCollisions() const{ return pDIWorldCheckDynamicCollisions; }
	
	
	
	/** \brief Begin process physics. */
	void BeginProcessPhysics(debpWorld *world);
	
	/** \brief End process physics. */
	void EndProcessPhysics(debpWorld *world);
	/*@}*/
	
	
	
private:
	void pUpdateDebugInformation();
	void pUpdateDebugInformation(debpDebugInformation &debugInfo);
	void pLogDebugInformation();
};

#endif
