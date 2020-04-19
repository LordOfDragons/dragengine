/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCENVMAPPROBE_H_
#define _GDEOCENVMAPPROBE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Object class environment map probe.
 */
class gdeOCEnvMapProbe : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Influence area. */
		epInfluenceArea,
		
		/** \brief Influence border size. */
		epInfluenceBorderSize,
		
		/** \brief Influence priority. */
		epInfluencePriority,
		
		/** \brief Reflection shape. */
		epReflectionShape,
		
		/** \brief Reflection mask. */
		epReflectionMask,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach orientation. */
		epAttachRotation
	};
	
	
	
private:
	decVector pPosition;
	decVector pRotation;
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
	/** \brief Create environment map probe. */
	gdeOCEnvMapProbe();
	
	/** \brief Create copy of environment map probe. */
	gdeOCEnvMapProbe( const gdeOCEnvMapProbe &probe );
	
	/** \brief Clean up environment map probe. */
	virtual ~gdeOCEnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set orientation. */
	void SetRotation( const decVector &orientation );
	
	/** \brief Scaling. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	
	
	/** \brief Influence shape list. */
	inline decShapeList &GetShapeListInfluence(){ return pShapeListInfluence; }
	inline const decShapeList &GetShapeListInfluence() const{ return pShapeListInfluence; }
	
	/** \brief Reflection shape or \em NULL if probe is global. */
	inline decShape *GetShapeReflection() const{ return pShapeReflection; }
	
	/** \brief Set reflection shape or \em NULL if probe is global. */
	void SetShapeReflection( decShape *shape );
	
	/** \brief Reflection mask shape list. */
	inline decShapeList &GetShapeListReflectionMask(){ return pShapeListReflectionMask; }
	inline const decShapeList &GetShapeListReflectionMask() const{ return pShapeListReflectionMask; }
	
	/** \brief Influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	/** \brief Influence priority. */
	inline int GetInfluencePriority() const{ return pInfluencePriority; }
	
	/** \brief Set influence priority. */
	void SetInfluencePriority( int priority );
	
	
	
	/** \brief Property name is set. */
	bool IsPropertySet( int property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( int property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( int property, const char *name );
	
	/** \brief One or more properties use a name. */
	bool HasPropertyWithName( const char *name ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
