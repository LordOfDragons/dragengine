/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCENVMAPPROBE_H_
#define _IGDEGDCENVMAPPROBE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/deObject.h>



/**
 * \brief Game Definition Class Environment Map Probe.
 */
class DE_DLL_EXPORT igdeGDCEnvMapProbe : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		epInfluenceArea,
		epInfluenceBorderSize,
		epInfluencePriority,
		epReflectionShape,
		epReflectionMask,
		epAttachPosition,
		epAttachRotation
	};
	
	
	
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	
	decShapeList pShapeListInfluence;
	decShape *pShapeReflection;
	decShapeList pShapeListReflectionMask;
	float pInfluenceBorderSize;
	int pInfluencePriority;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition class environment map probe. */
	igdeGDCEnvMapProbe();
	
	/** \brief Create copy of a game definition class environment map probe. */
	igdeGDCEnvMapProbe( const igdeGDCEnvMapProbe &probe );
	
	/** \brief Clean up game definition class environment map probe. */
	virtual ~igdeGDCEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Influence shape list. */
	inline const decShapeList &GetShapeListInfluence() const{ return pShapeListInfluence; }
	
	/** \brief Set influence shape list. */
	void SetShapeListInfluence( const decShapeList &shapeList );
	
	/** \brief Reflection shape or NULL if the environment map is global. */
	inline decShape *GetShapeReflection() const{ return pShapeReflection; }
	
	/** \brief Set reflection shape or NULL if the environment map is global. */
	void SetShapeReflection( decShape *shape );
	
	/** \brief Reflection mask shape list. */
	inline const decShapeList &GetShapeListReflectionMask() const{ return pShapeListReflectionMask; }
	
	/** \brief Set reflection mask shape list. */
	void SetShapeListReflectionMask( const decShapeList &shapeList );
	
	/** \brief Influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	/** \brief Influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	
	/** \brief Set influence priority. */
	void SetInfluencePriority( int priority );
	
	/** \brief Determines if a property name is set or not (empty string). */
	bool IsPropertySet( int property ) const;
	
	/** \brief Name of the given property. */
	const decString &GetPropertyName( int property ) const;
	
	/** \brief Set name of the given property. */
	void SetPropertyName( int property, const char *name );
	
	/** \brief Determines if one or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
