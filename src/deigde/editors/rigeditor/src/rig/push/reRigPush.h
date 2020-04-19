/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RERIGPUSH_H_
#define _RERIGPUSH_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class reRig;
class igdeWDebugDrawerShape;
class deCollider;
class deColliderVolume;
class deDebugDrawer;
class deEngine;



/**
 * \brief Rig Push.
 */
class reRigPush : public deObject{
public:
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
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSPush;
	deColliderVolume *pCollider;
	
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
	reRigPush( deEngine *engine );
	/** \brief Creates a copy of a rig push. */
	reRigPush( const reRigPush &push );
	/** \brief Cleans up the rig push. */
	virtual ~reRigPush();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	/** \brief Sets the parent rig. */
	void SetRig( reRig *rig );
	
	/** \brief Retrieves the collider. */
	inline deColliderVolume *GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the push type. */
	inline ePushTypes GetType() const{ return pType; }
	/** \brief Sets the push type. */
	void SetType( ePushTypes type );
	/** \brief Retrieves the push position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the push position. */
	void SetPosition( const decVector &position );
	/** \brief Retrieves the push orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the push orientation. */
	void SetOrientation( const decVector &orientation );
	/** \brief Retrieves the push impuls. */
	inline float GetImpuls() const{ return pImpuls; }
	/** \brief Sets the push impuls. */
	void SetImpuls( float impuls );
	/** \brief Retrieves the number of rays to shoot. */
	inline int GetRayCount() const{ return pRayCount; }
	/** \brief Sets the number of rays to shoot. */
	void SetRayCount( int rayCount );
	/** \brief Retrieves the cone angle. */
	inline float GetConeAngle() const{ return pConeAngle; }
	/** \brief Sets the cone angle. */
	void SetConeAngle( float angle );
	
	/** \brief Determines if the push is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the push is selected. */
	void SetSelected( bool selected );
	/** \brief Determines if the push is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the push is the active one. */
	void SetActive( bool active );
	
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
