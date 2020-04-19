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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeOCForceField.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCForceField
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCForceField::gdeOCForceField() :
pRadius( 1.0f ),
pExponent( 1.0f ),
pFieldType( deForceField::eftRadial ),
pApplicationType( deForceField::eatDirect ),
pForce( 1.0f ),
pFluctuationDirection( 0.0f ),
pFluctuationForce( 0.0f ),
pEnabled( true ){
}

gdeOCForceField::gdeOCForceField( const gdeOCForceField &field ) :
pPosition( field.pPosition ),
pRotation( field.pRotation ),
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
	for( i=etEnabled; i<=etEnabled; i++ ){
		pTriggerNames[ i ] = field.pTriggerNames[ i ];
	}
}

gdeOCForceField::~gdeOCForceField(){
}



// Management
///////////////

void gdeOCForceField::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCForceField::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCForceField::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void gdeOCForceField::SetInfluenceArea( const decShapeList &area ){
	pInfluenceArea = area;
}

void gdeOCForceField::SetRadius( float radius ){
	pRadius = decMath::max( radius, 0.0f );
}

void gdeOCForceField::SetExponent( float exponent ){
	pExponent = decMath::max( exponent, 0.0f );
}

void gdeOCForceField::SetFieldType( deForceField::eFieldTypes type ){
	pFieldType = type;
}

void gdeOCForceField::SetApplicationType( deForceField::eApplicationTypes type ){
	pApplicationType = type;
}

void gdeOCForceField::SetDirection( const decVector &direction ){
	pDirection = direction;
}

void gdeOCForceField::SetForce( float force ){
	pForce = force;
}

void gdeOCForceField::SetFluctuationDirection( float fluctuation ){
	pFluctuationDirection = fluctuation;
}

void gdeOCForceField::SetFluctuationForce( float fluctuation ){
	pFluctuationForce = fluctuation;
}

void gdeOCForceField::SetShape( const decShapeList &shape ){
	pShape = shape;
}

void gdeOCForceField::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



bool gdeOCForceField::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCForceField::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCForceField::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCForceField::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	return false;
}



bool gdeOCForceField::IsTriggerSet( eTriggers trigger ) const{
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &gdeOCForceField::GetTriggerName( eTriggers trigger ) const{
	return pTriggerNames[ trigger ];
}

void gdeOCForceField::SetTriggerName( eTriggers trigger, const char *name ){
	pTriggerNames[ trigger ] = name;
}
