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

#ifndef _DEOGLPFCLUSTERGENERATOR_H_
#define _DEOGLPFCLUSTERGENERATOR_H_

#include <dragengine/common/math/decMath.h>



/**
 * Prop Field Cluster Generator.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new prop field cluster generator. */
	deoglPFClusterGenerator(const decVector2 &minExtend, const decVector2 &maxExtend, int maxEntryCount);
	/** Cleans up the prop field cluster generator. */
	~deoglPFClusterGenerator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the minimum extend. */
	inline const decVector2 &GetMinExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend. */
	inline const decVector2 &GetMaxExtend() const{return pMaxExtend;}
	/** Retrieves the maximum entry count. */
	inline int GetMaxEntryCount() const{return pMaxEntryCount;}
	
	/** Retrieves the list of entries. */
	inline const sEntry *GetEntries() const{return pEntries;}
	/** Retrieves the number of entries. */
	inline int GetEntryCount() const{return pEntryCount;}
	/** Adds an entry to the list of entries. */
	void AddEntry(int index, const decVector2 &position);
	/** Remove all entries in this node. */
	void RemoveAllEntries();
	/** Retrieves the extends of all the entries. */
	void GetEntryExtends(decVector2 &minExtend, decVector2 &maxExtend);
	
	/** Retrieves the first node or NULL if there is none. */
	inline deoglPFClusterGenerator *GetNode1() const{return pNode1;}
	/** Retrieves the second node or NULL if there is none. */
	inline deoglPFClusterGenerator *GetNode2() const{return pNode2;}
	
	/**
	 * Adds an entry to the tree. Finds the smallest matching node and adds it. If the node is already
	 * full the node is split in two smaller nodes and the entry added to the appropriate node.
	 */
	void AddEntryToTree(int index, const decVector2 &position);
	/** Locates the smallest node containing the given position. */
	deoglPFClusterGenerator *GetNodeContaining(const decVector2 &position) const;
	/** Split node into two nodes along the given x value distributing the entries into the matching nodes. */
	void SplitAtX(float position);
	/** Split node into two nodes along the given y value distributing the entries into the matching nodes. */
	void SplitAtY(float position);
	/** Clear tree. */
	void ClearTree();
	/*@}*/
};

#endif
