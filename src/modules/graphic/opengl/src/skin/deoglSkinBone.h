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

#ifndef _DEOGLSKINBONE_H_
#define _DEOGLSKINBONE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoglSkinState;


/**
 * Skin bone.
 */
class deoglSkinBone : public deObject{
public:
	typedef deTObjectReference<deoglSkinBone> Ref;
	
private:
	const decString pName;
	int pBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin bone. */
	deoglSkinBone( const decString &name );
	
protected:
	/** Clean up skin bone. */
	virtual ~deoglSkinBone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Name. */
	const decString &GetName() const{ return pName; }
	
	/** Set bone index. */
	void SetBone( int bone );
	/*@}*/
};

#endif
