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

#ifndef _DEDAILAYER_H_
#define _DEDAILAYER_H_

#include "../costs/dedaiCostTable.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>

class dedaiSpaceMeshFace;
class dedaiSpaceGridVertex;
class dedaiSpaceGridEdge;
class dedaiWorld;



/**
 * \brief Navigation layer.
 */
class dedaiLayer : public deObject{
private:
	dedaiWorld &pWorld;
	int pLayer;
	
	dedaiCostTable pCostTable;
	
	bool pDirty;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedaiLayer> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layer. */
	dedaiLayer(dedaiWorld &world, int layer);
	
	/** \brief Clean up layer. */
	virtual ~dedaiLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent world. */
	inline dedaiWorld &GetWorld() const{return pWorld;}
	
	/** \brief Layer. */
	inline int GetLayer() const{return pLayer;}
	
	
	
	/** \brief Cost table. */
	inline dedaiCostTable &GetCostTable(){return pCostTable;}
	inline const dedaiCostTable &GetCostTable() const{return pCostTable;}
	
	
	
	/** \brief Update layer. */
	void Update(float elapsed);
	
	/** \brief Prepare layer if dirty. */
	void Prepare();
	
	
	
	/**
	 * \brief Mark dirty.
	 * \details Used by for developer mode to force an update the next time.
	 */
	void MarkDirty();
	
	
	
	/** \brief Navigation grid vertex closest to a given position. */
	dedaiSpaceGridVertex *GetGridVertexClosestTo(const decDVector &position, float &distance);
	
	/** \brief Navigation mesh face closest to a given position. */
	dedaiSpaceMeshFace *GetMeshFaceClosestTo(const decDVector &position, float &distance);
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceGridEdge *GetGridNearestPoint(const decDVector &point, float radius,
		decDVector &nearestPoint, float &nearestLambda);
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceMeshFace *GetNavMeshNearestPoint(const decDVector &point, float radius, decDVector &nearest);
	
	/**
	 * \brief Check for line colliding with mesh boundaries.
	 */
	bool NavMeshLineCollide(const decDVector &origin, const decVector &direction, float &distance);
	
	
	
	/** \brief Invalidate linking for all navigation spaces. */
	void InvalidateLinks();
	
	/** \brief Invalidate linking for all navigation spaces. */
	void InvalidateLinks(deNavigationSpace::eSpaceTypes type);
	
	/** \brief Invalidate linking for overlapping navigation spaces. */
	void InvalidateLinks(deNavigationSpace::eSpaceTypes type, const decDVector &boxMin, const decDVector &boxMax);
	
	
	
	/**
	 * \brief Invalidate blocking for all navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking();
	
	/**
	 * \brief Invalidate blocking for all navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking(deNavigationSpace::eSpaceTypes type);
	
	/**
	 * \brief Invalidate blocking for overlapping navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking(deNavigationSpace::eSpaceTypes type, const decDVector &boxMin, const decDVector &boxMax);
	/*@}*/
	
	
	
private:
	void pUpdateCostTable();
	void pNavSpacesPrepare();
	void pNavSpacesPrepareLinks();
	void pNavigatorsPrepare();
};

#endif
