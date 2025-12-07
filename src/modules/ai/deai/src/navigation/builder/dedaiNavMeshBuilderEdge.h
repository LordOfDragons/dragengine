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

#ifndef _DEDAINAVMESHBUILDEREDGE_H_
#define _DEDAINAVMESHBUILDEREDGE_H_

class dedaiNavMeshBuilderFace;



/**
 * @brief Navigation Mesh Builder Edge.
 */
class dedaiNavMeshBuilderEdge{
private:
	int pIndex;
	int pVertex1;
	int pVertex2;
	dedaiNavMeshBuilderFace *pFace1;
	dedaiNavMeshBuilderFace *pFace2;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new navigation mesh builder edge. */
	dedaiNavMeshBuilderEdge();
	/** Creates a new navigation mesh builder edge. */
	dedaiNavMeshBuilderEdge(int vertex1, int vertex2);
	/** Cleans up the navigation mesh builder edge. */
	~dedaiNavMeshBuilderEdge();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the index. */
	void SetIndex(int index);
	/** Retrieves the index of the first vertex. */
	inline int GetVertex1() const{ return pVertex1; }
	/** Sets the index of the first vertex. */
	void SetVertex1(int vertex);
	/** Retrieves the index of the second vertex. */
	inline int GetVertex2() const{ return pVertex2; }
	/** Sets the index of the second vertex. */
	void SetVertex2(int vertex);
	/** Retrieves the first face or NULL if there is none. */
	inline dedaiNavMeshBuilderFace *GetFace1() const{ return pFace1; }
	/** Sets the first face or NULL if there is none. */
	void SetFace1(dedaiNavMeshBuilderFace *face);
	/** Retrieves the second face or NULL if there is none. */
	inline dedaiNavMeshBuilderFace *GetFace2() const{ return pFace2; }
	/** Sets the second face or NULL if there is none. */
	void SetFace2(dedaiNavMeshBuilderFace *face);
	/*@}*/
};

#endif
