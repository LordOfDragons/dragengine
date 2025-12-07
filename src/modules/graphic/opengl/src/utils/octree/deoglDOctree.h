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

#ifndef _DEOGLDOCTREE_H_
#define _DEOGLDOCTREE_H_

#include <dragengine/common/math/decMath.h>

class deoglDOctreeVisitor;
class deoglDCollisionVolume;



/**
 * Generic Double-Precision Octree Class.
 * Provides the generic implementation of an octree algorithm. For real usage subclass
 * this class and implement the required functions. Every Octree object is the root of
 * an octree. The top most octree object with no parent is the real octree root and
 * all children virtual octree roots. The subclass overwrites the node creation function
 * to create nodes of its own class. Furthermore the subclass has to implement the
 * management of the node content. This octree class only provides the skeleton required
 * for an octree algorithm and does not implement any node content logic.
 */
class deoglDOctree{
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
	deoglDOctree *pNodes[8];
	const decDVector pCenter;
	const decDVector pHalfSize;
	const decDVector pMinExtend;
	const decDVector pMaxExtend;
	deoglDOctree *pParent;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic octree object. */
	deoglDOctree(const decDVector &center, const decDVector &halfSize);
	/** Cleans up the generic octree object. */
	virtual ~deoglDOctree();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent of the octree or NULL if a root octree. */
	inline deoglDOctree *GetParent() const{ return pParent; }
	/** Sets the parent of the octree or NULL if a root octree. */
	void SetParent(deoglDOctree *parent);
	/** Retrieves the center of the octree. */
	inline const decDVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half size of the octree. */
	inline const decDVector &GetHalfSize() const{ return pHalfSize; }
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/**
	 * Retrieves one of the 8 child nodes. This is NULL if there
	 * exists no such node yet. You can use either an index from 0
	 * to 7 inclusive or use one of the the eOctants constants.
	 */
	deoglDOctree *GetNodeAt(int octant) const;
	/**
	 * Sets one of the eight child nodes. You can use either an index from 0
	 * to 7 inclusive or use one of the the eOctant constants. Node can be
	 * NULL to remove the child node.
	 */
	void SetNodeAt(int octant, deoglDOctree *octree);
	/**
	 * Looks for the child node in which the given box lies. If the child node
	 * does not yet exist it is created. If found the node is returned. If
	 * no node could be found NULL is returned.
	 */
	deoglDOctree *GetNodeAtBox(const decDVector &minExtend, const decDVector &maxExtend);
	/**
	 * Looks for the child node in which the box lies. If found the node
	 * is returned. If no node could be found NULL or the node does not exist
	 * yet NULL is returned.
	 */
	deoglDOctree *FindNodeAtBox(const decDVector &minExtend, const decDVector &maxExtend) const;
	/**
	 * Looks for the octant in which the element lies. Returns eoNotFound
	 * if no octant fully contains the element.
	 */
	int FindOctantAtBox(const decDVector &minExtend, const decDVector &maxExtend) const;
	/** Determines if the box is located completely in this node. */
	bool ContainsBox(const decDVector &minExtend, const decDVector &maxExtend) const;
	/** Looks for the octant in which the point lies. */
	int FindOctantAtPoint(const decDVector &point) const;
	/** Determines if the point is located in this node. */
	bool ContainsPoint(const decDVector &point) const;
	
	/** Visits all nodes. */
	void VisitNodes(deoglDOctreeVisitor *visitor);
	/** Visits all nodes which collide with the given collision volume. */
	void VisitNodesColliding(deoglDOctreeVisitor *visitor, deoglDCollisionVolume *volume);
	/** Visits all nodes which collide with the given box. */
	void VisitNodesColliding(deoglDOctreeVisitor *visitor, const decDVector &boxMinExtend, const decDVector &boxMaxExtend);
	/**
	 * Clears the octree. If clearNodes is set to true all elements are cleared and
	 * nodes are reset to NULL. Otherwise only all elements are removed but the
	 * nodes stay intact.
	 */
	void ClearTree(bool clearNodes);
	
	/**
	 * Creates new octree for the specified octant. Implement this function
	 * to create a new octree of your own type. Do not set the parent of
	 * octree. The caller is responsible for this action if applicable.
	 */
	virtual deoglDOctree *CreateOctree(int octant) const = 0;
	/** Clears the content of this node. */
	virtual void ClearNodeContent() = 0;
	/*@}*/
};

// end of include only once
#endif
