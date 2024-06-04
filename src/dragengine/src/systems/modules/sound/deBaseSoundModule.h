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

#ifndef _DEBASESOUNDMODULE_H_
#define _DEBASESOUNDMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deBaseSoundDecoder;
class deBaseSoundInfo;
class deSound;


/**
 * \brief Base sound module providing load/save support for sounds.
 */
class DE_DLL_EXPORT deBaseSoundModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSoundModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseSoundModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read sound information from file. */
	virtual void InitLoadSound( decBaseFileReader &reader, deBaseSoundInfo &info ) = 0;
	
	/**
	 * \brief Save sound.
	 * 
	 * You can be sure that the file provided has been rewinded prior to this
	 * function call.
	 */
	virtual void SaveSound( decBaseFileWriter &writer, const deSound &sound ) = 0;
	
	/** \brief Create sound decoder peer. */
	virtual deBaseSoundDecoder *CreateDecoder( decBaseFileReader *reader ) = 0;
	/*@}*/
};

#endif
