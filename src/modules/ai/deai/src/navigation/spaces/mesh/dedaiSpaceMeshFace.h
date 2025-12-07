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

#ifndef _DEDAISPACEMESHFACE_H_
#define _DEDAISPACEMESHFACE_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceMesh;



/**
 * \brief Space mesh face.
 */
class dedaiSpaceMeshFace{
public:
	/** \brief Path finding type. */
	enum ePathFindingTypes{
		/** \brief Face is in no list. */
		epftFree,
		
		/** \brief Face is in the open list. */
		epftOpen,
		
		/** \brief Face is in the closed list. */
		epftClosed
	};
	
	
	
private:
	dedaiSpaceMesh *pMesh;
	int pFirstCorner;
	unsigned short pCornerCount;
	unsigned short pIndex;
	unsigned short pTypeNumber;
	
	decVector pCenter;
	decVector pNormal;
	float pDistance;
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pEntryPoint;
	
	bool pEnabled;
	
	dedaiSpaceMeshFace *pPathParent;
	float pPathCostF;
	float pPathCostG;
	float pPathCostH;
	ePathFindingTypes pPathType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh face. */
	dedaiSpaceMeshFace();
	
	/** \brief Clean up space mesh face. */
	~dedaiSpaceMeshFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent space mesh. */
	inline dedaiSpaceMesh *GetMesh() const{return pMesh;}
	
	/** \brief Set parent space mesh. */
	void SetMesh(dedaiSpaceMesh *mesh);
	
	/** \brief Type number. */
	inline unsigned short GetTypeNumber() const{return pTypeNumber;}
	
	/** \brief Set type number. */
	void SetTypeNumber(unsigned short typeNumber);
	
	/** \brief Face index. */
	inline unsigned short GetIndex() const{return pIndex;}
	
	/** \brief Set face index. */
	void SetIndex(unsigned short index);
	
	
	
	/** \brief Index of the first corner. */
	inline int GetFirstCorner() const{return pFirstCorner;}
	
	/** \brief Set index of the first corner. */
	void SetFirstCorner(int first);
	
	/** \brief Number of corners. */
	inline unsigned short GetCornerCount() const{return pCornerCount;}
	
	/** \brief Set number of corners. */
	void SetCornerCount(unsigned short count);
	
	/** \brief Index of corner with matching edge or -1 if not found. */
	int IndexOfCornerWithEdge(unsigned short edge) const;
	
	/** \brief Index of corner with matching vertex or -1 if not found. */
	int IndexOfCornerWithVertex(unsigned short vertex) const;
	
	
	
	/** \brief Face center. */
	inline const decVector &GetCenter() const{return pCenter;}
	
	/** \brief Set face center. */
	void SetCenter(const decVector &center);
	
	/** \brief Face normal. */
	inline const decVector &GetNormal() const{return pNormal;}
	
	/** \brief Set face normal. */
	void SetNormal(const decVector &normal);
	
	/** \brief Plane distance. */
	inline float GetDistance() const{return pDistance;}
	
	/** \brief Set plane distance. */
	void SetDistance(float distance);
	
	/** \brief Entry point. */
	inline const decVector &GetEntryPoint() const{return pEntryPoint;}
	
	/** \brief Set entry point. */
	void SetEntryPoint(const decVector &entryPoint);
	
	
	
	/** \brief Minimum extend. */
	inline const decVector &GetMinimumExtend() const{return pMinExtend;}
	
	/** \brief Maximum extend. */
	inline const decVector &GetMaximumExtend() const{return pMaxExtend;}
	
	/** \brief Set extends. */
	void SetExtends(const decVector &minExtend, const decVector &maxExtend);
	
	
	
	/** \brief Face is enabled for path finding. */
	inline bool GetEnabled() const{return pEnabled;}
	
	/** \brief Set if face is enabled for path finding. */
	void SetEnabled(bool enabled);
	
	
	
	/** \brief Path parent face or \em NULL. */
	inline dedaiSpaceMeshFace *GetPathParent() const{return pPathParent;}
	
	/** \brief Set path parent face or \em NULL. */
	void SetPathParent(dedaiSpaceMeshFace *face);
	
	/** \brief F path cost. */
	inline float GetPathCostF() const{return pPathCostF;}
	
	/** \brief Set F path cost. */
	void SetPathCostF(float cost);
	
	/** \brief G path cost. */
	inline float GetPathCostG() const{return pPathCostG;}
	
	/** \brief Set G path cost. */
	void SetPathCostG(float cost);
	
	/** \brief H path cost. */
	inline float GetPathCostH() const{return pPathCostH;}
	
	/** \brief Set H path cost. */
	void SetPathCostH(float cost);
	
	/** \brief Path type. */
	inline ePathFindingTypes GetPathType() const{return pPathType;}
	
	/** \brief Set path type. */
	void SetPathType(ePathFindingTypes type);
	
	/** \brief Clear path information. */
	void ClearPath();
	/*@}*/
};

#endif
