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

#ifndef _DEDAINAVBLOCKER_H_
#define _DEDAINAVBLOCKER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/systems/modules/ai/deBaseAINavigationBlocker.h>

class deDEAIModule;
class dedaiLayer;
class dedaiWorld;

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
	
	deDebugDrawer::Ref pDebugDrawer;
	deDebugDrawerShape *pDDSBlocker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates peer. */
	dedaiNavBlocker(deDEAIModule &deai, const deNavigationBlocker &blocker);
	
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
	void SetParentWorld(dedaiWorld *world);
	
	
	
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
