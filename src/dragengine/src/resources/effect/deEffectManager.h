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

#ifndef _DEEFFECTMANAGER_H_
#define _DEEFFECTMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deEffect;
class deEffectFilterKernel;
class deEffectOverlayImage;
class deEffectColorMatrix;
class deEffectDistortImage;


/**
 * \brief Effect Resource Manager.
 *
 */
class DE_DLL_EXPORT deEffectManager : public deResourceManager{
private:
	deResourceList pEffects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect resource manager linked to the given engine. */
	deEffectManager(deEngine *engine);
	
	/** \brief Clean up effect resource manager and reports leaking resources. */
	virtual ~deEffectManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of scenes. */
	int GetEffectCount() const;
	
	/** \brief Root effect resource for iteration purpose. */
	deEffect *GetRootEffect() const;
	
	/** \brief Create new effect filter kernel. */
	deEffectFilterKernel *CreateEffectFilterKernel();
	
	/** \brief Create new effect overlay image. */
	deEffectOverlayImage *CreateEffectOverlayImage();
	
	/** \brief Create new color matrix effect. */
	deEffectColorMatrix *CreateEffectColorMatrix();
	
	/** \brief Create new distort image effect. */
	deEffectDistortImage *CreateEffectDistortImage();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	//void SystemPhysicsLoad();
	//void SystemPhysicsUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
