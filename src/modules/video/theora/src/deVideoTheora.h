/* 
 * Drag[en]gine Theora Video Module
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

#ifndef _DEVIDEOTHEORA_H_
#define _DEVIDEOTHEORA_H_

#include <dragengine/systems/modules/video/deBaseVideoModule.h>



/**
 * \brief Theora video module.
 */
class deVideoTheora : public deBaseVideoModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deVideoTheora( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deVideoTheora();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load video information from file. */
	virtual void InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info );
	
	/**
	 * \brief Save video.
	 *
	 * You can be sure that the file provided has been rewinded prior to this function call.
	 */
	virtual void SaveVideo( decBaseFileWriter &writer, const deVideo &video );
	
	/** \brief Create video decoder peer. */
	virtual deBaseVideoDecoder *CreateDecoder( decBaseFileReader *reader );
	
	/**
	 * \brief Create video audio decoder peer.
	 * 
	 * If no video audio is present or module does not support audio null is returned..
	 */
	virtual deBaseVideoAudioDecoder *CreateAudioDecoder( decBaseFileReader *reader );
	/*@}*/
};

#endif
