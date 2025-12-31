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

#ifndef _RERIGPUSH_H_
#define _RERIGPUSH_H_

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class reRig;
class deCollider;
class deEngine;



/**
 * \brief Rig Push.
 */
class reRigPush : public deObject{
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reRigPush> Ref;
	
	typedef decTObjectOrderedSet<reRigPush> List;
	
	
	/** \brief Push type. */
	enum ePushTypes{
		/** \brief Apply a simple push. */
		eptSimple,
		/** \brief Apply an explosition type push. */
		eptExplosion,
		/** \brief Number of push types. */
		EPT_COUNT
	};
	
private:
	deEngine *pEngine;
	
	reRig *pRig;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape::Ref pDDSPush;
	deColliderVolume::Ref pCollider;
	
	ePushTypes pType;
	decVector pPosition;
	decVector pOrientation;
	float pImpuls;
	int pRayCount;
	float pConeAngle;
	
	bool pSelected;
	bool pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new rig push. */
	reRigPush(deEngine *engine);
	/** \brief Creates a copy of a rig push. */
	reRigPush(const reRigPush &push);
	/** \brief Cleans up the rig push. */
protected:
	virtual ~reRigPush();
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	/** \brief Sets the parent rig. */
	void SetRig(reRig *rig);
	
	/** \brief Retrieves the collider. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the push type. */
	inline ePushTypes GetType() const{ return pType; }
	/** \brief Sets the push type. */
	void SetType(ePushTypes type);
	/** \brief Retrieves the push position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the push position. */
	void SetPosition(const decVector &position);
	/** \brief Retrieves the push orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the push orientation. */
	void SetOrientation(const decVector &orientation);
	/** \brief Retrieves the push impuls. */
	inline float GetImpuls() const{ return pImpuls; }
	/** \brief Sets the push impuls. */
	void SetImpuls(float impuls);
	/** \brief Retrieves the number of rays to shoot. */
	inline int GetRayCount() const{ return pRayCount; }
	/** \brief Sets the number of rays to shoot. */
	void SetRayCount(int rayCount);
	/** \brief Retrieves the cone angle. */
	inline float GetConeAngle() const{ return pConeAngle; }
	/** \brief Sets the cone angle. */
	void SetConeAngle(float angle);
	
	/** \brief Determines if the push is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the push is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the push is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the push is the active one. */
	void SetActive(bool active);
	
	/** \brief Show state changed. */
	void ShowStateChanged();
	
	/** \brief Determines if this push is visible in the current state. */
	bool IsVisible() const;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateDDPush();
	void pUpdateDDPushGeometry();
	void pUpdateColliderShape();
};

#endif
