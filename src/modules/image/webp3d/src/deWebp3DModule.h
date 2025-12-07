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

#ifndef _DEWEBP3DMODULE_H_
#define _DEWEBP3DMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/image/deBaseImageModule.h>



/**
 * WebP-3D Image Module.
 */
class deWebp3DModule : public deBaseImageModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deWebp3DModule(deLoadableModule &loadableModule);
	
	/** Clean up module. */
	virtual ~deWebp3DModule();
	/*@}*/
	
	
	
	/** \name Management */
	/**
	 * Quick check if the file can be loaded by this module.
	 * 
	 * This is usually a header check and is used to detect
	 * with what image module a file could be possibly loaded if
	 * it does not match any patterns. If the image can be loaded
	 * store the necessary information into an ImageInfo object and
	 * return it. Otherwise return NULL. The returned ImageInfo object
	 * is used to create the image and as parameter to LoadImage.
	 * You can be sure that the file pointer is located at the
	 * beginning of the file.
	 */
	virtual deBaseImageInfo *InitLoadImage(decBaseFileReader &reader);
	
	/**
	 * Load image.
	 * 
	 * You can be sure that the file pointer is at the same place left off
	 * after the prior call of CheckLoadImage on the same file and module.
	 * You can be sure that the image object specified in the function
	 * call has been created with the information stored in the infos
	 * parameter obtained from a prior call to the CheckLoadImage function.
	 */
	virtual void LoadImage(decBaseFileReader &reader, deImage &image, deBaseImageInfo &infos);
	
	/**
	 * Save image.
	 * 
	 * You can be sure that the file provided has been rewinded prior to this
	 * function call.
	 */
	virtual void SaveImage(decBaseFileWriter &writer, const deImage &image);
	/*@}*/
};

#endif
