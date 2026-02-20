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

#ifndef _DEBPMODEL_H_
#define _DEBPMODEL_H_

#include "debpBulletShapeModel.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsModel.h>

class deModel;
class deModelWeight;
class debpModelOctree;
class dePhysicsBullet;


/**
 * \brief Bullet Physics Model Peer
 * 
 * The peer for model resources in the ODE Physics Module. The main
 * purpose of this class is to provide an octree of faces for quick
 * collision detection if the model is a simple model. Complex models
 * have to be stored inside the Component Peer. Simple models are
 * much quicker as they do not change over time. A model is considered
 * simple if it has no weights that could influence the vertices.
 */
class debpModel : public deBasePhysicsModel{
public:
	struct sExtends{
		decVector minimum;
		decVector maximum;
	};
	
	struct sWeightSet{
		const deModelWeight *first;
		int count;
	};
	
	
	
private:
	dePhysicsBullet &pBullet;
	deModel &pModel;
	
	debpModelOctree *pOctree;
	bool pCanDeform;
	
	decTList<sWeightSet> pWeightSets;
	sExtends pExtends;
	sExtends pWeightlessExtends;
	bool pHasWeightlessExtends;
	decTList<sExtends> pBoneExtends;
	
	decTList<decVector> pNormals;
	decTList<float> pFaceProbabilities;
	
	debpBulletShapeModel::Ref pBulletShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model peer. */
	debpModel(dePhysicsBullet &bullet, deModel &model);
	
	/** \brief Clean up model peer. */
	~debpModel() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine resource. */
	inline deModel &GetModel() const{ return pModel; }
	
	
	
	/** \brief Octree or \em NULL if not prepared. */
	inline debpModelOctree *GetOctree() const{ return pOctree; }
	
	/** \brief Prepare octree if not ready yet. */
	void PrepareOctree();
	
	
	
	/** \brief Weight sets. */
	inline const sWeightSet *GetWeightSets() const{ return pWeightSets.GetArrayPointer(); }
	
	/** \brief Weight set count. */
	inline int GetWeightSetCount() const{ return pWeightSets.GetCount(); }
	
	/** \brief Extends. */
	inline const sExtends &GetExtends() const{ return pExtends; }
	
	/** \brief Weightless extends. */
	inline const sExtends &GetWeightlessExtends() const{ return pWeightlessExtends; }
	
	/** \brief Has weightless extends. */
	inline bool GetHasWeightlessExtends() const{ return pHasWeightlessExtends; }
	
	/** \brief Bone extends. */
	inline const sExtends *GetBoneExtends() const{ return pBoneExtends.GetArrayPointer(); }
	
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneExtends.GetCount(); }
	
	/** \brief Model can deform. */
	inline bool GetCanDeform() const{ return pCanDeform; }
	
	
	
	/** \brief Normals or \em NULL if not prepared. */
	inline const decVector *GetNormals() const{ return pNormals.GetArrayPointer(); }
	
	/** \brief Prepare normals if not prepared. */
	void PrepareNormals();
	
	
	
	/** \brief Face probabilities or \em NULL if not prepared. */
	inline const float *GetFaceProbabilities() const{ return pFaceProbabilities.GetArrayPointer(); }
	
	/** \brief Index of face containing probability or -1 if not found or not prepared yet. */
	int IndexOfFaceWithProbability(float probability) const;
	
	/** \brief Prepare face probabilities if not prepared. */
	void PrepareFaceProbabilities();
	
	
	
	/** \brief Shared bullet collision shape or \em NULL if not prepared. */
	inline const debpBulletShapeModel::Ref &GetShape() const{ return pBulletShape; }
	
	/** \brief Prepare shared bullet collision shape if not prepared. */
	void PrepareShape();
	/*@}*/
	
	
	
private:
	void pCheckCanDeform();
	void pCalculateExtends();
};

#endif
