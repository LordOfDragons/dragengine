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

#ifndef _DENAVIGATIONSPACE_H_
#define _DENAVIGATIONSPACE_H_

#include "../../deResource.h"
#include "../../../common/collection/decTList.h"
#include "../../../common/math/decMath.h"
#include "../../../common/utils/decLayerMask.h"
#include "../../../common/collection/decTLinkedList.h"
#include "../../../common/shape/decShape.h"

class deNavigationSpaceManager;
class deNavigationSpaceCorner;
class deNavigationSpaceEdge;
class deNavigationSpaceFace;
class deNavigationSpaceWall;
class deNavigationSpaceRoom;
class deNavigationSpaceManager;
class deBaseAINavigationSpace;
class deWorld;


/**
 * \brief Navigation Space.
 * Defines a space for AI navigation. A navigation space composes of a list of nodes
 * and connections between them. Navigation spaces can be either grids, meshes or
 * volumes. In addition each navigation space has a layer number. Navigation spaces
 * with the same layer number are linked together. Using different layers different
 * navigation spaces can be used side by side in the same world. This can be due to
 * different requirements of game entities or to use spaces of varying resolution.
 * The navigation grid is the default type for newly created navigation spaces.
 * 
 * \par Navigation Grid
 * Navigation grids are useful for navigation problems where an exact and smooth path
 * is not required. This is usually used for coarse grained navigation typically not
 * directly linked to a visible world. In this navigation space vertices are the nodes
 * and edges are the connections. Thus for a valid navigation grid vertices and edges
 * have to be defined. The cost function is calculated using the type stored in the
 * edges. The resulting cost function is thus:<br/>
 * cost = edge.fixCost + edge.costPerMeter * distance
 * 
 * \par Navigation Mesh
 * Navigation meshes are useful for all kinds of navigation problems in detailed
 * scene geometry where a smooth path around the world is desired. This is the typical
 * space used for AI navigation of visible game actors. Most of the time you want to
 * use this type of navigation space. In this navigation space faces are the nodes and
 * edges the connections. In contrary to the navigation grid the connection is located
 * at the edges of the face. Thus for a valid navigation mesh vertices, corners and
 * faces have to be defined. Here corners are the vertices used in the faces and are
 * stored as a continuous array of vertex indices. Set the firstCorner and cornerCount
 * in faces accordingly to assign the right set of vertex indices to each face. The
 * orientation of faces does not matter. The cost function is calculated using the type
 * stored in the faces as well as the edges. For the edges only the fix cost is used.
 * This way certain edges can be penalized or even disallowed to be crossed. The
 * resulting cost function is thus:<br/>
 * cost = edge.fixCost + face.fixCost + face.costPerMeter * distance
 * 
 * \par Navigation Volume
 * Navigation volumes are useful for all navigation problems that navigation meshes can
 * not accurately represent anymore. These are typically situations where the movement
 * of actors is not limited anymore to moving on the ground with jumping or hovering
 * but where they can roam around three dimensions freely. A good example for this is
 * Descent where the AI roams around in 0-gravity inside a large cavern complex. Here
 * navigation volumes provide the same smooth path finding around the game world as does
 * the navigation mesh just in three dimensions. In this navigation space rooms are the
 * nodes and faces are the connections. Here too the Rooms are directly connected to
 * each other using faces similar to navigation meshes. Thus for a valid navigation
 * volume vertices, corners, faces, walls and rooms have to be defined. Here corners are
 * the vertices used in faces as in the case of navigation meshes and walls are the
 * indices of faces used in the rooms and are stored as a continuous array of face indices.
 * Set the firstWall and wallCount in rooms accordingly to assign the right set of faces
 * to each room. The orientation of faces does not matter. The cost function is calculated
 * using the type stored in the rooms as well as the faces. For the faces only the fix
 * cost is used. This way certain faces can be penalized or even disallowed to be crossed.
 * The resulting cost function is thus:<br/>
 * cost = face.fixCost + room.fixCost + room.costPerMeter * distance
 * 
 * \par Blocker
 * Navigation spaces can contain a list of blocker shapes. The definition and behavior
 * is similar to deNavigationBlocker but with the difference that the blocking applies
 * only to all other navigation spaces except this one. This allows to use a navigation
 * space to refine existing navigation spaces by blocking out certain space and snapping
 * in the new navigation space. A good example is adding stairs to a room where the
 * stairs blocks out all other navigation spaces and attaches itself to the emerging
 * navigation space borders.
 * 
 */
class DE_DLL_EXPORT deNavigationSpace : public deResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deNavigationSpace>;
	
	
public:
	/** \brief Space Type. */
	enum eSpaceTypes{
		/** \brief Navigation Grid. */
		estGrid,
		
		/** \brief Navigation Mesh. */
		estMesh,
		
		/** \brief Navigation Volume. */
		estVolume
	};
	
	
	
private:
	eSpaceTypes pType;
	int pLayer;
	decDVector pPosition;
	decQuaternion pOrientation;
	decLayerMask pLayerMask;
	
	decTList<decVector> pVertices;
	decTList<deNavigationSpaceEdge> pEdges;
	decTList<deNavigationSpaceCorner> pCorners;
	decTList<deNavigationSpaceFace> pFaces;
	decTList<deNavigationSpaceWall> pWalls;
	decTList<deNavigationSpaceRoom> pRooms;
	
	float pSnapDistance;
	float pSnapAngle;
	
	decShape::List pBlockerShapeList;
	int pBlockingPriority;
	
	deBaseAINavigationSpace *pPeerAI;
	
	deWorld *pParentWorld;
	decTObjectLinkedList<deNavigationSpace>::Element pLLWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space with the given resource manager. */
	deNavigationSpace(deNavigationSpaceManager *manager);
	
protected:
	/**
	 * \brief Clean up animator.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deNavigationSpace() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Space type. */
	inline eSpaceTypes GetType() const{ return pType; }
	
	/** \brief Set space type. */
	void SetType(eSpaceTypes type);
	
	/** \brief Layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set layer number. */
	void SetLayer(int layer);
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/**
	 * \brief Snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/**
	 * \brief Set snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	void SetSnapDistance(float distance);
	
	/**
	 * \brief Snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/**
	 * \brief Set snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	void SetSnapAngle(float angle);
	
	/** \brief Blocker shape list. */
	inline decShape::List &GetBlockerShapeList(){ return pBlockerShapeList; }
	inline const decShape::List &GetBlockerShapeList() const{ return pBlockerShapeList; }
	
	/** \brief Notifies the peers that the blocker shape list changed. */
	void NotifyBlockerShapeListChanged();
	
	/**
	 * \brief Blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/**
	 * \brief Set blocking priority.
	 * 
	 * Navigation meshes is blocked by all blockers with an equal or larger blocking priority.
	 * If a blocker shape exists it blocks all navigation meshes with equal or less blocking priority.
	 */
	void SetBlockingPriority(int priority);
	
	/**
	 * Notifies the peers that the layout of the navigation space changed. This has to be
	 * called after changes made to vertices, corners, faces, walls or rooms or if the
	 * space type has been changed. Call this after all changes have been done.
	 */
	void NotifyLayoutChanged();
	
	/** \brief Verify the navigation space. */
	bool Verify() const;
	
	
	/** \brief Vertices. */
	inline decTList<decVector> &GetVertices(){ return pVertices; }
	inline const decTList<decVector> &GetVertices() const{ return pVertices; }
	
	/** \brief Edges. */
	inline decTList<deNavigationSpaceEdge> &GetEdges(){ return pEdges; }
	inline const decTList<deNavigationSpaceEdge> &GetEdges() const{ return pEdges; }
	
	
	/** \brief Corners. */
	inline decTList<deNavigationSpaceCorner> &GetCorners(){ return pCorners; }
	inline const decTList<deNavigationSpaceCorner> &GetCorners() const{ return pCorners; }
	
	
	/** \brief Faces. */
	inline decTList<deNavigationSpaceFace> &GetFaces(){ return pFaces; }
	inline const decTList<deNavigationSpaceFace> &GetFaces() const{ return pFaces; }
	
	
	/** \brief Walls. */
	inline decTList<deNavigationSpaceWall> &GetWalls(){ return pWalls; }
	inline const decTList<deNavigationSpaceWall> &GetWalls() const{ return pWalls; }
	
	
	/** \brief Rooms. */
	inline decTList<deNavigationSpaceRoom> &GetRooms(){ return pRooms; }
	inline const decTList<deNavigationSpaceRoom> &GetRooms() const{ return pRooms; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief AI system peer. */
	inline deBaseAINavigationSpace *GetPeerAI() const{ return pPeerAI; }
	
	/** \brief Set AI system peer. */
	void SetPeerAI(deBaseAINavigationSpace *peer);
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief World linked list element. */
	inline decTObjectLinkedList<deNavigationSpace>::Element &GetLLWorld(){ return pLLWorld; }
	/*@}*/
};

#endif
