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

#ifndef _DEBASEAINAVIGATOR_H_
#define _DEBASEAINAVIGATOR_H_

#include "../../../common/math/decMath.h"

class deCollider;
class deNavigatorPath;



/**
 * \brief AI module navigator peer.
 */
class DE_DLL_EXPORT deBaseAINavigator{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAINavigator();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAINavigator();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Layer number changed. */
	virtual void LayerChanged();
	
	/** \brief Space type changed. */
	virtual void SpaceTypeChanged();
	
	/** \brief Costs changed. */
	virtual void CostsChanged();
	
	/** \brief Types changed. */
	virtual void TypesChanged();
	
	/** \brief Parameters changed. */
	virtual void ParametersChanged();
	
	
	
	/**
	 * \brief Nearest point on nearest navigation space.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] point Point to find nearest point for.
	 * \param[in] radius Maximum radius clamped to 0 or larger to find nearest point in.
	 * \param[out] nearestPoint Set to the nearest point if found.
	 * \param[out] nearestType Set to the type number of the nearest point if found.
	 * \retval true Nearest point is found and written to \em nearestPoint and \em nearestType.
	 * \retval false No nearest point found inside \em radius around \em point.
	 * \retval false There are no matching navigation spaces.
	 */
	virtual bool NearestPoint(const decDVector &point, float radius, decDVector &nearestPoint, int &nearestType);
	
	/**
	 * \brief Distance moving from point along direction before crossing navigation space boundaries.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] origin Origin to start moving. Use NearestPoint() to make sure it is located on a navigation space.
	 * \param[in] direction Direction to move starting at \em origin.
	 * \param[out] distance Set to the distance along \em displacement as percentage from 0 to 1 before touching
	 *                      a navigation space boundary. Set to 1 if no border is touched.
	 * \retval true \em origin is located in the navigation space and moving along \em direction a navigation space
	 *              boundary is touched. \em distance is set to the travelled percentage of \em direction.
	 * \retval false \em origin is not located in the navigation space.
	 * \retval false No navigation space boundary is hit moving along line.
	 */
	virtual bool LineCollide(const decDVector &origin, const decVector &direction, float &distance);
	
	/**
	 * \brief Find path.
	 * 
	 * Replaces path with found path. If path can not be found the path is empty.
	 * 
	 * \param[out] path Path to update.
	 * \param[in] start Start position of path.
	 * \param[in] goal Goal position of path.
	 */
	virtual void FindPath(deNavigatorPath &path, const decDVector &start, const decDVector &goal);
	
	/**
	 * \brief Test path for collision using ray test.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	virtual bool PathCollideRay(const deNavigatorPath &path, deCollider &collider,
		int &hitAfterPoint, float &hitDistance);
	
	/**
	 * \brief Test path for collision using ray test in range.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	virtual bool PathCollideRay(const deNavigatorPath &path, deCollider &collider,
		const decDVector &startPosition, int nextPoint, float maxDistance,
		int &hitAfterPoint, float &hitDistance);
	
	/**
	 * \brief Test path for collision using a collider moved along the path.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	virtual bool PathCollideShape(const deNavigatorPath &path, deCollider &collider,
		deCollider &agent, int &hitAfterPoint, float &hitDistance);
	
	/**
	 * \brief Test path for collision using a collider moved along the path in range.
	 * 
	 * Default implementation returns \em false. AI module has to subclass this method.
	 * 
	 * \param[in] path Path to use for testing.
	 * \param[out] hitAfterPoint Index of point after which the collision has been found.
	 * \param[out] hitDistance Relative distance from 0 to 1 along segment after \em hitAfterPoint.
	 * \retval true if a collision is found and \em hitAfterPoint and \em hitDistance are set.
	 * \retval false No collision found.
	 */
	virtual bool PathCollideShape(const deNavigatorPath &path, deCollider &collider,
		deCollider &agent, const decDVector &startPosition, int nextPoint,
		float maxDistance, int &hitAfterPoint, float &hitDistance);
	/*@}*/
};

#endif
