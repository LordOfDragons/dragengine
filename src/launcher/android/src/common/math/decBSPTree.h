/* 
 * Drag[en]gine Android Launcher
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

// include only once
#ifndef _DECBSPTREE_H_
#define _DECBSPTREE_H_

// includes
#include "decMath.h"

// definitions
class decBSPTreeFace;
class decCollisionVolume;



/**
 * @brief Generic BSPTree Class.
 *
 * Provides the generic implementation of an bsp tree algorithm. For real usage subclass
 * this class and implement the required functions. Every BSPTree object is the root of
 * a bsp tree. The top most bsp tree object with no parent is the real bsp tree root and
 * all children virtual octree roots. The subclass overwrites the node creation function
 * to create nodes of its own class. Furthermore the subclass has to implement the
 * management of the node content. This bsp tree class only provides the skeleton required
 * for a bsp tree algorithm and does not implement any node content logic.
 */
class decBSPTree{
public:
	/** Sideness. */
	enum eSideness{
		/** Front side. */
		esFront,
		/** Back side. */
		esBack,
		/** Overlaping. */
		esOverlaping,
		/** Coplanar. */
		esCoplanar
	};
	
private:
	decBSPTree *pNodeFront;
	decBSPTree *pNodeBack;
	decVector pNormal;
	float pDistance;
	decBSPTree *pParent;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new generic bsp tree object. */
	decBSPTree( const decVector &normal, float distance );
	/** Cleans up the generic bsp tree object. */
	virtual ~decBSPTree();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent of the bsp tree or NULL if a root bsp tree. */
	inline decBSPTree *GetParent() const{ return pParent; }
	/** Sets the parent of the bsp tree or NULL if a root bsp tree. */
	void SetParent( decBSPTree *parent );
	/** Retrieves the normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	/** Retrieves the distance. */
	inline float GetDistance() const{ return pDistance; }
	/** Retrieves the front node or NULL if not existing. */
	inline decBSPTree *GetFrontNode() const{ return pNodeFront; }
	/** Sets the front node or NULL if not existing. */
	void SetFrontNode( decBSPTree *bsptree );
	/** Retrieves the back node or NULL if not existing. */
	inline decBSPTree *GetBackNode() const{ return pNodeBack; }
	/** Sets the back node or NULL if not existing. */
	void SetBackNode( decBSPTree *bsptree );
	/**
	 * Clears the bsp tree. If clearNodes is set to true all elements are cleared and
	 * nodes are reset to NULL. Otherwise only all elements are removed but the
	 * nodes stay intact.
	 */
	void ClearTree( bool clearNodes );
	
	/**
	 * Creates new bsp tree for the specified octant. Implement this function
	 * to create a new bsp tree of your own type. Do not set the parent of
	 * bsp tree. The caller is responsible for this action if applicable.
	 */
	virtual decBSPTree *CreateBSPTree( const decVector &normal, float distance ) const;
	/** Clears the content of this node. */
	virtual void ClearNodeContent();
	/** Add face as a coplanar face for this node. */
	virtual void AddCoplanarFace( const decBSPTreeFace &face );
	
	/**
	 * Tests the face against this node. Returns 1 if the face is fully on
	 * the front side or -1 if fully on the back side. If the face crosses
	 * the plane 0 is returned.
	 */
	int TestFaceAgainstNode( const decBSPTreeFace &face ) const;
	/**
	 * Tests if a face is inside the bsp tree. The face is send down the bsp
	 * tree like during an insert action but instead of inserting the last
	 * state is returned. If the face is in front of the last split plane
	 * it is considered inside returning 1 as result. If the face is on the
	 * back side it is considered outside and -1 is returned as the result.
	 * if the face crosses the last split plane 0 is returned as result.
	 */
	bool IsFaceInsideTree( const decBSPTreeFace &face ) const;
	/** Splits a face against this node. */
	void SplitFace( const decBSPTreeFace &face, decBSPTreeFace &front, decBSPTreeFace &back ) const;
	/** Adds a new face into bsp tree splitting it if required. */
	void InsertFaceIntoTree( const decBSPTreeFace &face );
	/*@}*/
};

// end of include only once
#endif
