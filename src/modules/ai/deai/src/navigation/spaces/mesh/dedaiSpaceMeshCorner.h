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

#ifndef _DEDAISPACEMESHCORNER_H_
#define _DEDAISPACEMESHCORNER_H_



/**
 * \brief No-cost table entry.
 * \details Special entry used for split edges to be excluded from cost look-up.
 */
#define CORNER_NO_COST		( ( unsigned short )~0 )
#define CORNER_NO_LINK		( ( short )-1 )



/**
 * \brief Space mesh corner.
 * 
 * Corner of a face in a space mesh. Stores the index to the vertex of the corner as well
 * as the index of the edge leading from this corner to the next one.
 */
class dedaiSpaceMeshCorner{
private:
	unsigned short pVertex;
	unsigned short pEdge;
	unsigned short pTypeNumber;
	short pLink;
	bool pEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh corner. */
	dedaiSpaceMeshCorner();
	
	/** \brief Clean up space mesh corner. */
	~dedaiSpaceMeshCorner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of vertex. */
	inline unsigned short GetVertex() const{ return pVertex; }
	
	/** \brief Set index of vertex. */
	void SetVertex( unsigned short vertex );
	
	/** \brief Index of edge. */
	inline unsigned short GetEdge() const{ return pEdge; }
	
	/** \brief Set index of edge. */
	void SetEdge( unsigned short edge );
	
	/** \brief Type number to cross edge. */
	inline unsigned short GetTypeNumber() const{ return pTypeNumber; }
	
	/** \brief Set type number to cross edge. */
	void SetTypeNumber( unsigned short typeNumber );
	
	/** \brief Link index or -1 if edge is not linked. */
	inline short GetLink() const{ return pLink; }
	
	/** \brief Set link index or -1 if edge is not linked. */
	void SetLink( short link );
	
	/** \brief Corner is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if corner is enabled. */
	void SetEnabled( bool enabled );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brier Copy corner. */
	dedaiSpaceMeshCorner &operator=( const dedaiSpaceMeshCorner &corner );
	/*@}*/
};

#endif
