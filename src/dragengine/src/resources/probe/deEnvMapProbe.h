/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEENVMAPPROBE_H_
#define _DEENVMAPPROBE_H_

#include "../deResource.h"
#include "../image/deImageReference.h"
#include "../../common/math/decMath.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/string/decString.h"
#include "../../common/utils/decLayerMask.h"

class deEnvMapProbeManager;
class deBaseGraphicEnvMapProbe;
class deWorld;


/**
 * \brief Environment Map Probe Resource.
 * 
 * Environment map probes tell the Graphic Module about a good place for a local environment
 * map probe. Environment map probes define a shape list of influence, a shape list of
 * reflection border, an influence border size and an influence priority. The influence shape
 * list defines the shape inside which this environment map probe is best used. The influence
 * border size defines the distance in meters form the border of the influence shape list
 * where the influence gradually fades. Inside this border the influence is at the maximum.
 * If multiple environment map probes overlap with full influence the influence priority
 * defines which environment map wins over the others. The environment map probe with the
 * highest influence priorty receives the full influence. All others are blended accordingly.
 * This allows to place smaller environment maps inside larger ones to refine the result in
 * places where a more narrow sampling is useful without having to construct complex influence
 * shape lists excluding each other properly.The reflection border shape list defines the
 * shape where the reflection maps to. Typically this is a box and should line up with the
 * reflected map geometry as well as possible. Avoid complex shape lists for the reflection
 * border as this is usually better for Graphic Modules to handle.
 */
class deEnvMapProbe : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEnvMapProbe> Ref;
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	
	decShapeList pShapeListInfluence;
	decShape *pShapeReflection;
	decShapeList pShapeListReflectionMask;
	float pInfluenceBorderSize;
	int pInfluencePriority;
	
	decLayerMask pLayerMask;
	
	deImageReference pImage;
	
	deWorld *pParentWorld;
	deEnvMapProbe *pLLWorldPrev;
	deEnvMapProbe *pLLWorldNext;
	
	deBaseGraphicEnvMapProbe *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new environment map probe. */
	deEnvMapProbe( deEnvMapProbeManager *manager );
	
protected:
	/**
	 * \brief Clean up environment map probe.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEnvMapProbe();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Influence shape list. */
	inline decShapeList &GetShapeListInfluence(){ return pShapeListInfluence; }
	inline const decShapeList &GetShapeListInfluence() const{ return pShapeListInfluence; }
	
	/** \brief Notifies the peers that the influence shape list changed. */
	void NotifyShapeListInfluenceChanged();
	
	/** \brief Reflection shape or NULL if the environment map is global. */
	inline decShape *GetShapeReflection() const{ return pShapeReflection; }
	
	/** \brief Set reflection shape or NULL if the environment map is global. */
	void SetShapeReflection( decShape *shape );
	
	/** \brief Reflection mask shape list. */
	inline decShapeList &GetShapeListReflectionMask(){ return pShapeListReflectionMask; }
	inline const decShapeList &GetShapeListReflectionMask() const{ return pShapeListReflectionMask; }
	
	/** \brief Notifies the peers that the reflection shape changed. */
	void NotifyShapeReflectionChanged();
	
	/** \brief Influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	/** \brief Influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	
	/** \brief Set influence priority. */
	void SetInfluencePriority( int priority );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Explicit environment map image to use or NULL to auto-generate. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set explicit environment map image to use or NULL to auto-generate. */
	void SetImage( deImage *image );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphics system peer. */
	inline deBaseGraphicEnvMapProbe *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphics system peer. */
	void SetPeerGraphic( deBaseGraphicEnvMapProbe *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous environment map probe in the parent world linked list. */
	inline deEnvMapProbe *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next environment map probe in the parent world linked list. */
	void SetLLWorldPrev( deEnvMapProbe *instance );
	
	/** \brief Next environment map probe in the parent world linked list. */
	inline deEnvMapProbe *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next environment map probe in the parent world linked list. */
	void SetLLWorldNext( deEnvMapProbe *instance );
	/*@}*/
};

#endif
