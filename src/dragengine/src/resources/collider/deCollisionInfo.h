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

#ifndef _DECOLLISIONINFO_H_
#define _DECOLLISIONINFO_H_

#include "../particle/deParticleEmitterType.h"
#include "../../common/math/decMath.h"
#include "../../deObject.h"

#include <dragengine/resources/collider/deCollider.h>
#include "../terrain/heightmap/deHeightTerrain.h"
class deHeightTerrainSector;


/**
 * \brief Collision Information.
 * 
 * Stores information about a collision between a collider and a height terrain or another
 * collider. This object is reference counted because it does keep references to the
 * collision objects.
 */
class DE_DLL_EXPORT deCollisionInfo : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCollisionInfo> Ref;
	
	
private:
	int pOwnerBone;
	int pOwnerShape;
	int pOwnerFace;
	
	deHeightTerrain::Ref pHeightTerrain;
	deHeightTerrainSector *pHTSector;
	deCollider::Ref pCollider;
	int pBone;
	int pShape;
	int pFace;
	
	float pParticleLifetime;
	float pParticleMass;
	decDVector pParticlePosition;
	decVector pParticleVelocity;
	deParticleEmitterType::eCollisionResponses pParticleResponse;
	
	float pDistance;
	decVector pNormal;
	decDVector pPosition;
	float pImpulse;
	
	bool pStopTesting;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collision info. */
	deCollisionInfo();
	
	/** \brief Create copy of collision info. */
	deCollisionInfo(const deCollisionInfo &info);
	
protected:
	/**
	 * \brief Clean up collision info.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deCollisionInfo() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Owner bone or -1 if not applicable. */
	inline int GetOwnerBone() const{ return pOwnerBone; }
	
	/** \brief Set owner bone or -1 if not applicable. */
	void SetOwnerBone(int bone);
	
	/** \brief Owner shape or -1 if not applicable. */
	inline int GetOwnerShape() const{ return pOwnerShape; }
	
	/** \brief Set owner shape or -1 if not applicable. */
	void SetOwnerShape(int shape);
	
	/** \brief Owner face or -1 if not applicable. */
	inline int GetOwnerFace() const{ return pOwnerFace; }
	
	/** \brief Set owner face or -1 if not applicable. */
	void SetOwnerFace(int face);
	
	/** \brief Height terrain or NULL if none is hit. */
	inline const deHeightTerrain::Ref &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Height terrain sector or NULL if none is hit. */
	inline deHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** \brief Collider or NULL if none is hit. */
	inline const deCollider::Ref &GetCollider() const{ return pCollider; }
	
	/**
	 * \brief Bone index or -1 if none is hit.
	 * 
	 * The bone is set if a collider is hit and the collider has bone
	 * information. In all other cases this is -1.
	 */
	inline int GetBone() const{ return pBone; }
	
	/**
	 * \brief Face index or -1 if none is hit.
	 * 
	 * The face is set if a component is hit which has a model assigned
	 * to it. In all other cases this is -1.
	 */
	inline int GetFace() const{ return pFace; }
	
	/**
	 * \brief Shape index or -1 if none is hit.
	 * 
	 * The shape is set if a collider is hit (either rig or bone) and the
	 * collider rig or bone has shapes. In all other cases this is -1.
	 */
	inline int GetShape() const{ return pShape; }
	
	/** \brief Height terrain sector is set. */
	bool IsHTSector() const;
	
	/** \brief Collider is set. */
	bool IsCollider() const;
	
	/** \brief Collision found. */
	bool HasCollision() const;
	
	/** \brief Set height terrain sector. */
	void SetHTSector(deHeightTerrain *heightTerrain, deHeightTerrainSector *sector);
	
	/**
	 * \brief Set collider.
	 * \param[in] bone Index of the hit bone if otherwise -1.
	 * \param[in] shape Index of hit rig or bone shape otherwsie -1.
	 * \param[in] face Index of hit face otherwise -1.
	 */
	void SetCollider(deCollider *collider, int bone, int shape, int face);
	
	/** \brief Clear collision. */
	void Clear();
	
	
	
	/** \brief Particle lifetime. */
	inline float GetParticleLifetime() const{ return pParticleLifetime; }
	
	/** \brief Set particle lifetime. */
	void SetParticleLifetime(float lifetime);
	
	/** \brief Particle mass. */
	inline float GetParticleMass() const{ return pParticleMass; }
	
	/** \brief Set particle mass. */
	void SetParticleMass(float mass);
	
	/** \brief Particle position. */
	inline const decDVector &GetParticlePosition() const{ return pParticlePosition; }
	
	/** \brief Set particle position. */
	void SetParticlePosition(const decDVector &position);
	
	/** \brief Particle velocity. */
	inline const decVector &GetParticleVelocity() const{ return pParticleVelocity; }
	
	/** \brief Set particle velocity. */
	void SetParticleVelocity(const decVector &velocity);
	
	/** \brief Collision response to apply to particle. */
	inline deParticleEmitterType::eCollisionResponses GetParticleResponse() const{ return pParticleResponse; }
	
	/** \brief Set collision response to apply to particle. */
	void SetParticleResponse(deParticleEmitterType::eCollisionResponses respone);
	
	
	
	/**
	 * \brief Percentage distance or remaining step time.
	 * 
	 * For explicite collision tests with a displacement vector the distance is
	 * the percentage of the displacement travelled before the collision occurred.
	 * For physical collision detection as carried out by the physics module this
	 * is the remaining movement time after the collision point.
	 */
	inline float GetDistance() const{ return pDistance; }
	
	/**
	 * \brief Set percentage distance or remaining step time.
	 * 
	 * For explicite collision tests with a displacement vector the distance is the
	 * percentage of the displacement travelled before the collision occurred. For
	 * physical collision detection as carried out by the physics module this is the
	 * remaining movement time after the collision point.
	 */
	void SetDistance(float distance);
	
	/** \brief Hit normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set hit normal. */
	void SetNormal(const decVector &normal);
	
	/** \brief Hit position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set hit position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Hit impulse. */
	inline float GetImpulse() const{ return pImpulse; }
	
	/** \brief Set hit impulse. */
	void SetImpulse(float impulse);
	
	
	
	/** \brief Collision detection has to be stopped. */
	inline bool GetStopTesting() const{ return pStopTesting; }
	
	/** \brief Set if collision detection has to be stopped. */
	void SetStopTesting(bool stopTesting);
	
	
	
	/** \name Operator */
	/*@{*/
	/** \brief Copy from collision information. */
	deCollisionInfo &operator=(const deCollisionInfo &info);
	/*@}*/
};

#endif
