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

#ifndef _DEBASEIMAGEMODULE_H_
#define _DEBASEIMAGEMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deBaseImageInfo;
class deImage;


/**
 * \brief Base image module providing load/save support for images.
 */
class deBaseImageModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseImageModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseImageModule();
	/*@}*/
	
	
	
	/** \name Management */
	/**
	 * \brief Quick check if the file can be loaded by this module.
	 * 
	 * This is usually a header check and is used to detect
	 * with what image module a file could be possibly loaded if
	 * it does not match any patterns. If the image can be loaded
	 * store the necessary informations into an ImageInfo object and
	 * return it. Otherwise return NULL. The returned ImageInfo object
	 * is used to create the image and as parameter to LoadImage.
	 * You can be sure that the file pointer is located at the
	 * beginning of the file.
	 */
	virtual deBaseImageInfo *InitLoadImage( decBaseFileReader &reader ) = 0;
	
	/**
	 * \brief Load image.
	 * 
	 * You can be sure that the file pointer is at the same place left off
	 * after the prior call of CheckLoadImage on the same file and module.
	 * You can be sure that the image object specified in the function
	 * call has been created with the information stored in the infos
	 * parameter obtained from a prior call to the CheckLoadImage function.
	 */
	virtual void LoadImage( decBaseFileReader &reader, deImage &image, deBaseImageInfo &infos ) = 0;
	
	/**
	 * \brief Save image.
	 * 
	 * You can be sure that the file provided has been rewinded prior to this
	 * function call.
	 */
	virtual void SaveImage( decBaseFileWriter &writer, const deImage &image ) = 0;
	/*@}*/
};

#endif
