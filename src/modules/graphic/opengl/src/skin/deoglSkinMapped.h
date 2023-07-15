/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINMAPPED_H_
#define _DEOGLSKINMAPPED_H_

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/skin/deSkinMapped.h>


class deoglSkinState;
class deoglSkinStateMapped;


/**
 * Skin mapped.
 */
class deoglSkinMapped : public deObject{
public:
	typedef deTObjectReference<deoglSkinMapped> Ref;
	
private:
	const deSkinMapped &pMapped;
	decCurveBezierEvaluator pCurveEvaluator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin mapped. */
	deoglSkinMapped( const deSkinMapped &mapped );
	
protected:
	/** Clean up skin mapped. */
	virtual ~deoglSkinMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Skin mapped. */
	const deSkinMapped &GetMapped() const{ return pMapped; }
	
	/** Mapped value. */
	float Calculate( const deoglSkinState &skinState, const deoglSkinStateMapped &mapped ) const;
	/*@}*/
	
	
	
private:
	float pCalculateInputValue( const deoglSkinState &skinState, const deoglSkinStateMapped &mapped ) const;
	float pCalculateOutputValue( float inputValue ) const;
};

#endif
