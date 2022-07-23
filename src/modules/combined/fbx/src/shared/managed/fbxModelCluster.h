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

#ifndef _FBXMODELCLUSTER_H_
#define _FBXMODELCLUSTER_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxModel;
class fbxNode;
class fbxScene;

class deBaseModule;
class fbxRigBone;


/**
 * \brief FBX managed model cluster.
 */
class fbxModelCluster : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxModelCluster> Ref;
	
	
	
private:
	fbxModel &pModel;
	fbxNode &pNodeCluster;
	int64_t pNodeClusterID;
	fbxRigBone *pRigBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxModelCluster( fbxModel &model, fbxNode &nodeCluser );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxModelCluster();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Model. */
	inline fbxModel &GetModel() const{ return pModel; }
	
	/** \brief Cluster node. */
	inline fbxNode &GetNodeCluster() const{ return pNodeCluster; }
	
	/** \brief Cluster node ID. */
	inline int64_t GetNodeClusterID() const{ return pNodeClusterID; }
	
	/** \brief Rig bone or NULL. */
	inline fbxRigBone *GetRigBone() const{ return pRigBone; }
	
	/** \brief Set rig bone or NULL. */
	void SetRigBone( fbxRigBone *rigBone );
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
