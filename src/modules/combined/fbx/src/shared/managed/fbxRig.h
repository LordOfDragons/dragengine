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

#ifndef _FBXRIG_H_
#define _FBXRIG_H_


#include "../fbxNode.h"

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxScene;
class fbxRigBone;

class deBaseModule;


/**
 * \brief FBX managed rig.
 */
class fbxRig : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxRig> Ref;
	
	
	
private:
	fbxScene &pScene;
	const fbxNode::Ref pNodePose;
	decObjectOrderedSet pBones;
	decMatrix pMatrix;
	decMatrix pMatrixInverse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxRig( fbxScene &scene, fbxNode *nodePose );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxRig();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** \brief Pose node. */
	inline fbxNode &GetNodePose() const{ return pNodePose; }
	
	
	
	/** \brief Count of bones. */
	int GetBoneCount() const;
	
	/** \brief Bone at index. */
	fbxRigBone *GetBoneAt( int index ) const;
	
	/** \brief Named bone or NULL if absent. */
	fbxRigBone *GetBoneNamed( const char *name ) const;
	
	/** \brief Bone with model ID or NULL if absent. */
	fbxRigBone *GetBoneWithModelID( int64_t id ) const;
	
	
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decMatrix &GetMatrixInverse() const{ return pMatrix; }
	
	
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	void pAddRootBone( fbxScene &scene, fbxNode &nodeRoot );
};

#endif
