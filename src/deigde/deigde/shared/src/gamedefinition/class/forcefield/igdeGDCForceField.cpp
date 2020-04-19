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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDCForceField.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCForceField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCForceField::igdeGDCForceField() :
pRadius( 1.0f ),
pExponent( 1.0f ),
pFieldType( deForceField::eftRadial ),
pApplicationType( deForceField::eatDirect ),
pForce( 1.0f ),
pFluctuationDirection( 0.0f ),
pFluctuationForce( 0.0f ),
pEnabled( true ){
}

igdeGDCForceField::igdeGDCForceField( const igdeGDCForceField &field ) :
pPosition( field.pPosition ),
pOrientation( field.pOrientation ),
pBoneName( field.pBoneName ),
pInfluenceArea( field.pInfluenceArea ),
pRadius( field.pRadius ),
pExponent( field.pExponent ),
pFieldType( field.pFieldType ),
pApplicationType( field.pApplicationType ),
pDirection( field.pDirection ),
pForce( field.pForce ),
pFluctuationDirection( field.pFluctuationDirection ),
pFluctuationForce( field.pFluctuationForce ),
pShape( field.pShape ),
pEnabled( field.pEnabled )
{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = field.pPropertyNames[ i ];
	}
	for( i=0; i<=etEnabled; i++ ){
		pTriggerNames[ i ] = field.pTriggerNames[ i ];
	}
}

igdeGDCForceField::~igdeGDCForceField(){
}



// Management
///////////////

void igdeGDCForceField::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCForceField::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCForceField::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void igdeGDCForceField::SetInfluenceArea( const decShapeList &area ){
	pInfluenceArea = area;
}

void igdeGDCForceField::SetRadius( float radius ){
	pRadius = decMath::max( radius, 0.0f );
}

void igdeGDCForceField::SetExponent( float exponent ){
	pExponent = decMath::max( exponent, 0.0f );
}

void igdeGDCForceField::SetFieldType( deForceField::eFieldTypes type ){
	pFieldType = type;
}

void igdeGDCForceField::SetApplicationType( deForceField::eApplicationTypes type ){
	pApplicationType = type;
}

void igdeGDCForceField::SetDirection( const decVector &direction ){
	pDirection = direction;
}

void igdeGDCForceField::SetForce( float force ){
	pForce = force;
}

void igdeGDCForceField::SetFluctuationDirection( float fluctuation ){
	pFluctuationDirection = fluctuation;
}

void igdeGDCForceField::SetFluctuationForce( float fluctuation ){
	pFluctuationForce = fluctuation;
}

void igdeGDCForceField::SetShape( const decShapeList &shape ){
	pShape = shape;
}

void igdeGDCForceField::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



bool igdeGDCForceField::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCForceField::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCForceField::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCForceField::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	return false;
}



bool igdeGDCForceField::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &igdeGDCForceField::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void igdeGDCForceField::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}
