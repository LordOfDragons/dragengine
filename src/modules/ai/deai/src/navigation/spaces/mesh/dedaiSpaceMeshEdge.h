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

#ifndef _DEDAISPACEMESHEDGE_H_
#define _DEDAISPACEMESHEDGE_H_



/**
 * \brief Edge in space mesh.
 * 
 * Every edge uses 2 vertices and belongs to 1 or 2 faces. Faces can be -1 if not assigned.
 */
class dedaiSpaceMeshEdge{
private:
	unsigned short pVertex1;
	unsigned short pVertex2;
	short pFace1;
	short pFace2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh edge. */
	dedaiSpaceMeshEdge();
	
	/** \brief Clean up space mesh edge. */
	~dedaiSpaceMeshEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of first vertex. */
	inline unsigned short GetVertex1() const{return pVertex1;}
	
	/** \brief Set index of first vertex. */
	void SetVertex1(unsigned short vertex);
	
	/** \brief Index of second vertex. */
	inline unsigned short GetVertex2() const{return pVertex2;}
	
	/** \brief Set index of second vertex. */
	void SetVertex2(unsigned short vertex);
	
	/** \brief Index of first face or -1 if not linked to any face. */
	inline short GetFace1() const{return pFace1;}
	
	/** \brief Set index of first face or -1 if not linked to any face. */
	void SetFace1(short face);
	
	/** \brief Index of second face or -1 if not linked to two faces. */
	inline short GetFace2() const{return pFace2;}
	
	/** \brief Set index of second face or -1 if not linked to two faces. */
	void SetFace2(short face);
	/*@}*/
};

#endif
