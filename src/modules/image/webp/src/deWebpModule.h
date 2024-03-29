/* 
 * Drag[en]gine Webp Image Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWEBPMODULE_H_
#define _DEWEBPMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/image/deBaseImageModule.h>


/**
 * Webp image module.
 */
class deWebpModule : public deBaseImageModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deWebpModule( deLoadableModule &loadableModule );
	
	/** Clean up module. */
	virtual ~deWebpModule();
	/*@}*/
	
	
	
	/** \name Management */
	/** Quick check if the file can be loaded by this module. */
	virtual deBaseImageInfo *InitLoadImage( decBaseFileReader &reader );
	
	/** Load image. */
	virtual void LoadImage( decBaseFileReader &reader, deImage &image, deBaseImageInfo &infos );
	
	/** Save image. */
	virtual void SaveImage( decBaseFileWriter &writer, const deImage &image );
	/*@}*/
};

#endif
