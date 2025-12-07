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

#ifndef _DEIESMODULE_H_
#define _DEIESMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/image/deBaseImageModule.h>


/**
 * \brief IES Image Module.
 */
class deIesModule : public deBaseImageModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deIesModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deIesModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Quick check if the file can be loaded by this module.  */
	virtual deBaseImageInfo *InitLoadImage(decBaseFileReader &reader);
	
	/** \brief Load image. */
	virtual void LoadImage(decBaseFileReader &reader, deImage &image, deBaseImageInfo &infos);
	
	/** \brief Save image. */
	virtual void SaveImage(decBaseFileWriter &writer, const deImage &image);
	/*@}*/
};

#endif
