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

#ifndef _IGDEENVIRONMENTIGDE_H_
#define _IGDEENVIRONMENTIGDE_H_

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/logger/deLogger::Ref.h>


class igdeWindowMain;


/**
 * \brief IGDE Environment Implementation.
 */
class igdeEnvironmentIGDE : public igdeEnvironment{
private:
	deLogger::Ref pLogger;
	igdeWindowMain *pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment. */
	igdeEnvironmentIGDE();
	
	/** \brief Clean up environment. */
	~igdeEnvironmentIGDE() override;
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
	igdeContainer *GetUIContainer() override;
	
	/** \brief Retrieves a stock icon. */
	igdeIcon *GetStockIcon( eStockIcons icon ) override;
	
	/** \brief Stock rig. */
	deRig::Ref GetStockRig( eStockRigs rig ) override;
	
	/** \brief Stock model. */
	deModel::Ref GetStockModel( eStockModels model ) override;
	
	/**
	 * \brief Stock skin.
	 * 
	 * Stock skins are only available after the engine controller has been started for the
	 * first time. If not available nullptr is returned.
	 */
	deSkin::Ref GetStockSkin( eStockSkins skin ) override;
	
	/** \brief Retrieves the engine controller. */
	igdeEngineController *GetEngineController() override;
	
	/** \brief Retrieves the IGDE game definition. */
	igdeGameDefinition *GetGameDefinition() override;
	
	/** \brief Shared game definition with identifier or NULL if not found. */
	igdeGameDefinition *GetSharedGameDefinition( const char *id ) override;
	
	/** \brief Retrieves the game definition preview manager. */
	igdeGDPreviewManager *GetGDPreviewManager() override;
	
	/** \brief Retrieves the texture property list. */
	const igdeTexturePropertyList *GetTexturePropertyList() override;
	
	/** \brief The active game project. */
	igdeGameProject *GetGameProject() override;
	
	/** \brief Retrieves the logger. */
	deLogger *GetLogger() override;
	
	/** \brief Retrieves the game project file system. */
	deVirtualFileSystem *GetFileSystemGame() override;
	
	/** \brief Retrieves the igde file system. */
	deVirtualFileSystem *GetFileSystemIGDE() override;
	
	/** \brief Default application font configuration from host platform. */
	void GetApplicationFont( igdeFont::sConfiguration &config ) override;
	
	/**
	 * \brief Shared font with configuration.
	 * 
	 * Avoids multiple instances of the same font configuration. Used by UI widgets only
	 * to use fonts defined in GuiThemes. Do not use for custom fonts with little chance
	 * of being shared by different parties. Shared fonts stay in memory until the IGDE
	 * is shut down.
	 */
	igdeFont *GetSharedFont( const igdeFont::sConfiguration &configuration ) override;
	
	/** \brief System color. */
	decColor GetSystemColor( eSystemColors color ) override;
	
	/** \brief Shared model collision rig. */
	deRig *GetSharedModelCollisionRig() override;
	
	/** \brief Request asynchronous loading of resource calling listener once finished. */
	void AsyncLoadResource( const char *filename,
		deResourceLoader::eResourceType resourceType,
		igdeResourceLoaderListener *listener ) override;
	
	/** \brief UI Helper. */
	igdeUIHelper &GetUIHelper() override;
	
	/** \brief UI Helper for properties panels. */
	igdeUIHelper &GetUIHelperProperties() override;
	
	
	
	/** \brief Named GuiTheme or default if not found. */
	igdeGuiTheme *GetGuiThemeNamed( const char *name ) override;
	
	/** \brief Default GuiTheme. */
	igdeGuiTheme *GetDefaultGuiTheme() override;
	
	
	
	/** \brief Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible ) override;
	/** \brief Sets the progress bar progress. */
	void SetProgress( float progress ) override;
	/** \brief Sets the progress text. */
	void SetProgressText( const char *text ) override;
	
	
	
	/** \brief Active module shared menus changed. */
	void ActiveModuleSharedMenusChanged() override;
	
	/** \brief Active module shared toolbars changed. */
	void ActiveModuleSharedToolBarsChanged() override;
	
	/** \brief Activate editor and bring it to the front. */
	void ActivateEditor( igdeEditorModule *editor ) override;
	
	
	
	/**
	 * \brief Retrieves a list of file patterns suitable to be used in a file dialog
	 *        for opening files of a given resource type.
	 * \details The pattern list is build from the available engine modules.
	 */
	const igdeFilePatternList *GetOpenFilePatternList( eFilePatternListTypes type ) override;
	
	/**
	 * \brief Retrieves a list of file patterns suitable to be used in a file dialog
	 *        for saving files of a given resource type.
	 * \details The pattern list is build from the available engine modules.
	 */
	const igdeFilePatternList *GetSaveFilePatternList( eFilePatternListTypes type ) override;
	
	
	/**
	 * \brief Set collider delegee.
	 * 
	 * Events generated by the game engine for collider will be send to the delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	void SetColliderDelegee( deCollider *collider, deBaseScriptingCollider *delegee ) override;
	
	/** \brief Get collider user pointer. */
	void *GetColliderUserPointer( deCollider *collider ) override;
	
	/** \brief Set user pointer for collider. */
	void SetColliderUserPointer( deCollider *collider, void *userPointer ) override;
	
	/**
	 * \brief Set delegee for touch sensor.
	 * 
	 * Events generated by the game engine for touch sensor will be send to delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	void SetTouchSensorDelegee( deTouchSensor *touchSensor,
		deBaseScriptingTouchSensor *delegee ) override;
	
	/**
	 * \brief Set delegee for prop field.
	 * 
	 * Events generated by the game engine for prop field will be send to delegee.
	 * Set to \em NULL to stop receiving events.
	 */
	void SetPropFieldDelegee( dePropField *propField, deBaseScriptingPropField *delegee ) override;
	
	
	
	/**
	 * \brief Show window modal while keeping engine properly updating.
	 */
	void RunModalWhileShown( igdeWindow &window ) override;
	
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * For use by game definition editor. All modules are notified the game definition change.
	 * This process is potentially lengthy. User input is blocked until all modules finished
	 * processing the changes. This method returns after starting the update process but
	 * before doing any actual work for the first time.
	 */
	void ProjecGameDefinitionChanged() override;
	
	/**
	 * \brief Reload XML Element Classes.
	 * 
	 * After updating element classes calls ProjecGameDefinitionChanged().
	 * 
	 * For use by editors to tell the IGDE that XML Element Classes changed in disk. This is
	 * not done automatically since this can be an expensive operation. Editors have to trigger
	 * this if they know XML Element Classes are likely to have changed.
	 */
	void ReloadXMLElementClasses() override;
	
	/**
	 * \brief Request opening file in editor.
	 * 
	 * Asks all editors if they can open the request file. If multiple editors can open
	 * the file the most suitable one.
	 * 
	 * \param[in] filename Filename in game data file system.
	 * \returns \em true if the object has been opened or \em false otherwise.
	 */
	bool OpenFileInEditor( const char *filename ) override;
	
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
	bool RequestSaveDocuments( const char *title, const char* message ) override;
	
	/**
	 * \brief List of recently opened editor files.
	 */
	const decStringList &GetRecentEditorFiles() override;
	
	/**
	 * \brief Add a file to the recent editor files list.
	 */
	void AddRecentEditorFile( const char *filename ) override;
	
	/** \brief Close application. */
	void CloseApplication() override;
	/*@}*/
};

#endif
