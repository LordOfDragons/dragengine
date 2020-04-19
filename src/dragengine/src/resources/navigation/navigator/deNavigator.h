/* 
 * Drag[en]gine Game Engine
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

#ifndef _DENAVIGATOR_H_
#define _DENAVIGATOR_H_

#include "../space/deNavigationSpace.h"
#include "../../deResource.h"
#include "../../../common/math/decMath.h"

class deCollider;
class deBaseAINavigator;
class deNavigatorManager;
class deNavigatorType;
class deNavigatorPath;
class deWorld;


/**
 * \brief Navigate navigation spaces.
 * 
 * Blocking cost defines the maximum cost above which a node or conneciton is considered
 * impassable. By default this is set to 1'000'000. Hence using a fix cost of more than
 * 1M marks the node or connection as blocked or impassable.
 */
class deNavigator : public deResource{
private:
	int pLayer;
	deNavigationSpace::eSpaceTypes pSpaceType;
	float pMaxOutsideDistance;
	
	float pDefFixCost;
	float pDefCostPerMeter;
	float pBlockingCost;
	deNavigatorType *pTypes;
	int pTypeCount;
	int pTypeSize;
	
	deBaseAINavigator *pPeerAI;
	
	deWorld *pParentWorld;
	deNavigator *pLLWorldPrev;
	deNavigator *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigator. */
	deNavigator( deNavigatorManager *manager );
	
protected:
	/**
	 * \brief Clean up navigator.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNavigator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer( int layer );
	
	/** \brief Space type to navigate. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	
	/** \brief Set space type to navigate. */
	void SetSpaceType( deNavigationSpace::eSpaceTypes spaceType );
	
	/**
	 * \brief Maximum distance the start and goal point are allowed to be outside any navigation space.
	 * 
	 * If the start or goal point is outside the range path update returns an empty path.
	 * The default disstance is 0.5m .
	 */
	inline float GetMaxOutsideDistance() const{ return pMaxOutsideDistance; }
	
	/**
	 * \brief Set maximum distance the start and goal point are allowed to be outside any navigation space.
	 * 
	 * If the start or goal point is outside the range path update returns an empty path.
	 */
	void SetMaxOutsideDistance( float maxDistance );
	
	
	
	/** \brief Fix cost to use if no matching type is found. */
	inline float GetDefaultFixCost() const{ return pDefFixCost; }
	
	/** \brief Set fix cost to use if no matching type is found. */
	void SetDefaultFixCost( float cost );
	
	/** \brief Cost per meter to use if no matching type is found. */
	inline float GetDefaultCostPerMeter() const{ return pDefCostPerMeter; }
	
	/** \brief Set cost per meter to use if no matching type is found. */
	void SetDefaultCostPerMeter( float costPerMeter );
	
	/** \brief Maximum cost beyond which a node or path is considered impassable. */
	inline float GetBlockingCost() const{ return pBlockingCost; }
	
	/** \brief Set maximum cost beyond which a node or path is considered impassable. */
	void SetBlockingCost( float cost );
	
	
	
	/** \brief Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Type by index. */
	deNavigatorType *GetTypeAt( int index ) const;
	
	/** \brief Matching type or NULL if not found. */
	deNavigatorType *GetTypeWith( int typeValue ) const;
	
	/** \brief Index of a type or -1 if not found. */
	int IndexOfType( deNavigatorType *type ) const;
	
	/** \brief Index of the type with the given type value or -1 if not found. */
	int IndexOfTypeWith( int typeValue ) const;
	
	/** \brief Type exists. */
	bool HasType( deNavigatorType *type ) const;
	
	/** \brief Type with the given type value exists. */
	bool HasTypeWith( int typeValue ) const;
	
	/** \brief Add type if not existing returning the type. */
	deNavigatorType *AddType( int type );
	
	/** \brief Remove type. */
	void RemoveType( deNavigatorType *type );
	
	/** \brief Remove type if existing. */
	void RemoveTypeWith( int typeValue );
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	
	/** \brief Notify peers types changed. */
	void NotifyTypesChanged();
	
	
	
	/**
	 * \brief Find path.
	 * 
	 * Replaces path with found path. If path can not be found the path is empty.
	 * 
	 * \param[out] path Path to update.
	 * \param[in] start Start position of path.
	 * \param[in] goal Goal position of path.
	 */
	void FindPath( deNavigatorPath &path, const decDVector &start, const decDVector &goal );
	/*@}*/
	
	
	
	/** \name Testing */
	/*@{*/
	/**
	 * \brief Nearest point on nearest navigation space.
	 * 
	 * Default implementation returns false. AI module has to subclass this method.
	 * 
	 * \param[in] point Point to find nearest point for.
	 * \param[in] radius Maximum radius clamped to 0 or larger to find nearest point in.
	 * \param[out] nearestPoint Set to the nearest point if found.
	 * \param[out] nearestType Set to the type number of the nearest point if found.
	 * \retval true Nearest point is found and written to \em nearestPoint and \em nearestType.
	 * \retval false No nearest point found inside \em radius around \em point.
	 * \retval false There are no matching navigation spaces.
	 */
	bool NearestPoint( const decDVector &point, float radius, decDVector &nearestPoint,
		int &nearestType ) const;
	
	/**
	 * \brief Distance moving from point along direction before crossing navigation space boundaries.
	 * 
	 * \param[in] origin Origin to start moving. Use NearestPoint() to make sure it is located
	 *                   on a navigation space.
	 * \param[in] direction Direction to move starting at \em origin.
	 * \param[out] distance Set to the distance along \em displacement as percentage
	 *                      from 0 to 1 before touching
	 * a navigation space boundary. Set to 1 if no border is touched.
	 * \retval true \em origin is located in the navigation space and moving along
	 *              \em direction a navigation space boundary is touched. \em distance is
	 *              set to the travelled percentage of \em direction.
	 * \retval false \em origin is not located in the navigation space.
	 * \retval false No navigation space boundary is hit moving along line.
	 */
	bool LineCollide( const decDVector &origin, const decVector &direction, float &distance ) const;
	/*@}*/
	
	
	
	/** \name Path Collision */
	/*@{*/
	/**
	 * \brief Test path for collision using ray test.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	bool PathCollideRay( const deNavigatorPath &path, deCollider &collider,
		int &hitAfterPoint, float &hitDistance ) const;
	
	/**
	 * \brief Test path for collision using ray test inside range.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[in] startPosition Starting position for testing.
	 * \param[in] nextPoint Index of next point along the path to test. If this is equal to
	 * GetPathPointCount() the goal position is used.
	 * \param[in] maxDistance Limit testing to \em maxDistance meters after \em startPosition along path.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \throws EOutOfBoundary \em nextPoint is less than 0 or larger than GetPathPointCount().
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	bool PathCollideRay( const deNavigatorPath &path, deCollider &collider,
		const decDVector &startPosition, int nextPoint, float maxDistance,
		int &hitAfterPoint, float &hitDistance ) const;
	
	/**
	 * \brief Test path for collision using a collider moved along the path.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	bool PathCollideShape( const deNavigatorPath &path, deCollider &collider,
		deCollider &agent, int &hitAfterPoint, float &hitDistance ) const;
	
	/**
	 * \brief Test path for collision using a collider moved along the path inside range.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[in] startPosition Starting position for testing.
	 * \param[in] nextPoint Index of next point along the path to test. If this is equal to
	 * GetPathPointCount() the goal position is used.
	 * \param[in] maxDistance Limit testing to \em maxDistance meters after \em startPosition along path.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \throws EOutOfBoundary \em nextPoint is less than 0 or larger than GetPathPointCount().
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	bool PathCollideShape( const deNavigatorPath &path, deCollider &collider,
		deCollider &agent, const decDVector &startPosition, int nextPoint,
		float maxDistance, int &hitAfterPoint, float &hitDistance ) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief AI system peer. */
	inline deBaseAINavigator *GetPeerAI() const{ return pPeerAI; }
	
	/** \brief Set AI system peer. */
	void SetPeerAI( deBaseAINavigator *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous navigator in the parent world linked list. */
	inline deNavigator *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next navigator in the parent world linked list. */
	void SetLLWorldPrev( deNavigator *navigator );
	
	/** \brief Next navigator in the parent world linked list. */
	inline deNavigator *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next navigator in the parent world linked list. */
	void SetLLWorldNext( deNavigator *navigator );
	/*@}*/
};

#endif
