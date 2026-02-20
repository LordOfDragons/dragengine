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

#ifndef _DEDAISPACEGRIDVERTEX_H_
#define _DEDAISPACEGRIDVERTEX_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceGrid;



/**
 * \brief Space grid vertex.
 */
class dedaiSpaceGridVertex{
public:
	/** \brief Path finding type. */
	enum ePathFindingTypes{
		/** \brief Vertex is in no list. */
		epftFree,
		
		/** \brief Vertex is in the open list. */
		epftOpen,
		
		/** \brief Vertex is in the closed list. */
		epftClosed
	};
	
	
	
private:
	dedaiSpaceGrid *pGrid;
	decVector pPosition;
	int pFirstEdge;
	unsigned short pIndex;
	unsigned short pEdgeCount;
	unsigned short pFirstLink;
	unsigned short pLinkCount;
	
	bool pEnabled;
	
	dedaiSpaceGridVertex *pPathParent;
	float pPathCostF;
	float pPathCostG;
	float pPathCostH;
	ePathFindingTypes pPathType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create grid vertex. */
	dedaiSpaceGridVertex();
	
	/** \brief Clean up grid vertex. */
	~dedaiSpaceGridVertex();
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
	
	
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Index of the first edge. */
	inline unsigned short GetFirstEdge() const{ return pFirstEdge; }
	
	/** \brief Set index of the first edge. */
	void SetFirstEdge(unsigned short first);
	
	/** \brief Number of edges. */
	inline unsigned short GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Set number of edges. */
	void SetEdgeCount(unsigned short count);
	
	/** \brief Index of the first link. */
	inline unsigned short GetFirstLink() const{ return pFirstLink; }
	
	/** \brief Set index of the first link. */
	void SetFirstLink(unsigned short first);
	
	/** \brief Number of links. */
	inline unsigned short GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Set number of links. */
	void SetLinkCount(unsigned short count);
	
	
	
	/** \brief Vertex is enabled for path finding. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if vertex is enabled for path finding. */
	void SetEnabled(bool enabled);
	
	
	
	/** \brief Path parent vertex or \em NULL. */
	inline dedaiSpaceGridVertex *GetPathParent() const{ return pPathParent; }
	
	/** \brief Set path parent vertex or \em NULL. */
	void SetPathParent(dedaiSpaceGridVertex *face);
	
	/** \brief F path cost. */
	inline float GetPathCostF() const{ return pPathCostF; }
	
	/** \brief Set F path cost. */
	void SetPathCostF(float cost);
	
	/** \brief G path cost. */
	inline float GetPathCostG() const{ return pPathCostG; }
	
	/** \brief Set G path cost. */
	void SetPathCostG(float cost);
	
	/** \brief H path cost. */
	inline float GetPathCostH() const{ return pPathCostH; }
	
	/** \brief Set H path cost. */
	void SetPathCostH(float cost);
	
	/** \brief Path type. */
	inline int GetPathType() const{ return pPathType; }
	
	/** \brief Set path type. */
	void SetPathType(ePathFindingTypes type);
	
	/** \brief Clear path information. */
	void ClearPath();
	/*@}*/
};

#endif
