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

#ifndef _IGDEENVIRONMENTIGDE_H_
#define _IGDEENVIRONMENTIGDE_H_

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/logger/deLoggerReference.h>


class igdeWindowMain;


/**
 * \brief IGDE Environment Implementation.
 */
class igdeEnvironmentIGDE : public igdeEnvironment{
private:
	deLoggerReference pLogger;
	igdeWindowMain *pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment. */
	igdeEnvironmentIGDE();
	
	/** \brief Clean up environment. */
	virtual ~igdeEnvironmentIGDE() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void SetWindowMain( igdeWindowMain *windowMain );
	
	/** \brief Set logger. */
	void SetLogger( deLogger *logger );
	
	
	
	/**
	 * \brief IGDE UI Container to add module editor windows to.
	 * 
	 * Each module has to add one and only one editor window.
	 */
	virtual igdeContainer *GetUIContainer() override;
	
	/** \brief Retrieves a stock icon. */
	virtual igdeIcon *GetStockIcon( eStockIcons icon ) override;
	
	/** \brief Stock rig. */
	virtual deRig::Ref GetStockRig( eStockRigs rig ) override;
	
	/** \brief Stock model. */
	virtual deModel::Ref GetStockModel( eStockModels model ) override;
	
	/**
	 * \brief Stock skin.
	 * 
	 * Stock skins are only available after the engine controller has been started for the
	 * first time. If not available nullptr is returned.
	 */
	virtual deSkin::Ref GetStockSkin( eStockSkins skin ) override;
	
	/** \brief Retrieves the engine controller. */
	virtual igdeEngineController *GetEngineController() override;
	
	/** \brief Retrieves the IGDE game definition. */
	virtual igdeGameDefinition *GetGameDefinition() override;
	
	/** \brief Shared game definition with identifier or NULL if not found. */
	virtual igdeGameDefinition *GetSharedGameDefinition( const char *id ) override;
	
	/** \brief Retrieves the game definition preview manager. */
	virtual igdeGDPreviewManager *GetGDPreviewManager() override;
	
	/** \brief Retrieves the texture property list. */
	virtual const igdeTexturePropertyList *GetTexturePropertyList() override;
	
	/** \brief The active game project. */
	virtual igdeGameProject *GetGameProject() override;
	
	/** \brief Retrieves the logger. */
	virtual deLogger *GetLogger() override;
	
	/** \brief Retrieves the game project virtual file system. */
	virtual deVirtualFileSystem *GetFileSystemGame() override;
	
	/** \brief Retrieves the igde virtual file system. */
	virtual deVirtualFileSystem *GetFileSystemIGDE() override;
	
	/** \brief Default application font configuration from host platform. */
	virtual void GetApplicationFont( igdeFont::sConfiguration &config ) override;
	
	/**
	 * \brief Shared font with configuration.
	 * 
	 * Avoids multiple instances of the same font configuration. Used by UI widgets only
	 * to use fonts defined in GuiThemes. Do not use for custom fonts with little chance
	 * of being shared by different parties. Shared fonts stay in memory until the IGDE
	 * is shut down.
	 */
	virtual igdeFont *GetSharedFont( const igdeFont::sConfiguration &configuration ) override;
	
	/** \brief System color. */
	virtual decColor GetSystemColor( eSystemColors color ) override;
	
	/** \brief Shared model collision rig. */
	virtual deRig *GetSharedModelCollisionRig() override;
	
	/** \brief Request asynchronous loading of resource calling listener once finished. */
	virtual void AsyncLoadResource( const char *filename,
		deResourceLoader::eResourceType resourceType,
		igdeResourceLoaderListener *listener ) override;
	
	/** \brief UI Helper. */
	virtual igdeUIHelper &GetUIHelper() override;
	
	/** \brief UI Helper for properties panels. */
	virtual igdeUIHelper &GetUIHelperProperties() override;
	
	
	
	/** \brief Named GuiTheme or default if not found. */
	virtual igdeGuiTheme *GetGuiThemeNamed( const char *name ) override;
	
	/** \brief Default GuiTheme. */
	virtual igdeGuiTheme *GetDefaultGuiTheme() override;
	
	
	
	/** \brief Sets the visibility of the progress bar in the status bar. */
	virtual void SetProgressVisible( bool visible ) override;
	/** \brief Sets the progress bar progress. */
	virtual void SetProgress( float progress ) override;
	/** \brief Sets the progress text. */
	virtual void SetProgressText( const char *text ) override;
	
	
	
	/** \brief Active module shared menus changed. */
	virtual void ActiveModuleSharedMenusChanged() override;
	
	/** \brief Active module shared toolbars changed. */
	virtual void ActiveModuleSharedToolBarsChanged() override;
	
	/** \brief Activate editor and bring it to the front. */
	virtual void ActivateEditor( igdeEditorModule *editor ) override;
	
	
	
	/**
	 * \brief Retrieves a list of file patterns suitable to be used in a file dialog
	 *        for opening files of a given resource type.
	 * \details The pattern list is build from the available engine modules.
	 */
	virtual const igdeFilePatternList *GetOpenFilePatternList( eFilePatternListTypes type ) override;
	
	/**
	 * \brief Retrieves a list of file patterns suitable to be used in a file dialog
	 *        for saving files of a given resource type.
	 * \details The pattern list is build from the available engine modules.
	 */
	virtual const igdeFilePatternList *GetSaveFilePatternList( eFilePatternListTypes type ) override;
	
	
	/**
	 * \brief Set collider delegee.
	 * 
	 * Events generated by the game engine for collider will be send to the delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	virtual void SetColliderDelegee( deCollider *collider, deBaseScriptingCollider *delegee ) override;
	
	/** \brief Get collider user pointer. */
	virtual void *GetColliderUserPointer( deCollider *collider ) override;
	
	/** \brief Set user pointer for collider. */
	virtual void SetColliderUserPointer( deCollider *collider, void *userPointer ) override;
	
	/**
	 * \brief Set delegee for touch sensor.
	 * 
	 * Events generated by the game engine for touch sensor will be send to delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	virtual void SetTouchSensorDelegee( deTouchSensor *touchSensor,
		deBaseScriptingTouchSensor *delegee ) override;
	
	/**
	 * \brief Set delegee for prop field.
	 * 
	 * Events generated by the game engine for prop field will be send to delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	virtual void SetPropFieldDelegee( dePropField *propField, deBaseScriptingPropField *delegee ) override;
	
	
	
	/**
	 * \brief Show window modal while keeping engine properly updating.
	 */
	virtual void RunModalWhileShown( igdeWindow &window ) override;
	
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * For use by game definition editor. All modules are notified the game definition change.
	 * This process is potentially lengthy. User input is blocked until all modules finished
	 * processing the changes. This method returns after starting the update process but
	 * before doing any actual work for the first time.
	 */
	virtual void ProjecGameDefinitionChanged() override;
	
	/**
	 * \brief Reload XML Element Classes.
	 * 
	 * After updating element classes calls ProjecGameDefinitionChanged().
	 * 
	 * For use by editors to tell the IGDE that XML Element Classes changed in disk. This is
	 * not done automatically since this can be an expensive operation. Editors have to trigger
	 * this if they know XML Element Classes are likely to have changed.
	 */
	virtual void ReloadXMLElementClasses() override;
	
	/**
	 * \brief Request opening file in editor.
	 * 
	 * Asks all editors if they can open the request file. If multiple editors can open
	 * the file the most suitable one.
	 * 
	 * \param[in] filename Filename in game data virtual file system.
	 * \returns \em true if the object has been opened or \em false otherwise.
	 */
	virtual bool OpenFileInEditor( const char *filename ) override;
	
	/**
	 * \brief Request saving unsaved documents.
	 * 
	 * If unsaved documents are present asks the user if he wants to save them. The user
	 * is free to decide which documents to save. He can also abort the saving.
	 * 
	 * \param[in] title Title of the unsaved documents dialog if shown.
	 * \param[in] message Message of the unsaved documents dialog if shown.
	 *                    Message is optional and can be \em NULL.
	 * \returns \em true if user wants to continue the action or \em false to abort.
	 */
	virtual bool RequestSaveDocuments( const char *title, const char* message ) override;
	
	/**
	 * \brief List of recently opened editor files.
	 */
	virtual const decStringList &GetRecentEditorFiles() override;
	
	/**
	 * \brief Add a file to the recent editor files list.
	 */
	virtual void AddRecentEditorFile( const char *filename ) override;
	
	/** \brief Close application. */
	virtual void CloseApplication() override;
	/*@}*/
};

#endif
