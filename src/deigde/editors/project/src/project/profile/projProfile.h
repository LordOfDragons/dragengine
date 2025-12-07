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

#ifndef _PROJPROFILE_H_
#define _PROJPROFILE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/utils/decUuid.h>


class projProject;



/**
 * \brief Project profile.
 */
class projProfile : public deObject{
private:
	projProject *pProject;
	
	decString pName;
	decString pDescription;
	
	decString pScriptDirectory;
	decString pGameObject;
	decString pPathConfig;
	decString pPathCapture;
	
	decUuid pIdentifier;
	decString pAliasIdentifier;
	decString pTitle;
	decString pGameDescription;
	decString pCreator;
	decString pWebsite;
	decPoint pWindowSize;
	decStringSet pIcons;
	
	decStringSet pExcludePatterns;
	decStringSet pRequiredExtensions;
	
	decString pDelgaPath;
	
	decString pRunArguments;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<projProfile> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile. */
	projProfile();
	
	/** \brief Create copy og profile. */
	projProfile(const projProfile &profile);
	
protected:
	/** \brief Clean up profile. */
	virtual ~projProfile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Project. */
	inline projProject *GetProject() const{return pProject;}
	
	/** \brief Set project. */
	void SetProject(projProject *project);
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	
	
	/** \brief Script directory. */
	inline const decString &GetScriptDirectory() const{return pScriptDirectory;}
	
	/** \brief Set script directory. */
	void SetScriptDirectory(const char *directory);
	
	/** \brief Game object. */
	inline const decString &GetGameObject() const{return pGameObject;}
	
	/** \brief Set game object. */
	void SetGameObject(const char *gameObject);
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{return pPathConfig;}
	
	/** \brief Set config path. */
	void SetPathConfig(const char *path);
	
	/** \brief Capture path. */
	inline const decString &GetPathCapture() const{return pPathCapture;}
	
	/** \brief Set capture path. */
	void SetPathCapture(const char *path);
	
	
	
	/** \brief Game identifier. */
	inline const decUuid &GetIdentifier() const{return pIdentifier;}
	
	/** \brief Set game identifier. */
	void SetIdentifier(const decUuid &identifier);
	
	/** \brief Game alias identifier. */
	inline const decString &GetAliasIdentifier() const{return pAliasIdentifier;}
	
	/** \brief Set game alias identifier. */
	void SetAliasIdentifier(const char *identifier);
	
	/** \brief Game title. */
	inline const decString &GetTitle() const{return pTitle;}
	
	/** \brief Set game title. */
	void SetTitle(const char *title);
	
	/** \brief Game description. */
	inline const decString &GetGameDescription() const{return pGameDescription;}
	
	/** \brief Set Game description. */
	void SetGameDescription(const char *description);
	
	/** \brief Creator. */
	inline const decString &GetCreator() const{return pCreator;}
	
	/** \brief Set creator. */
	void SetCreator(const char *creator);
	
	/** \brief Website. */
	inline const decString &GetWebsite() const{return pWebsite;}
	
	/** \brief Set website. */
	void SetWebsite(const char *website);
	
	/** \brief Window size or 0 for fullscreen. */
	inline const decPoint &GetWindowSize() const{return pWindowSize;}
	
	/** \brief Set window size or 0 for fullscreen. */
	void SetWindowSize(const decPoint &size);
	
	/** \brief Icons. */
	inline const decStringSet &GetIcons() const{return pIcons;}
	
	/** \brief Set icons. */
	void SetIcons(const decStringSet &icons);
	
	
	
	/** \brief Set of file patterns to exclude from processing. */
	inline const decStringSet &GetExcludePatterns() const{return pExcludePatterns;}
	
	/** \brief Set set of file patterns to exclude from processing. */
	void SetExcludePatterns(const decStringSet &patterns);
	
	/** \brief Set of required resource file extensions. */
	inline const decStringSet &GetRequiredExtensions() const{return pRequiredExtensions;}
	
	/** \brief Set set of required resource file extensions. */
	void SetRequiredExtensions(const decStringSet &extensions);
	
	
	
	/** \brief Path to delga file to build. */
	inline const decString &GetDelgaPath() const{return pDelgaPath;}
	
	/** \brief Set path to delga file to build. */
	void SetDelgaPath(const char *path);
	
	
	
	/** \brief Arguments to use while test running. */
	inline const decString &GetRunArguments() const{return pRunArguments;}
	
	/** \brief Set arguments to use while test running. */
	void SetRunArguments(const char *arguments);
	
	
	
	/** \brief Notify listeners profile changed. */
	void NotifyChanged();
	
	/** \brief Notify listeners profile name changed. */
	void NotifyNameChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
