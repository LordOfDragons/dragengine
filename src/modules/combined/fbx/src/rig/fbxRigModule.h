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

#ifndef _FBXRIGMODULE_H_
#define _FBXRIGMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>

class fbxScene;
class fbxNode;
class fbxProperty;
class fbxRigBone;


/**
 * \brief FBX Rig Module.
 */
class fbxRigModule : public deBaseRigModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	fbxRigModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~fbxRigModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load rig. */
	virtual void LoadRig(decBaseFileReader &reader, deRig &rig);
	
	/** \brief Save rig. */
	virtual void SaveRig(decBaseFileWriter &writer, const deRig &rig);
	/*@}*/
	
	
	
private:
	void pLoadRig(deRig &rig, fbxScene &scene);
	
	void pLoadRigBone(deRig &rig, const fbxScene &scene, const fbxRigBone &bone);
};

#endif
