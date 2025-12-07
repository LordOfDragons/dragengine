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

#ifndef _IGDECREATEPROJECT_H_
#define _IGDECREATEPROJECT_H_

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include "igdeGameDefinition.h"
#include "igdeGameProject.h"

class igdeTemplate;
class igdeTemplateFile;
class igdeTemplateReplace;
class igdeWindowMain;



/**
 * \brief Create game project.
 */
class igdeCreateProject{
private:
	igdeWindowMain &pWindowMain;
	
	decString pName;
	decString pDescription;
	decString pPathProject;
	decString pPathGameDefProject;
	decString pPathData;
	decString pPathCache;
	decString pScriptModule;
	decStringList pBaseGameDefs;
	const igdeTemplate *pTemplate;
	
	igdeGameProject::Ref pProject;
	
	decPath pNativePathProject;
	decString pNativePathData;
	decString pGameId;
	decString pGameAliasId;
	
	igdeGameDefinition::Ref pGameDef;
	decString pSharedGameDefContent;
	deVirtualFileSystem::Ref pVFS;
	
	decStringList pFileRenameTokens;
	decStringList pFileRenameValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project creator. */
	igdeCreateProject(igdeWindowMain &windowMain);
	
	/** \brief Clean up task. */
	~igdeCreateProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Path project game definition. */
	void SetPathGameDefProject(const char *path);
	
	/** \brief Set path project. */
	void SetPathProject(const char *path);
	
	/** \brief Set path data. */
	void SetPathData(const char *path);
	
	/** \brief Set path cache. */
	void SetPathCache(const char *path);
	
	/** \brief Script module. */
	void SetScriptModule(const char *module);
	
	/** \brief Base game definitions. */
	inline decStringList &GetBaseGameDefs(){ return pBaseGameDefs; }
	
	/** \brief Set project template or \em NULL. */
	void SetTemplate(const igdeTemplate *atemplate);
	
	
	
	/** \brief Created project or \em NULL. */
	inline const igdeGameProject::Ref &GetProject() const{ return pProject; }
	
	
	
	/**
	 * \brief Create project with stored parameters.
	 */
	void CreateProject();
	/*@}*/
	
	
	
private:
	void pCreateGameAliasId();
	void pCreateDirectories();
	void pCopyDefaultFiles();
	void pCreateGameDefinition();
	void pLoadSharedGameDefContent();
	void pSharedGameDefContentReplace();
	decString pReplaceTokens(const decString &string, const decStringList &tokens,
		const decStringList &values);
	void pCreateProjectReplacements(decStringList &tokens, decStringList &values);
	void pXmlEscapeValues(decStringList &values);
	decString pReplaceProjectTokens(const decString &string);
	decString pReplaceProjectTokensXml(const decString &string);
	void pStoreBaseGameDefs();
	void pApplyTemplate();
	void pTemplateCreateFile(const igdeTemplateFile &file);
	void pCreateFileRenames();
	void pCreateFileReplacements(const igdeTemplateFile &file,
		decStringList &tokens, decStringList &values);
	decString pEscapeStringC(const decString &string);
	decString pEscapeStringXml(const decString &string);
	decString pEscapeStringXmlMultiline(const decString &string);
};

#endif
