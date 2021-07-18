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

#ifndef _DEEFFECTCOLORMATRIX_H_
#define _DEEFFECTCOLORMATRIX_H_

#include "deEffect.h"
#include "../../common/math/decMath.h"


/**
 * \brief Effect Color Matrix Class.
 *
 * Color Matrix effects are post processing effects that manipulate
 * the final image after rendering. Every pixel is transformed by
 * a given color matrix. This transformation is a linear transformation.
 * This effect is best used as the last effect in an effect chain since
 * most graphic modules can carry out such a transform as part of the
 * final image rendering in hardware.
 */
class deEffectColorMatrix : public deEffect{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deEffectColorMatrix> Ref;
	
	
	
private:
	decColorMatrix pColorMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new color matrix effect. */
	deEffectColorMatrix( deEffectManager *manager );
	
protected:
	/**
	 * \brief Clean up color matrix effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deEffectColorMatrix();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Color matrix. */
	inline const decColorMatrix &GetColorMatrix() const{ return pColorMatrix; }
	
	/** \brief Set color matrix. */
	void SetColorMatrix( const decColorMatrix &colorMatrix );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deEffectVisitor &visitor );
	/*@}*/
};

#endif
