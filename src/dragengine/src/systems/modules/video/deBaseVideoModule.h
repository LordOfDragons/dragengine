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

#ifndef _DEBASEVIDEOMODULE_H_
#define _DEBASEVIDEOMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deBaseVideoDecoder;
class deBaseVideoAudioDecoder;
class deBaseVideoInfo;
class deVideo;


/**
 * \brief Base video module provding load/save support for videos.
 */
class DE_DLL_EXPORT deBaseVideoModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseVideoModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseVideoModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load video information from file. */
	virtual void InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info ) = 0;
	
	/**
	 * \brief Save video.
	 *
	 * You can be sure that the file provided has been rewinded prior to this function call.
	 */
	virtual void SaveVideo( decBaseFileWriter &writer, const deVideo &video ) = 0;
	
	/** \brief Create video decoder peer. */
	virtual deBaseVideoDecoder *CreateDecoder( decBaseFileReader *reader ) = 0;
	
	/**
	 * \brief Create video audio decoder peer.
	 * 
	 * If no video audio is present or module does not support audio null is returned..
	 */
	virtual deBaseVideoAudioDecoder *CreateAudioDecoder( decBaseFileReader *reader ) = 0;
	/*@}*/
};

#endif
