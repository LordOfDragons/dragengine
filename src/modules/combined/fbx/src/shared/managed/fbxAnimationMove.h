/* 
 * FBX Modules
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

#ifndef _FBXANIMATIONMOVE_H_
#define _FBXANIMATIONMOVE_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxAnimation;
class fbxAnimationMoveCurves;
class fbxNode;
class fbxScene;

class deBaseModule;


/**
 * \brief FBX managed model cluster.
 */
class fbxAnimationMove : public deObject{
private:
	fbxAnimation &pAnimation;
	fbxNode &pNodeStack;
	int64_t pNodeStackID;
	fbxNode *pNodeLayer;
	int64_t pNodeLayerID;
	decObjectOrderedSet pCurveNodes;
	
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationMove( fbxAnimation &animation, fbxNode &nodeStack );
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationMove();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Animation. */
	inline fbxAnimation &GetAnimation() const{ return pAnimation; }
	
	/** \brief Stack node. */
	inline fbxNode &GetNodeStack() const{ return pNodeStack; }
	
	/** \brief Node stack ID. */
	inline int64_t GetNodeStackID() const{ return pNodeStackID; }
	
	/** \brief Node layer. */
	inline fbxNode &GetNodeLayer() const{ return *pNodeLayer; }
	
	/** \brief Node layer ID. */
	inline int64_t GetNodeLayerID() const{ return pNodeLayerID; }
	
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set bone name. */
	void SetName( const char *name );
	
	
	
	/** \brief Count of curve nodes. */
	int GetCurvesCount() const;
	
	/** \brief Curves at index. */
	fbxAnimationMoveCurves *GetCurvesAt( int index ) const;
	
	/** \brief Match curves against rig. */
	void MatchRig( const fbxRig &rig );
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
