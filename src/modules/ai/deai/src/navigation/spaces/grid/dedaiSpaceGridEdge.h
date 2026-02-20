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

#ifndef _DEDAISPACEGRIDEDGE_H_
#define _DEDAISPACEGRIDEDGE_H_

class dedaiSpaceGrid;



/**
 * \brief Space grid edge.
 * 
 * Edge in a navigation grid. Every edge connects 2 vertices. The first type is used while
 * traveling from the first vertex to the second and the second type for the other direction.
 */
class dedaiSpaceGridEdge{
private:
	dedaiSpaceGrid *pGrid;
	unsigned short pIndex;
	unsigned short pVertex1;
	unsigned short pVertex2;
	unsigned short pTypeNumber1;
	unsigned short pTypeNumber2;
	float pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create grid edge. */
	dedaiSpaceGridEdge();
	
	/** \brief Clean up grid edge. */
	~dedaiSpaceGridEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent grid. */
	inline dedaiSpaceGrid *GetGrid() const{ return pGrid; }
	
	/** \brief Set parent grid. */
	void SetGrid(dedaiSpaceGrid *grid);
	
	/** \brief Vertex index. */
	inline unsigned short GetIndex() const{ return pIndex; }
	
	/** \brief Set vertex index. */
	void SetIndex(unsigned short index);
	
	
	
	/** \brief Index of the first vertex. */
	inline unsigned short GetVertex1() const{ return pVertex1; }
	
	/** \brief Set index of the first vertex. */
	void SetVertex1(unsigned short vertex);
	
	/** \brief Index of the second vertex. */
	inline unsigned short GetVertex2() const{ return pVertex2; }
	
	/** \brief Set index of the second vertex. */
	void SetVertex2(unsigned short vertex);
	
	/** \brief Type number to use traveling the edge from the first vertex to the second. */
	inline unsigned short GetTypeNumber1() const{ return pTypeNumber1; }
	
	/** \brief Set type number to use traveling the edge from the first vertex to the second. */
	void SetTypeNumber1(unsigned short typeNumber);
	
	/** \brief Type number to use traveling the edge from the second vertex to the first. */
	inline unsigned short GetTypeNumber2() const{ return pTypeNumber2; }
	
	/** \brief Set type number to use traveling the edge from the second vertex to the first. */
	void SetTypeNumber2(unsigned short typeNumber);
	
	/** \brief Length of the edge. */
	inline float GetLength() const{ return pLength; }
	
	/** \brief Set length of the edge. */
	void SetLength(float length);
	/*@}*/
};

#endif
