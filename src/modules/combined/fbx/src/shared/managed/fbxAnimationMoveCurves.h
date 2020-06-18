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

#ifndef _FBXANIMATIONMOVECURVES_H_
#define _FBXANIMATIONMOVECURVES_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxAnimationMove;
class fbxNode;
class fbxScene;
class fbxRigBone;

class deBaseModule;


/**
 * \brief FBX managed model cluster.
 */
class fbxAnimationMoveCurves : public deObject{
private:
	fbxAnimationMove &pMove;
	fbxNode &pNodeCurves;
	int64_t pNodeCurvesID;
	fbxNode *pNodeModel;
	int64_t pNodeModelID;
	decObjectOrderedSet pNodesCurve;
	
	decString pBoneName;
	decString pChannelName;
	fbxRigBone *pRigBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationMoveCurves( fbxAnimationMove &move, fbxNode &nodeCurves );
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationMoveCurves();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Model. */
	inline fbxAnimationMove &GetMove() const{ return pMove; }
	
	/** \brief Curves node. */
	inline fbxNode &GetNodeCurves() const{ return pNodeCurves; }
	
	/** \brief Node curves ID. */
	inline int64_t GetNodeCurvesID() const{ return pNodeCurvesID; }
	
	/** \brief Model node. */
	inline fbxNode *GetNodeModel() const{ return pNodeModel; }
	
	/** \brief Model node ID. */
	inline int64_t GetNodeModelID() const{ return pNodeModelID; }
	
	/** \brief Channel name. */
	inline const decString &GetChannelName() const{ return pChannelName; }
	
	/** \brief Set channel name. */
	void SetChannelName( const char *name );
	
	/** \brief Bone name. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set bone name. */
	void SetBoneName( const char *name );
	
	/** \brief Rig bone or NULL. */
	inline fbxRigBone *GetRigBone() const{ return pRigBone; }
	
	/** \brief Set rig bone or NULL. */
	void SetRigBone( fbxRigBone *bone );
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
