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

#ifndef _DEOGLRESOURCES_H_
#define _DEOGLRESOURCES_H_

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>


class deGraphicOpenGl;


/**
 * Manages resources loaded from the shared module directory.
 */
class deoglResources{
private:
	deGraphicOpenGl &pOgl;
	
	deVideo::Ref pVideoCompileShader;
	deVideoPlayer::Ref pVideoPlayerCompileShader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create resources. */
	deoglResources( deGraphicOpenGl &ogl );
	
	/** Clean up resources. */
	~deoglResources();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
};

#endif
