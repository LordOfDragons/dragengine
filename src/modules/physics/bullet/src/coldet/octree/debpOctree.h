/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPOCTREE_H_
#define _DEBPOCTREE_H_

#include <dragengine/common/math/decMath.h>

class debpOctreeVisitor;
class debpCollisionVolume;



/**
 * @brief Generic Octree Class.
 * Provides the generic implementation of an octree algorithm. For real usage subclass
 * this class and implement the required functions. Every Octree object is the root of
 * an octree. The top most octree object with no parent is the real octree root and
 * all children virtual octree roots. The subclass overwrites the node creation function
 * to create nodes of its own class. Furthermore the subclass has to implement the
 * management of the node content. This octree class only provides the skeleton required
 * for an octree algorithm and does not implement any node content logic.
 */
class debpOctree{
public:
	/**
	 * Specified the constants used for the octants. There exists two equal
	 * versions suitable for different types of implementations. The first
	 * set of octant definitions is classified using the axis coordinates.
	 * They are also aligned in a way that the definition value represents
	 * a bit mask. The first bit is the x axis, the second the y axis and
	 * the third the z axis. A set bit indicates positive direction and a
	 * 0 negative direction. This set is useful for implementations working
	 * with bit masks to identify octants. The second set of octant definitions
	 * uses verbal descriptions of the octants and is useful for implementations
	 * based around the logic location of an octant. Both sets include also
	 * a definition for a null octant. This is used by functions searching
	 * for octants to indicate that no matching octant could be found.
	 */
	enum eOctants{
		/** Node X- Y- Z-, bitwise 000. */
		eoXnegYnegZneg,
		/** Node X- Y- Z+, bitwise 001. */
		eoXnegYnegZpos,
		/** Node X- Y+ Z-, bitwise 010. */
		eoXnegYposZneg,
		/** Node X- Y+ Z+, bitwise 011. */
		eoXnegYposZpos,
		/** Node X+ Y- Z-, bitwise 100. */
		eoXposYnegZneg,
		/** Node X+ Y- Z+, bitwise 101. */
		eoXposYnegZpos,
		/** Node X+ Y+ Z-, bitwise 110. */
		eoXposYposZneg,
		/** Node X+ Y+ Z+, bitwise 111. */
		eoXposYposZpos,
		/** Special constant for searching meaning no match */
		eoNotFound,
		
		/** Node Front-Left-Top */
		eoFrontLeftTop = eoXnegYposZneg,
		/** Node Front-Right-Top */
		eoFrontRightTop = eoXposYposZneg,
		/** Node Front-Left-Bottom */
		eoFrontLeftBottom = eoXnegYnegZneg,
		/** Node Front-Right-Bottom */
		eoFrontRightBottom = eoXposYnegZneg,
		/** Node Back-Left-Top */
		eoBackLeftTop = eoXnegYposZpos,
		/** Node Back-Right-Top */
		eoBackRightTop = eoXposYposZpos,
		/** Node Back-Left-Bottom */
		eoBackLeftBotom = eoXnegYnegZpos,
		/** Node Back-Right-Bottom */
		eoBackRightBottom = eoXposYnegZpos
	};
	
private:
	debpOctree *pNodes[ 8 ];
	decVector pCenter;
	decVector pHalfSize;
	debpOctree *pParent;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic octree object. */
	debpOctree( const decVector &center, const decVector &halfSize );
	/** Cleans up the generic octree object. */
	virtual ~debpOctree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent of the octree or NULL if a root octree. */
	inline debpOctree *GetParent() const{ return pParent; }
	/** Sets the parent of the octree or NULL if a root octree. */
	void SetParent( debpOctree *parent );
	/** Retrieves the center of the octree. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half size of the octree. */
	inline const decVector &GetHalfSize() const{ return pHalfSize; }
	/**
	 * Retrieves one of the 8 child nodes. This is NULL if there
	 * exists no such node yet. You can use either an index from 0
	 * to 7 inclusive or use one of the the eOctants constants.
	 */
	debpOctree *GetNodeAt( int octant ) const;
	/**
	 * Sets one of the eight child nodes. You can use either an index from 0
	 * to 7 inclusive or use one of the the eOctant constants. Node can be
	 * NULL to remove the child node.
	 */
	void SetNodeAt( int octant, debpOctree *octree );
	/**
	 * Looks for the child node in which the given box lies. If the child node
	 * does not yet exist it is created. If found the node is returned. If
	 * no node could be found NULL is returned.
	 */
	debpOctree *GetNodeAtBox( const decVector &boxCenter, const decVector &boxHalfSize );
	/**
	 * Looks for the child node in which the box lies. If found the node
	 * is returned. If no node could be found NULL or the node does not exist
	 * yet NULL is returned.
	 */
	debpOctree *FindNodeAtBox( const decVector &boxCenter, const decVector &boxHalfSize ) const;
	/**
	 * Looks for the octant in which the element lies. Returns eoNotFound
	 * if no octant fully contains the element.
	 */
	int FindOctantAtBox( const decVector &boxCenter, const decVector &boxHalfSize ) const;
	/** Determines if the box is located completely in this node. */
	bool ContainsBox( const decVector &boxCenter, const decVector &boxHalfSize ) const;
	/**
	 * Looks for the child node in which the point lies. If found the node
	 * is returned. If no node could be found NULL or the node does not exist
	 * yet NULL is returned.
	 */
	debpOctree *FindNodeAtPoint( const decVector &point ) const;
	/** Looks for the octant in which the point lies. */
	int FindOctantAtPoint( const decVector &point ) const;
	/** Determines if the point is located in this node. */
	bool ContainsPoint( const decVector &point ) const;
	
	/** Searches for the Node containing a given box. */
	debpOctree *SearchTreeForBox( const decVector &boxCenter, const decVector &boxHalfSize ) const;
	/** Searches for the Node where the given point lies in. */
	debpOctree *SearchTreeForPoint( const decVector &point ) const;
	/** Visits all nodes. */
	void VisitNodes( debpOctreeVisitor *visitor );
	/** Visits all nodes which collide with the given collision volume. */
	void VisitNodesColliding( debpOctreeVisitor *visitor, debpCollisionVolume *volume );
	/** Visits all nodes which collide with the given box. */
	void VisitNodesColliding( debpOctreeVisitor *visitor, const decVector &boxMinExtend, const decVector &boxMaxExtend );
	/**
	 * Clears the octree. If clearNodes is set to true all elements are cleared and
	 * nodes are reset to NULL. Otherwise only all elements are removed but the
	 * nodes stay intact.
	 */
	void ClearTree( bool clearNodes );
	
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual debpOctree *CreateOctree( int octant ) const = 0;
	/** Clears the content of this node. */
	virtual void ClearNodeContent() = 0;
	/*@}*/
};

// end of include only once
#endif
