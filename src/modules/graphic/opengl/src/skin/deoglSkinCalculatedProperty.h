/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSKINCALCULATEDPROPERTY_H_
#define _DEOGLSKINCALCULATEDPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>


class deoglSkinState;
class deoglSkinStateCalculated;
class deoglRComponent;

class decCurveBezierEvaluator;


/**
 * Skin calculated property.
 */
class deoglSkinCalculatedProperty : public deObject{
private:
	deSkinPropertyMapped::cComponent pMappedComponent[ 4 ];
	decCurveBezierEvaluator *pMappedComponentEvaluator[ 4 ];
	bool pCopyFromFirst[ 3 ];
	bool pRequiresTexture;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin calculated property. */
	deoglSkinCalculatedProperty();
	
protected:
	/** Cleans up the skin calculated property. */
	virtual ~deoglSkinCalculatedProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Mapped component. */
	deSkinPropertyMapped::cComponent &GetMappedComponent( int index );
	const deSkinPropertyMapped::cComponent &GetMappedComponent( int index ) const;
	
	/** Requires texture. */
	inline bool GetRequiresTexture() const{ return pRequiresTexture; }
	
	/** Set if texture is required. */
	void SetRequiresTexture( bool requiresTexture );
	
	/** Prepare. */
	void Prepare();
	
	/** Calculated value. */
	decColor Calculate( const deoglSkinState &skinState, const deoglSkinStateCalculated &calculated );
	/*@}*/
	
	
	
private:
	float pCalculateInputValue( int index, const deoglSkinState &skinState,
		const deoglSkinStateCalculated &calculated );
	float pCalculateOutputValue( int index, float inputValue );
};

#endif
