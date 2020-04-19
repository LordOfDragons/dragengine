/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAINAVBLOCKER_H_
#define _DEDAINAVBLOCKER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/systems/modules/ai/deBaseAINavigationBlocker.h>

class deDEAIModule;
class dedaiLayer;
class dedaiWorld;

class deDebugDrawer;
class deDebugDrawerShape;
class deNavigationBlocker;



/**
 * \brief Navigation blocker peer.
 */
class dedaiNavBlocker : public deBaseAINavigationBlocker{
private:
	deDEAIModule &pDEAI;
	const deNavigationBlocker &pNavBlocker;
	
	dedaiWorld *pParentWorld;
	
	decDMatrix pMatrix;
	decDMatrix pInvMatrix;
	decDVector pMinExtends;
	decDVector pMaxExtends;
	bool pDirtyMatrix;
	bool pDirtyExtends;
	bool pDirtyShape;
	
	dedaiLayer *pLayer;
	
	decConvexVolumeList pConvexVolumeList;
	
	deDebugDrawer *pDebugDrawer;
	deDebugDrawerShape *pDDSBlocker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates peer. */
	dedaiNavBlocker( deDEAIModule &deai, const deNavigationBlocker &blocker );
	
	/** \brief Clean up peer. */
	virtual ~dedaiNavBlocker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	/** \brief Navigation space resource. */
	inline const deNavigationBlocker &GetNavigationBlocker() const{ return pNavBlocker; }
	
	
	
	/** \brief Parent world or \em NULL if not set. */
	inline dedaiWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL if not set. */
	void SetParentWorld( dedaiWorld *world );
	
	
	
	/** \brief Matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Inverse matrix. */
	const decDMatrix &GetInverseMatrix();
	
	/** \brief Minimum extends. */
	const decDVector &GetMinimumExtends();
	
	/** \brief Maximum extends. */
	const decDVector &GetMaximumExtends();
	
	
	
	/** \brief Convex volume list used to block navigation spaces. */
	inline decConvexVolumeList &GetConvexVolumeList(){ return pConvexVolumeList; }
	inline const decConvexVolumeList &GetConvexVolumeList() const{ return pConvexVolumeList; }
	
	/** \brief Prepare navigation blocker if not ready yet. */
	void Prepare();
	
	/** \brief Debug drawer shape for the blocker or NULL if not existing. */
	inline deDebugDrawerShape *GetDDSBlocker() const{ return pDDSBlocker; }
	
	/**
	 * \brief Update debug drawer shape for the space.
	 * 
	 * If space visualization is enabled in the developer mode ensures a debug
	 * drawer shape exists and is added to the appropriate parent world debug
	 * drawer object. If visualization is disabled ensures the the debug drawer
	 * shape is deleted and removed from the appropriate parent world debug
	 * drawer object. Updating the shape is handled by UpdateDDSBlockerShape.
	 */
	void UpdateDDSBlocker();
	
	/** \brief Update debug drawer shape from the navigation blocker if existing. */
	void UpdateDDSBlockerShape();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Layer changed. */
	virtual void LayerChanged();
	
	/** \brief Space type changed. */
	virtual void SpaceTypeChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Shape changed. */
	virtual void ShapeChanged();
	
	/** \brief Priority changed. */
	virtual void BlockingPriorityChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateMatrices();
	void pUpdateExtends();
	void pUpdateBlocker();
	void pUpdateConvexVolumeList();
	void pInvalidateLayerBlocking();
};

#endif
