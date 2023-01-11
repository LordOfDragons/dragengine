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

#ifndef _DEOGLSHAREDSPBLISTSSBO_H_
#define _DEOGLSHAREDSPBLISTSSBO_H_

#include "deoglSharedSPBList.h"

class deoglSPBlockSSBO;



/**
 * \brief OpenGL shared shader parameter block List using SSBO as backing storage.
 */
class deoglSharedSPBListSSBO : public deoglSharedSPBList{
private:
	deoglSPBlockSSBO &pLayoutSSBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared shader parameter block list. */
	deoglSharedSPBListSSBO( deoglRenderThread &renderThread, deoglSPBlockSSBO *layout );
	/*@}*/
	
	
	
protected:
	deoglShaderParameterBlock::Ref pCreateBlock() const;
};

#endif
