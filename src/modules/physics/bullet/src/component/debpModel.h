/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPMODEL_H_
#define _DEBPMODEL_H_

#include <dragengine/systems/modules/physics/deBasePhysicsModel.h>

#include <dragengine/common/math/decMath.h>

class deModel;
class deModelWeight;
class debpModelOctree;
class dePhysicsBullet;
class debpBulletShapeModel;


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
	
	sWeightSet *pWeightSets;
	int pWeightSetCount;
	sExtends pExtends;
	sExtends pWeightlessExtends;
	bool pHasWeightlessExtends;
	sExtends *pBoneExtends;
	int pBoneCount;
	
	decVector *pNormals;
	float *pFaceProbabilities;
	
	debpBulletShapeModel *pBulletShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model peer. */
	debpModel( dePhysicsBullet &bullet, deModel &model );
	
	/** \brief Clean up model peer. */
	virtual ~debpModel();
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
	inline sWeightSet *GetWeightSets() const{ return pWeightSets; }
	
	/** \brief Weight set count. */
	inline int GetWeightSetCount() const{ return pWeightSetCount; }
	
	/** \brief Extends. */
	inline const sExtends &GetExtends() const{ return pExtends; }
	
	/** \brief Weightless extends. */
	inline const sExtends &GetWeightlessExtends() const{ return pWeightlessExtends; }
	
	/** \brief Has weightless extends. */
	inline bool GetHasWeightlessExtends() const{ return pHasWeightlessExtends; }
	
	/** \brief Bone extends. */
	inline const sExtends *GetBoneExtends() const{ return pBoneExtends; }
	
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Model can deform. */
	inline bool GetCanDeform() const{ return pCanDeform; }
	
	
	
	/** \brief Normals or \em NULL if not prepared. */
	inline const decVector *GetNormals() const{ return pNormals; }
	
	/** \brief Prepare normals if not prepared. */
	void PrepareNormals();
	
	
	
	/** \brief Face probabilities or \em NULL if not prepared. */
	inline const float *GetFaceProbabilities() const{ return pFaceProbabilities; }
	
	/** \brief Index of face containing probability or -1 if not found or not prepared yet. */
	int IndexOfFaceWithProbability( float probability ) const;
	
	/** \brief Prepare face probabilities if not prepared. */
	void PrepareFaceProbabilities();
	
	
	
	/** \brief Shared bullet collision shape or \em NULL if not prepared. */
	inline debpBulletShapeModel *GetShape() const{ return pBulletShape; }
	
	/** \brief Prepare shared bullet collision shape if not prepared. */
	void PrepareShape();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCheckCanDeform();
	void pCalculateExtends();
};

#endif
