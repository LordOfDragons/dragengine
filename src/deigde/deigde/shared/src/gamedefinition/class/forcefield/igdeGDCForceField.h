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

#ifndef _IGDEGDCFORCEFIELD_H_
#define _IGDEGDCFORCEFIELD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/forcefield/deForceField.h>



/**
 * \brief Game Definition Class Force Field.
 */
class igdeGDCForceField : public deObject{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Influence area. */
		epInfluenceArea,
		
		/** \brief Radius. */
		epRadius,
		
		/** \brief Exponent. */
		epExponent,
		
		/** \brief Field type. */
		epFieldType,
		
		/** \brief Application type. */
		epApplicationType,
		
		/** \brief Direction. */
		epDirection,
		
		/** \brief Force. */
		epForce,
		
		/** \brief Fluctuation direction. */
		epFluctuationDirection,
		
		/** \brief Fluctuation force. */
		epFluctuationForce,
		
		/** \brief Shape. */
		epShape,
		
		/** \brief Enabled. */
		epEnabled,
		
		/** \brief Attach position. */
		epAttachPosition,
		
		/** \brief Attach rotation. */
		epAttachRotation
	};
	
	/** \brief Trigger. */
	enum eTriggers{
		/** \brief Enabled. */
		etEnabled
	};
	
	
	
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decString pBoneName;
	
	decShapeList pInfluenceArea;
	float pRadius;
	float pExponent;
	
	deForceField::eFieldTypes pFieldType;
	deForceField::eApplicationTypes pApplicationType;
	decVector pDirection;
	float pForce;
	float pFluctuationDirection;
	float pFluctuationForce;
	
	decShapeList pShape;
	bool pEnabled;
	
	decString pPropertyNames[ epAttachRotation + 1 ];
	decString pTriggerNames[ etEnabled + 1 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class force field. */
	igdeGDCForceField();
	
	/** \brief Create copy of a game definition class force field. */
	igdeGDCForceField( const igdeGDCForceField &field );
	
protected:
	/** \brief Clean up class force field. */
	virtual ~igdeGDCForceField();
	/*@}*/
	
	
	
public:
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
	
	/** \brief Name of the bone to attach to or empty string. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to or empty string. */
	void SetBoneName( const char *boneName );
	
	/** \brief Influence area. */
	inline const decShapeList &GetInfluenceArea() const{ return pInfluenceArea; }
	
	/** \brief Set influence area. */
	void SetInfluenceArea( const decShapeList &area );
	
	/** \brief Falloff radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set falloff radius. */
	void SetRadius( float radius );
	
	/** \brief Falloff exponent. */
	inline float GetExponent() const{ return pExponent; }
	
	/** \brief Set falloff exponent. */
	void SetExponent( float exponent );
	
	/** \brief Field type. */
	inline deForceField::eFieldTypes GetFieldType() const{ return pFieldType; }
	
	/** \brief Set field type. */
	void SetFieldType( deForceField::eFieldTypes type );
	
	/** \brief Force application type. */
	inline deForceField::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set application type. */
	void SetApplicationType( deForceField::eApplicationTypes type );
	
	/** \brief Force direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set force direction. */
	void SetDirection( const decVector &direction );
	
	/** \brief Force in newton. */
	inline float GetForce() const{ return pForce; }
	
	/** \brief Set force in newton. */
	void SetForce( float force );
	
	/** \brief Fluctuation of direction in radians. */
	inline float GetFluctuationDirection() const{ return pFluctuationDirection; }
	
	/** \brief Set fluctuation of direction in radians. */
	void SetFluctuationDirection( float fluctuation );
	
	/** \brief Fluctuation of force in newton. */
	inline float GetFluctuationForce() const{ return pFluctuationForce; }
	
	/** \brief Set fluctuation of force in newton. */
	void SetFluctuationForce( float fluctuation );
	
	/** \brief Sound shape. */
	inline const decShapeList &GetShape() const{ return pShape; }
	
	/** \brief Set sound shape. */
	void SetShape( const decShapeList &shape );
	
	/** \brief Force field is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if force field is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Property name is set (not empty string). */
	bool IsPropertySet( eProperties property ) const;
	
	/** \brief Name of property. */
	const decString &GetPropertyName( eProperties property ) const;
	
	/** \brief Set name of property. */
	void SetPropertyName( eProperties property, const char *name );
	
	/** \brief One or more properties use name. */
	bool HasPropertyWithName( const char *name ) const;
	
	
	
	/** \brief Trigger name is set (not empty string). */
	bool IsTriggerSet( eTriggers trigger ) const;
	
	/** \brief Name of trigger. */
	const decString &GetTriggerName( eTriggers trigger ) const;
	
	/** \brief Set name of trigger. */
	void SetTriggerName( eTriggers trigger, const char *name );
	/*@}*/
};

#endif
