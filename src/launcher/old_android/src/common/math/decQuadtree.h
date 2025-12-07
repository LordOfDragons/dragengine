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

// include only once
#ifndef _DECQUADTREE_H_
#define _DECQUADTREE_H_

// includes
#include "decMath.h"

// definitions
class decQuadtreeVisitor;
class decCollisionVolume;



/**
 * @brief Generic Quadtree Class.
 *
 * Provides the generic implementation of an quadtree algorithm. For real usage subclass
 * this class and implement the required functions. Every Quadtree object is the root of
 * an quadtree. The top most quadtree object with no parent is the real quadtree root and
 * all children virtual quadtree roots. The subclass overwrites the node creation function
 * to create nodes of its own class. Furthermore the subclass has to implement the
 * management of the node content. This quadtree class only provides the skeleton required
 * for an quadtree algorithm and does not implement any node content logic.
 */
class decQuadtree{
public:
	/**
	 * Specified the constants used for the quadrants. There exists two equal
	 * versions suitable for different types of implementations. The first
	 * set of quadrant definitions is classified using the axis coordinates.
	 * They are also aligned in a way that the definition value represents
	 * a bit mask. The first bit is the x axis and the second the y axis. A set bit
	 * indicates positive direction and a 0 negative direction. This set is useful
	 * for implementations working with bit masks to identify quadrants. The second
	 * set of quadrant definitions uses verbal descriptions of the quadrants and is
	 * useful for implementations based around the logic location of an quadrant.
	 * Both sets include also a definition for a null quadrant. This is used by
	 * functions searching for quadrants to indicate that no matching quadrant could
	 * be found.
	 */
	enum eQuadrants{
		/** Node X- Y-, bitwise 00. */
		eqXnegYneg,
		/** Node X- Y+, bitwise 01. */
		eqXnegYpos,
		/** Node X+ Y-, bitwise 10. */
		eqXposYneg,
		/** Node X+ Y+, bitwise 11. */
		eqXposYpos,
		/** Special constant for searching meaning no match */
		eqNotFound,
		
		/** Node Left-Top */
		eqLeftTop = eqXnegYpos,
		/** Node Right-Top */
		eqRightTop = eqXposYpos,
		/** Node Left-Bottom */
		eqLeftBottom = eqXnegYneg,
		/** Node Right-Bottom */
		eqRightBottom = eqXposYneg
	};
	
private:
	decQuadtree *pNodes[4];
	decVector2 pCenter;
	decVector2 pHalfSize;
	decQuadtree *pParent;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic quadtree object. */
	decQuadtree(const decVector2 &center, const decVector2 &halfSize);
	/** Cleans up the generic quadtree object. */
	virtual ~decQuadtree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent of the quadtree or NULL if a root quadtree. */
	inline decQuadtree *GetParent() const{return pParent;}
	/** Sets the parent of the quadtree or NULL if a root quadtree. */
	void SetParent(decQuadtree *parent);
	/** Retrieves the center of the quadtree. */
	inline const decVector2 &GetCenter() const{return pCenter;}
	/** Retrieves the half size of the quadtree. */
	inline const decVector2 &GetHalfSize() const{return pHalfSize;}
	/**
	 * Retrieves one of the 8 child nodes. This is NULL if there
	 * exists no such node yet. You can use either an index from 0
	 * to 3 inclusive or use one of the the eQuadrants constants.
	 */
	decQuadtree *GetNodeAt(int quadrant) const;
	/**
	 * Sets one of the eight child nodes. You can use either an index from 0
	 * to 3 inclusive or use one of the the eQuadrant constants. Node can be
	 * NULL to remove the child node.
	 */
	void SetNodeAt(int quadrant, decQuadtree *quadtree);
	/**
	 * Looks for the child node in which the given box lies. If the child node
	 * does not yet exist it is created. If found the node is returned. If
	 * no node could be found NULL is returned.
	 */
	decQuadtree *GetNodeAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize);
	/**
	 * Looks for the child node in which the given point lies. If the child node
	 * does not yet exist it is created. If found the node is returned. If
	 * no node could be found NULL is returned.
	 */
	decQuadtree *GetNodeAtPoint(const decVector2 &point);
	/**
	 * Looks for the child node in which the box lies. If found the node
	 * is returned. If no node could be found NULL or the node does not exist
	 * yet NULL is returned.
	 */
	decQuadtree *FindNodeAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const;
	/**
	 * Looks for the quadrant in which the element lies. Returns eqNotFound
	 * if no quadrant fully contains the element.
	 */
	int FindQuadrantAtBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const;
	/** Determines if the box is located completely in this node. */
	bool ContainsBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const;
	/**
	 * Looks for the child node in which the point lies. If found the node
	 * is returned. If no node could be found NULL or the node does not exist
	 * yet NULL is returned.
	 */
	decQuadtree *FindNodeAtPoint(const decVector2 &point) const;
	/** Looks for the quadrant in which the point lies. */
	int FindQuadrantAtPoint(const decVector2 &point) const;
	/** Determines if the point is located in this node. */
	bool ContainsPoint(const decVector2 &point) const;
	
	/** Searches for the Node containing a given box. */
	decQuadtree *SearchTreeForBox(const decVector2 &boxCenter, const decVector2 &boxHalfSize) const;
	/** Searches for the Node where the given point lies in. */
	decQuadtree *SearchTreeForPoint(const decVector2 &point) const;
	/** Visits all nodes. */
	void VisitNodes(decQuadtreeVisitor *visitor);
	/** Visits all nodes which collide with the given collision volume. */
	void VisitNodesColliding(decQuadtreeVisitor *visitor, decCollisionVolume *volume);
	/**
	 * Clears the quadtree. If clearNodes is set to true all elements are cleared and
	 * nodes are reset to NULL. Otherwise only all elements are removed but the
	 * nodes stay intact.
	 */
	void ClearTree(bool clearNodes);
	
	/**
	 * Creates new quadtree for the specified quadrant. Implement this function
	 * to create a new quadtree of your own type. Do not set the parent of
	 * quadtree. The caller is responsible for this action if applicable.
	 */
	virtual decQuadtree *CreateQuadtree(int quadrant) const = 0;
	/** Clears the content of this node. */
	virtual void ClearNodeContent() = 0;
	/*@}*/
};

// end of include only once
#endif
