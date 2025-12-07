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

#ifndef _MEPATHFINDTEST_H_
#define _MEPATHFINDTEST_H_

#include "types/mePathFindTestTypeList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>
#include <dragengine/resources/navigation/navigator/deNavigatorPath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>

class meObject;
class meWorld;

class igdeWDebugDrawerShape;

class deEngine;



/**
 * \brief Path Find Test.
 */
class mePathFindTest : public deObject{
private:
	deEngine *pEngine;
	meWorld *pWorld;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSPath;
	deNavigator::Ref pEngNavigator;
	
	decDVector pStartPosition;
	decDVector pGoalPosition;
	decVector pStartOrientation;
	decVector pGoalOrientation;
	int pLayer;
	float pBlockingCost;
	deNavigationSpace::eSpaceTypes pSpaceType;
	deNavigatorPath pPath;
	bool pShowPath;
	bool pDirtyPath;
	
	mePathFindTestTypeList pTypeList;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<mePathFindTest> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new path find test. */
	mePathFindTest(deEngine *engine);
	/** \brief Cleans up the path find test. */
	virtual ~mePathFindTest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** \brief Retrieves the engine navigator. */
	inline const deNavigator::Ref &GetEngineNavigator() const{ return pEngNavigator; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld(meWorld *world);
	
	/** \brief Retrieves the start position. */
	inline const decDVector &GetStartPosition() const{ return pStartPosition; }
	/** \brief Sets the start position. */
	void SetStartPosition(const decDVector &position);
	/** \brief Retrieves the goal position. */
	inline const decDVector &GetGoalPosition() const{ return pGoalPosition; }
	/** \brief Sets the goal position. */
	void SetGoalPosition(const decDVector &position);
	/** \brief Retrieves the start orientation. */
	inline const decVector &GetStartOrientation() const{ return pStartOrientation; }
	/** \brief Sets the start orientation. */
	void SetStartOrientation(const decVector &orientation);
	/** \brief Retrieves the goal orientation. */
	inline const decVector &GetGoalOrientation() const{ return pGoalOrientation; }
	/** \brief Sets the goal orientation. */
	void SetGoalOrientation(const decVector &orientation);
	/** \brief Retrieves the layer number. */
	inline int GetLayer() const{ return pLayer; }
	/** \brief Sets the layer number. */
	void SetLayer(int layer);
	/** \brief Retrieves the space type to navigate. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	/** \brief Sets the space type to navigate. */
	void SetSpaceType(deNavigationSpace::eSpaceTypes spaceType);
	
	/** \brief Found path. */
	inline const deNavigatorPath &GetPath() const{ return pPath; }
	
	/** \brief Set found path. */
	void SetPath(const deNavigatorPath &path);
	
	/** \brief Blocking cost. */
	inline float GetBlockingCost() const{ return pBlockingCost; }
	
	/** \brief Set blocking cost. */
	void SetBlockingCost(float cost);
	
	/** \brief Determines if the path is shown. */
	inline bool GetShowPath() const{ return pShowPath; }
	/** \brief Sets if the path is shown. */
	void SetShowPath(bool showPath);
	
	/** \brief Retrieves the type list. */
	inline mePathFindTestTypeList &GetTypeList(){ return pTypeList; }
	inline const mePathFindTestTypeList &GetTypeList() const{ return pTypeList; }
	/** \brief Notify world that the types changed. */
	void NotifyTypesChanged();
	
	/** \brief Invalidate path. Update during the next world update cycle. */
	void Invalidate();
	
	/** \brief Update path if dirty. */
	void Update();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateDDPath();
	void pUpdateTypes();
};

#endif
