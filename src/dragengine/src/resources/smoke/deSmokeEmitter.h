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

#ifndef _DESMOKEEMITTER_H_
#define _DESMOKEEMITTER_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"

class deSmokeEmitterManager;
class deComponent;
class deSkin;
class deWorld;

class deBaseGraphicSmokeEmitter;
class deBasePhysicsSmokeEmitter;


/**
 * \brief Smoke Density Point.
 *
 * Density point of a smoke emitter. The radius parameter defines
 * the radius of the sphere across which the density is distributed
 * in a quadratic way.
 */
struct DE_DLL_EXPORT deSmokeDensityPoint{
	decVector position;
	float radius;
};


/**
 * \brief Smoke Emitter.
 *
 * A smoke emitter produces gazous volumetric objects influenced by
 * force fields or other physics objects. The gasous volume is
 * described using a list of density points. Each density point
 * represents a sphere of variable size. The density distribution
 * inside a sphere declines quadratic towards the sphere boundary.
 * The transparency of the sphere depends therefore on the radius.
 * The maximum radius parameter indicates the largest radius of a
 * sphere where the density is not equal to 0. Spheres with a larger
 * radius become invisible and are removed.
 */
class DE_DLL_EXPORT deSmokeEmitter : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSmokeEmitter> Ref;
	
	
private:
	decDVector pEmitterPosition;
	decQuaternion pEmitterOrientation;
	
	decDVector pVolumePosition;
	
	decVector pCastDirectionMin;
	decVector pCastDirectionMax;
	float pCastVelocityMin;
	float pCastVelocityMax;
	float pCastDensityMin;
	float pCastDensityMax;
	bool pEnableCasting;
	
	deComponent *pComponent;
	int pCastTexture;
	deSkin *pSkin;
	
	decCollisionFilter pCollisionFilter;
	
	decVector pGravity;
	bool pUseLocalGravity;
	
	deSmokeDensityPoint *pPoints;
	int pPointCount;
	
	deBaseGraphicSmokeEmitter *pPeerGraphic;
	deBasePhysicsSmokeEmitter *pPeerPhysics;
	
	deWorld *pParentWorld;
	deSmokeEmitter *pLLWorldPrev;
	deSmokeEmitter *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new particle system. */
	deSmokeEmitter( deSmokeEmitterManager *manager );
	
protected:
	/**
	 * \brief Clean up particle system.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSmokeEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Emitter position. */
	inline const decDVector &GetEmitterPosition() const{ return pEmitterPosition; }
	
	/** \brief Set emitter position. */
	void SetEmitterPosition( const decDVector &position );
	
	/** \brief Emitter orientation. */
	inline const decQuaternion &GetEmitterOrientation() const{ return pEmitterOrientation; }
	
	/** \brief Set emitter orientation. */
	void SetEmitterOrientation( const decQuaternion &orientation );
	
	/** \brief Volume position. */
	inline const decDVector &GetVolumePosition() const{ return pVolumePosition; }
	
	/** \brief Set volume position. */
	void SetVolumePosition( const decDVector &position );
	
	/** \brief Minimum cast direction deviation. */
	inline const decVector &GetMinCastDirection() const{ return pCastDirectionMin; }
	
	/** \brief Set minimum cast direction deviation. */
	void SetMinCastDirection( const decVector &deviation );
	
	/** \brief Maximum cast direction deviation. */
	inline const decVector &GetMaxCastDirection() const{ return pCastDirectionMax; }
	
	/** \brief Set maximum cast direction deviation. */
	void SetMaxCastDirection( const decVector &deviation );
	
	/** \brief Minimum cast velocity. */
	inline float GetMinCastVelocity() const{ return pCastVelocityMin; }
	
	/** \brief Set minimum cast velocity. */
	void SetMinCastVelocity( float velocity );
	
	/** \brief Maximum cast velocity. */
	inline float GetMaxCastVelocity() const{ return pCastVelocityMax; }
	
	/** \brief Set maximum cast velocity. */
	void SetMaxCastVelocity( float velocity );
	
	/** \brief Minimum cast density. */
	inline float GetMinCastDensity() const{ return pCastDensityMin; }
	
	/** \brief Set minimum cast density. */
	void SetMinCastDensity( float density );
	
	/** \brief Maximum cast density. */
	inline float GetMaxCastDensity() const{ return pCastDensityMax; }
	
	/** \brief Set maximum cast density. */
	void SetMaxCastDensity( float density );
	
	/** \brief Determines if casting particles is enabled. */
	inline bool GetEnableCasting() const{ return pEnableCasting; }
	
	/** \brief Sets if casting particles is enabled. */
	void SetEnableCasting( bool enable );
	
	/** \brief Component used for casting or NULL. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Set component used for casting or NULL. */
	void SetComponent( deComponent *component );
	
	/** \brief Texture to use from the component skin to cast particles from. */
	inline int GetCastTexture() const{ return pCastTexture; }
	
	/** \brief Set texture to use from the component skin to cast particles from. */
	void SetCastTexture( int texture );
	
	/** \brief Skin of the particle instances or NULL. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set mode of the particle instances or NULL. */
	void SetSkin( deSkin *skin );
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &collisionFilter );
	
	/** \brief Gravity in m/s^2. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/** \brief Set gravity in m/s^2. */
	void SetGravity( const decVector &gravity );
	
	/** \brief Local gravity has to be used instead of the world gravity. */
	inline bool GetUseLocalGravity() const{ return pUseLocalGravity; }
	
	/** \brief Set if local gravity has to be used instead of the world gravity. */
	void SetUseLocalGravity( bool useLocalGravity );
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Pointer to the points which can be NULL. */
	inline deSmokeDensityPoint *GetPoints() const{ return pPoints; }
	
	/** \brief Set points. */
	void SetPoints( deSmokeDensityPoint *points, int count );
	
	/** \brief Notifies the graphic peer that the points changed. */
	void NotifyPointsChanged();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicSmokeEmitter *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic( deBaseGraphicSmokeEmitter *peer );
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsSmokeEmitter *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics( deBasePhysicsSmokeEmitter *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous smoke emitter in the parent world linked list. */
	inline deSmokeEmitter *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next smoke emitter in the parent world linked list. */
	void SetLLWorldPrev( deSmokeEmitter *smokeEmitter );
	
	/** \brief Next smoke emitter in the parent world linked list. */
	inline deSmokeEmitter *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next smoke emitter in the parent world linked list. */
	void SetLLWorldNext( deSmokeEmitter *smokeEmitter );
	/*@}*/
};

#endif
