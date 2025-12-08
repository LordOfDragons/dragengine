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

#ifndef _IGDEGAMEPROJECT_H_
#define _IGDEGAMEPROJECT_H_

#include "../gamedefinition/igdeGameDefinitionList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>
#include "igdeGameDefinition.h"

class igdeEnvironment;


/**
 * \brief IGDE Game Project.
 * 
 * Main object containing an entire game project loaded in the IGDE. This object
 * is available to all editors through the environment. Only one project can
 * be active at the same time.
 * 
 * \warning If you make changes to a game definition in use by the editors you are responsible
 *          to call the appropriate notification method in \ref igdeEnvironment.
 */
class DE_DLL_EXPORT igdeGameProject : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGameProject> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	
	decString pPathDirectory;
	decString pPathFile;
	bool pChanged;
	
	decString pName;
	decString pDescription;
	decString pPathData;
	decString pPathCache;
	decString pPathLocal;
	
	decStringList pBaseGameDefinitionIDList;
	decString pPathProjectGameDefinition;
	igdeGameDefinitionList pBaseGameDefinitionList;
	igdeGameDefinition::Ref pProjectGameDefinition;
	igdeGameDefinition::Ref pXMLEClassGameDefinition;
	igdeGameDefinition::Ref pFoundGameDefinition;
	igdeGameDefinition::Ref pGameDefinition;
	
	decString pScriptModule;
	decString pScriptModuleVersion;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project. */
	igdeGameProject(igdeEnvironment &environment);
	
	/** \brief Clean up game definition. */
	virtual ~igdeGameProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Directory path. */
	inline const decString &GetDirectoryPath() const{ return pPathDirectory; }
	
	/** \brief File path. */
	inline const decString &GetFilePath() const{ return pPathFile; }
	
	/** \brief Set file path. */
	void SetFilePath(const char *path);
	
	/** \brief Project name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set project name. */
	void SetName(const char *name);
	
	/** \brief Project description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set project description. */
	void SetDescription(const char *description);
	
	/** \brief Path to data relative to project directory. */
	inline const decString &GetPathData() const{ return pPathData; }
	
	/** \brief Set path to data relative to project directory. */
	void SetPathData(const char *path);
	
	/** \brief Path to cache relative to project directory. */
	inline const decString &GetPathCache() const{ return pPathCache; }
	
	/** \brief Set path to cache relative to project directory. */
	void SetPathCache(const char *path);
	
	/** \brief Path to local data relative to project directory. */
	inline const decString &GetPathLocal() const{ return pPathLocal; }
	
	/** \brief Set path to local data relative to project directory. */
	void SetPathLocal(const char *path);
	
	
	
	/** \brief Base game definition id list. */
	inline decStringList &GetBaseGameDefinitionIDList(){ return pBaseGameDefinitionIDList; }
	inline const decStringList &GetBaseGameDefinitionIDList() const{ return pBaseGameDefinitionIDList; }
	
	/** \brief Project game definition path relative to project directory. */
	inline const decString &GetPathProjectGameDefinition() const{ return pPathProjectGameDefinition; }
	
	/** \brief Set project game definition path relative to project directory. */
	void SetPathProjectGameDefinition(const char *path);
	
	/** \brief Base game definition list. */
	inline igdeGameDefinitionList &GetBaseGameDefinitionList(){ return pBaseGameDefinitionList; }
	inline const igdeGameDefinitionList &GetBaseGameDefinitionList() const{ return pBaseGameDefinitionList; }
	
	/**
	 * \brief Project game definition.
	 * 
	 * \note Make sure a valid game definition is set before using this object in the editors.
	 *       If the game definition path is not found set it to a default game definition instead.
	 */
	inline const igdeGameDefinition::Ref &GetProjectGameDefinition() const{ return pProjectGameDefinition; }
	
	/**
	 * \brief Set project game definition.
	 * 
	 * \note Make sure a valid game definition is set before using this object in the editors.
	 *       If the game definition name is not found set it to a default game definition instead.
	 */
	void SetProjectGameDefinition(igdeGameDefinition *gameDefinition);
	
	/**
	 * \brief XML Element Class game definition.
	 * 
	 * Contains game definition loaded from XML Element Classes. Contains only object classes.
	 * XML Element Classes define new object classes and update existing ones. This allows
	 * mappers to explicitly define object classes with specific extra parameters like snap
	 * points for example. The XML Element Class will then overwrite the regular parameters.
	 */
	inline const igdeGameDefinition::Ref &GetXMLEClassGameDefinition() const{ return pXMLEClassGameDefinition; }
	
	/**
	 * \brief Found file resources game definition.
	 */
	inline const igdeGameDefinition::Ref &GetFoundGameDefinition() const{ return pFoundGameDefinition; }
	
	/**
	 * \brief Merged game definition.
	 * 
	 * The merged game definition is the merge of the project, base and igde game definitions
	 * if existing. This is the working game definition for the entire editor as if a single
	 * game definition with all the content of the respective game definitions has been
	 * created. If possible use this game definition.
	 */
	inline const igdeGameDefinition::Ref &GetGameDefinition() const{ return pGameDefinition; }
	
	/**
	 * \brief Update merged game definition.
	 * 
	 * Replace merged game definition with new game definition build from merging IGDE game
	 * definition with base game definitions and eventually the project game definition
	 * if present. The merged game definition contains copies of all the elements defined
	 * in the source game definitions and is not going to be changed. Creating this merged
	 * game definition may take some time to finish. Since the merged game definition will
	 * not be changed only replaced it is safe to keep a reference.
	 */
	void MergeGameDefinitions();
	
	/** \brief Name of script module to use. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set name of script module to use. */
	void SetScriptModule(const char *moduleName);
	
	/** \brief Minimum script module version or empty string. */
	inline const decString &GetScriptModuleVersion() const{ return pScriptModuleVersion; }
	
	/** \brief Set minimum script module version or empty string. */
	void SetScriptModuleVersion(const char *version);
	
	
	
	/** \brief Project file changed. */
	inline bool GetChanged() const{ return pChanged; }
	
	/**
	 * \brief Sets if project file has changed.
	 * 
	 * If this is different than the current changed state a notification is send to all listeners.
	 */
	void SetChanged(bool changed);
	
	
	
	/** \brief Notify listeners the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
