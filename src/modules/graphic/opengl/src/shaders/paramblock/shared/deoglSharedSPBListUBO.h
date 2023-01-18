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

#ifndef _DEOGLSHAREDSPBLISTUBO_H_
#define _DEOGLSHAREDSPBLISTUBO_H_

#include "deoglSharedSPBList.h"

class deoglSPBlockUBO;



/**
 * OpenGL shared shader parameter block List using UBO as backing storage.
 */
class deoglSharedSPBListUBO : public deoglSharedSPBList{
private:
	deoglSPBlockUBO &pLayoutUBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared shader parameter block list. */
	deoglSharedSPBListUBO( deoglRenderThread &renderThread, deoglSPBlockUBO *layout );
	/*@}*/
	
	
	
protected:
	deoglShaderParameterBlock::Ref pCreateBlock() const;
};

#endif
