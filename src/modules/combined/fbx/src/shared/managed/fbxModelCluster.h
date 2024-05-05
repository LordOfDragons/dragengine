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
