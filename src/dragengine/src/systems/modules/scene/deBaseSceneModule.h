/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEBASESCENEMODULE_H_
#define _DEBASESCENEMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deScene;


/**
 * \brief Base scene module providing load/save support for scenes.
 *
 * Scene modules load and save file formats like FBX or GLB which contain multiple objects
 * such as models, skins, rigs and animations. These file formats are typically used to
 * transfer entire scenes from one application to another.
 */
class DE_DLL_EXPORT deBaseSceneModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSceneModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~deBaseSceneModule() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Load scene from file.
	 *
	 * The module stores all objects found in the scene file as the respective resource
	 * types in the scene resources dictionary. Objects not convertible to engine resources
	 * but convertible to other file formats are stored as memory files.
	 */
	virtual void LoadScene(decBaseFileReader &reader, deScene &scene) = 0;
	
	/**
	 * \brief Save scene to file.
	 *
	 * For saving the user has to add resources and memory files to the deScene object
	 * before calling this method. The scene module then saves them to the file.
	 */
	virtual void SaveScene(decBaseFileWriter &writer, const deScene &scene) = 0;
	/*@}*/
};

#endif
