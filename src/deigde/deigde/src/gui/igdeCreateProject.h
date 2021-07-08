/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDECREATEPROJECT_H_
#define _IGDECREATEPROJECT_H_

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>

class igdeGameProject;
class igdeTemplate;
class igdeTemplateFile;
class igdeTemplateReplace;
class igdeWindowMain;
class igdeGameDefinition;



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
	
	igdeGameProject *pProject;
	
	decPath pNativePathProject;
	decString pNativePathData;
	decString pGameId;
	decString pGameAliasId;
	
	igdeGameDefinition *pGameDef;
	decString pSharedGameDefContent;
	deVirtualFileSystemReference pVFS;
	
	decStringList pFileRenameTokens;
	decStringList pFileRenameValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project creator. */
	igdeCreateProject( igdeWindowMain &windowMain );
	
	/** \brief Clean up task. */
	~igdeCreateProject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Path project game definition. */
	void SetPathGameDefProject( const char *path );
	
	/** \brief Set path project. */
	void SetPathProject( const char *path );
	
	/** \brief Set path data. */
	void SetPathData( const char *path );
	
	/** \brief Set path cache. */
	void SetPathCache( const char *path );
	
	/** \brief Script module. */
	void SetScriptModule( const char *module );
	
	/** \brief Base game definitions. */
	inline decStringList &GetBaseGameDefs(){ return pBaseGameDefs; }
	
	/** \brief Set project template or \em NULL. */
	void SetTemplate( const igdeTemplate *atemplate );
	
	
	
	/** \brief Created project or \em NULL. */
	inline igdeGameProject *GetProject() const{ return pProject; }
	
	
	
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
	decString pReplaceTokens( const decString &string, const decStringList &tokens,
		const decStringList &values );
	void pCreateProjectReplacements( decStringList &tokens, decStringList &values );
	void pXmlEscapeValues( decStringList &values );
	decString pReplaceProjectTokens( const decString &string );
	decString pReplaceProjectTokensXml( const decString &string );
	void pStoreBaseGameDefs();
	void pApplyTemplate();
	void pTemplateCreateFile( const igdeTemplateFile &file );
	void pCreateFileRenames();
	void pCreateFileReplacements( const igdeTemplateFile &file,
		decStringList &tokens, decStringList &values );
	decString pEscapeStringC( const decString &string );
	decString pEscapeStringXml( const decString &string );
	decString pEscapeStringXmlMultiline( const decString &string );
};

#endif
