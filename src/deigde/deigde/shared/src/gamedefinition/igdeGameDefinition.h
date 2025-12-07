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

#ifndef _IGDEGAMEDEFINITION_H_
#define _IGDEGAMEDEFINITION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>

#include "particleemitter/igdeGDParticleEmitterManager.h"
#include "property/igdeGDPropertyList.h"
#include <dragengine/src/resources/model/deModel.h>
#include <dragengine/src/resources/skin/deSkin.h>

class igdeGDSkinManager;
class igdeGDClassManager;
class igdeGDSkyManager;
class igdeEnvironment;

class deVirtualFileSystem;


/**
 * \brief Base Game Definition Class.
 * 
 * Stores information about a game including the available object classes,
 * world properties, sector properties and default engine objects for the
 * case of broken links.
 */
class DE_DLL_EXPORT igdeGameDefinition : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGameDefinition> Ref;
	
	
private:
	igdeEnvironment &pEnvironment;
	
	decString pFilename;
	decString pID;
	decString pDescription;
	decString pBasePath;
	decString pVFSPath;
	decString pScriptModule;
	decStringList pBaseGameDefinitionIDList;
	
	igdeGDSkinManager *pSkinManager;
	igdeGDClassManager *pClassManager;
	igdeGDSkyManager *pSkyManager;
	igdeGDParticleEmitterManager pParticleEmitterManager;
	
	igdeGDPropertyList pListWorldProperties;
	igdeGDPropertyList pListDecalProperties;
	
	deModel::Ref pDefaultModel;
	deSkin::Ref pDefaultSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition. */
	igdeGameDefinition(igdeEnvironment &environment);
	
	
	
protected:
	/** \brief Clean up game definition. */
	virtual ~igdeGameDefinition();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	
	
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Set filename. */
	void SetFilename(const char *filename);
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID(const char *id);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Base path or empty string if not used. */
	inline const decString &GetBasePath() const{ return pBasePath; }
	
	/** \brief Set base path or empty string if not used. */
	void SetBasePath(const char *path);
	
	/** \brief VFS path or empty string if not used. */
	inline const decString &GetVFSPath() const{ return pVFSPath; }
	
	/** \brief Set vfs path or empty string if not used. */
	void SetVFSPath(const char *path);
	
	/** \brief Script module this game definition is compatible with. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set script module this game definition is compatible with. */
	void SetScriptModule(const char *identifier);
	
	/** \brief Base game definition id list. */
	inline decStringList &GetBaseGameDefinitionIDList(){ return pBaseGameDefinitionIDList; }
	inline const decStringList &GetBaseGameDefinitionIDList() const{ return pBaseGameDefinitionIDList; }
	
	
	
	/** \brief Skin manager. */
	inline igdeGDSkinManager *GetSkinManager() const{ return pSkinManager; }
	
	/** \brief Class manager. */
	inline igdeGDClassManager *GetClassManager() const{ return pClassManager; }
	
	/** \brief Sky manager. */
	inline igdeGDSkyManager *GetSkyManager() const{ return pSkyManager; }
	
	/** \brief Particle emitter manager. */
	inline igdeGDParticleEmitterManager &GetParticleEmitterManager(){
		return pParticleEmitterManager;}
	inline const igdeGDParticleEmitterManager &GetParticleEmitterManager() const{
		return pParticleEmitterManager;}
	
	/** \brief World properties. */
	inline igdeGDPropertyList &GetListWorldProperties(){ return pListWorldProperties; }
	inline const igdeGDPropertyList &GetListWorldProperties() const{ return pListWorldProperties; }
	
	/** \brief Decal properties. */
	inline igdeGDPropertyList &GetListDecalProperties(){ return pListDecalProperties; }
	inline const igdeGDPropertyList &GetListDecalProperties() const{ return pListDecalProperties; }
	
	
	
	/** \brief Update all tags. */
	void UpdateTags();
	
	/**
	 * \brief Resolve inherit classes.
	 * 
	 * Has to be called after the game definition has been loaded for the first time
	 * or after changing the structure of classes.
	 */
	void ResolveInheritClasses();
	
	/**
	 * \brief Update game definition using another game definition.
	 * 
	 * Adds copies of elements in the given game definition. If the element
	 * exists already it is replaced. \ref UpdateTags and \ref ResolveLinks is not
	 * called automatically.
	 */
	void UpdateWith(const igdeGameDefinition &gameDefinition);
	
	/**
	 * \brief Update game definition using loadede element classes.
	 * 
	 * If object class with same name as element class does not exist adds copy of element
	 * class as object class. furthermore the parent object class is looked up. if the parent
	 * object class is found the component links are updated to match the parent object class.
	 * 
	 * If object class with same name as element class exists then updates the object class
	 * with the element class information. This retains all information not stored inside
	 * element classes like for example snap points.
	 * 
	 * \ref UpdateTags and \ref ResolveLinks is not called automatically.
	 */
	void UpdateWithElementClasses(const igdeGameDefinition &gameDefinition);
	
	/**
	 * \brief Update game definition using found file resources.
	 * 
	 * \ref UpdateTags and \ref ResolveLinks is not called automatically.
	 */
	void UpdateWithFound(const igdeGameDefinition &gameDefinition);
	
	/** \brief Find XML elements classes using find path. */
	void FindClasses(deVirtualFileSystem &vfs, igdeGDClassManager &found);
	
	/** \brief Find skins using find path. */
	void FindSkins(deVirtualFileSystem &vfs, igdeGDSkinManager &found);
	
	/** \brief Find skies using find path. */
	void FindSkies(deVirtualFileSystem &vfs, igdeGDSkyManager &found);
	/*@}*/
	
	
	
	/**
	 * \name Engine Objects
	 * \deprecated Removed soon.
	 */
	/*@{*/
	/** \brief Default model. */
	inline const deModel::Ref &GetDefaultModel() const{ return pDefaultModel; }
	
	/** \brief Set default model. */
	void SetDefaultModel(deModel *model);
	
	/** \brief Default skin. */
	inline const deSkin::Ref &GetDefaultSkin() const{ return pDefaultSkin; }
	
	/** \brief Set default skin. */
	void SetDefaultSkin(deSkin *skin);
	
	/** \brief Update engine objects. */
	virtual void UpdateEngineObjects();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
