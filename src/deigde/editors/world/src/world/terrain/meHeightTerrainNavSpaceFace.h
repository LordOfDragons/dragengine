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

#ifndef _MEHEIGHTTERRAINNAVSPACEFACE_H_
#define _MEHEIGHTTERRAINNAVSPACEFACE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>

class meHeightTerrainNavSpaceType;


/**
 * \brief Height terrain navigation space face.
 */
class meHeightTerrainNavSpaceFace : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainNavSpaceFace> Ref;
	
	
private:
	meHeightTerrainNavSpaceType *pType;
	decIntList pNavPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates height terrain navigation space face. */
	meHeightTerrainNavSpaceFace();
	
	/** \brief Clean up height terrain navigation space face. */
	virtual ~meHeightTerrainNavSpaceFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent type or \em NULL. */
	inline meHeightTerrainNavSpaceType *GetType() const{return pType;}
	
	/** \brief Set parent type or \em NULL. */
	void SetType(meHeightTerrainNavSpaceType *type);
	
	
	
	/** \brief Navigation points. */
	decIntList &GetNavPoints(){return pNavPoints;}
	const decIntList &GetNavPoints() const{return pNavPoints;}
	
	/** \brief Order navigation points clock-wise. */
	void OrderClockwise(int pointsPerRow);
	
	/** \brief Navigation points are matching order independent. */
	bool NavPointsMatch(const decIntList &navpoints) const;
	
	/** \brief Face has navigation point. */
	bool HasNavPoint(int navpoint) const;
	
	/** \brief Face has edge. */
	bool HasNavEdge(int navpoint1, int navpoint2) const;
	
	/** \brief All face navigation points are contained in a list in any order. */
	bool HasAllNavPointsIn(const decIntList &navpoints) const;
	/*@}*/
};

#endif
