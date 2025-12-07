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

#ifndef _DELANGUAGEPACKAMANAGER_H_
#define _DELANGUAGEPACKAMANAGER_H_ 

#include "../deFileResourceList.h"
#include "../deFileResourceManager.h"

class deEngine;
class deLanguagePack;
class deLanguagePackBuilder;


/**
 * \brief Language pack resource manager.
 */
class DE_DLL_EXPORT deLanguagePackManager : public deFileResourceManager{
private:
	deFileResourceList pLangPacks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack resource manager. */
	deLanguagePackManager(deEngine *engine);
	
	/** \brief Clean up language pack resource manager and reports leaking resources. */
	~deLanguagePackManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of language packs. */
	int GetLanguagePackCount() const;
	
	/** \brief Root language pack resource for iteration purpose. */
	deLanguagePack *GetRootLanguagePack() const;
	
	/** \brief Language pack with filename or NULL. */
	deLanguagePack *GetLanguagePackWith(const char *filename) const;
	
	/** \brief Language pack with filename or NULL. */
	deLanguagePack *GetLanguagePackWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create language pack using builder. */
	deLanguagePack *CreateLanguagePack(const char *filename, deLanguagePackBuilder &builder);
	
	/** \brief Create language pack using builder. */
	deLanguagePack *CreateLanguagePack(deVirtualFileSystem *vfs,
		const char *filename, deLanguagePackBuilder &builder);
	
	/** \brief Load language pack relative to base path. */
	deLanguagePack *LoadLanguagePack(const char *filename, const char *basePath = "/");
	
	/** \brief Load language pack relative to base path. */
	deLanguagePack *LoadLanguagePack(deVirtualFileSystem *vfs,
		const char *filename, const char *basePath = "/");
	
	/**
	 * \brief Add loaded and prepared language pack.
	 * 
	 * \warning To be used only by deResourceLoader.
	 */
	void AddLoadedLanguagePack(deLanguagePack *languagePack);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * \warning For use by resource objects only.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
