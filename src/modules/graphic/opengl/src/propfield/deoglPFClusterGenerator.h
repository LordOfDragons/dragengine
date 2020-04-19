/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPFCLUSTERGENERATOR_H_
#define _DEOGLPFCLUSTERGENERATOR_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Prop Field Cluster Generator.
 * Generates clusters out of a list of points using a Kd-Tree type data structure.
 */
class deoglPFClusterGenerator{
public:
	struct sEntry{
		int index;
		decVector2 position;
	};
	
private:
	decVector2 pMinExtend;
	decVector2 pMaxExtend;
	
	sEntry *pEntries;
	int pEntryCount;
	int pEntrySize;
	int pMaxEntryCount;
	
	deoglPFClusterGenerator *pNode1;
	deoglPFClusterGenerator *pNode2;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new prop field cluster generator. */
	deoglPFClusterGenerator( const decVector2 &minExtend, const decVector2 &maxExtend, int maxEntryCount );
	/** Cleans up the prop field cluster generator. */
	~deoglPFClusterGenerator();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the minimum extend. */
	inline const decVector2 &GetMinExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decVector2 &GetMaxExtend() const{ return pMaxExtend; }
	/** Retrieves the maximum entry count. */
	inline int GetMaxEntryCount() const{ return pMaxEntryCount; }
	
	/** Retrieves the list of entries. */
	inline const sEntry *GetEntries() const{ return pEntries; }
	/** Retrieves the number of entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	/** Adds an entry to the list of entries. */
	void AddEntry( int index, const decVector2 &position );
	/** Remove all entries in this node. */
	void RemoveAllEntries();
	/** Retrieves the extends of all the entries. */
	void GetEntryExtends( decVector2 &minExtend, decVector2 &maxExtend );
	
	/** Retrieves the first node or NULL if there is none. */
	inline deoglPFClusterGenerator *GetNode1() const{ return pNode1; }
	/** Retrieves the second node or NULL if there is none. */
	inline deoglPFClusterGenerator *GetNode2() const{ return pNode2; }
	
	/**
	 * Adds an entry to the tree. Finds the smallest matching node and adds it. If the node is already
	 * full the node is split in two smaller nodes and the entry added to the appropriate node.
	 */
	void AddEntryToTree( int index, const decVector2 &position );
	/** Locates the smallest node containing the given position. */
	deoglPFClusterGenerator *GetNodeContaining( const decVector2 &position ) const;
	/** Split node into two nodes along the given x value distributing the entries into the matching nodes. */
	void SplitAtX( float position );
	/** Split node into two nodes along the given y value distributing the entries into the matching nodes. */
	void SplitAtY( float position );
	/** Clear tree. */
	void ClearTree();
	/*@}*/
};

#endif
