/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
