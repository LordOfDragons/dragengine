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

#ifndef _DEMODELMANAGER_H_
#define _DEMODELMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deModel;
class deModelBuilder;


/**
 * \brief Model Resourse Manager.
 */
class DE_DLL_EXPORT deModelManager : public deFileResourceManager{
private:
	deFileResourceList pModels;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model resource manager linked to the given engine. */
	deModelManager(deEngine *engine);
	
	/** \brief Clean up model resource manager and reports leaking resources. */
	~deModelManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of models. */
	int GetModelCount() const;
	
	/** \brief Root model resource for iteration purpose. */
	deModel *GetRootModel() const;
	
	/** \brief Model with the given filename or NULL if not loaded yet. */
	deModel *GetModelWith(const char *filename) const;
	
	/** \brief Model with the given filename or NULL if not loaded yet. */
	deModel *GetModelWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create new and empty model. */
	deModel::Ref CreateModel(const char *filename, deModelBuilder &builder);
	
	/** \brief Create new and empty model. */
	deModel::Ref CreateModel(deVirtualFileSystem *vfs, const char *filename, deModelBuilder &builder);
	
	/** \brief Loads a model from the given file relative to the given base path. */
	deModel::Ref LoadModel(const char *filename, const char *basePath);
	
	/** \brief Loads a model from the given file relative to the given base path. */
	deModel::Ref LoadModel(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/**
	 * \brief Add loaded and fully prepared model.
	 * 
	 * This method is to be used only by the resource loader to add an model that has
	 * been loaded asynchronously.
	 */
	void AddLoadedModel(deModel *model);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad() override;
	void SystemGraphicUnload() override;
	void SystemPhysicsLoad() override;
	void SystemPhysicsUnload() override;
	void SystemAudioLoad() override;
	void SystemAudioUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
