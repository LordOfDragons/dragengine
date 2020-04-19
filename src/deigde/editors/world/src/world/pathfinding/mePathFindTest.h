/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEPATHFINDTEST_H_
#define _MEPATHFINDTEST_H_

#include "types/mePathFindTestTypeList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>
#include <dragengine/resources/navigation/navigator/deNavigatorPath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>

class meObject;
class meWorld;

class igdeWDebugDrawerShape;

class deNavigator;
class deEngine;
class deDebugDrawer;



/**
 * \brief Path Find Test.
 */
class mePathFindTest : public deObject{
private:
	deEngine *pEngine;
	meWorld *pWorld;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSPath;
	deNavigator *pEngNavigator;
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new path find test. */
	mePathFindTest( deEngine *engine );
	/** \brief Cleans up the path find test. */
	virtual ~mePathFindTest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** \brief Retrieves the engine navigator. */
	inline deNavigator *GetEngineNavigator() const{ return pEngNavigator; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld( meWorld *world );
	
	/** \brief Retrieves the start position. */
	inline const decDVector &GetStartPosition() const{ return pStartPosition; }
	/** \brief Sets the start position. */
	void SetStartPosition( const decDVector &position );
	/** \brief Retrieves the goal position. */
	inline const decDVector &GetGoalPosition() const{ return pGoalPosition; }
	/** \brief Sets the goal position. */
	void SetGoalPosition( const decDVector &position );
	/** \brief Retrieves the start orientation. */
	inline const decVector &GetStartOrientation() const{ return pStartOrientation; }
	/** \brief Sets the start orientation. */
	void SetStartOrientation( const decVector &orientation );
	/** \brief Retrieves the goal orientation. */
	inline const decVector &GetGoalOrientation() const{ return pGoalOrientation; }
	/** \brief Sets the goal orientation. */
	void SetGoalOrientation( const decVector &orientation );
	/** \brief Retrieves the layer number. */
	inline int GetLayer() const{ return pLayer; }
	/** \brief Sets the layer number. */
	void SetLayer( int layer );
	/** \brief Retrieves the space type to navigate. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	/** \brief Sets the space type to navigate. */
	void SetSpaceType( deNavigationSpace::eSpaceTypes spaceType );
	
	/** \brief Found path. */
	inline const deNavigatorPath &GetPath() const{ return pPath; }
	
	/** \brief Set found path. */
	void SetPath( const deNavigatorPath &path );
	
	/** \brief Blocking cost. */
	inline float GetBlockingCost() const{ return pBlockingCost; }
	
	/** \brief Set blocking cost. */
	void SetBlockingCost( float cost );
	
	/** \brief Determines if the path is shown. */
	inline bool GetShowPath() const{ return pShowPath; }
	/** \brief Sets if the path is shown. */
	void SetShowPath( bool showPath );
	
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
