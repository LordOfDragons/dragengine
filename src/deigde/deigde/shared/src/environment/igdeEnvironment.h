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

#ifndef _IGDEENVIRONMENT_H_
#define _IGDEENVIRONMENT_H_

#include "../gui/resources/igdeFont.h"

#include <dragengine/resources/loader/deResourceLoader.h>

class igdeEngineController;
class igdeFilePatternList;
class igdeGameDefinition;
class igdeContainer;
class igdeWindow;
class igdeGDPreviewManager;
class igdeGameProject;
class igdeGuiTheme;
class igdeIcon;
class igdeTexturePropertyList;
class igdeToolBarDock;
class igdeUIHelper;
class igdeResourceLoaderListener;

class deBaseScriptingCollider;
class deBaseScriptingPropField;
class deBaseScriptingTouchSensor;
class deCollider;
class deLogger;
class dePropField;
class deTouchSensor;
class deVirtualFileSystem;
class deRig;
class deSkin;
class decStringList;



/**
 * \brief Editor Environment Interface.
 * 
 * Provides access for editor modules to the editing environment.
 */
class igdeEnvironment{
public:
	/** \brief File Pattern List Types. */
	enum eFilePatternListTypes{
		/** \brief All files. */
		efpltAll,
		
		/** \brief Animation. */
		efpltAnimation,
		
		/** \brief Animator. */
		efpltAnimator,
		
		/** \brief Font. */
		efpltFont,
		
		/** \brief Image. */
		efpltImage,
		
		/** \brief Language pack. */
		efpltLanguagePack,
		
		/** \brief Model. */
		efpltModel,
		
		/** \brief Navigation space. */
		efpltNavigationSpace,
		
		/** \brief Occlusion mesh. */
		efpltOcclusionMesh,
		
		/** \brief Particle Emitter. */
		efpltParticleEmitter,
		
		/** \brief Rig. */
		efpltRig,
		
		/** \brief Skin. */
		efpltSkin,
		
		/** \brief Sky. */
		efpltSky,
		
		/** \brief Synthesizer. */
		efpltSynthesizer,
		
		/** \brief Sound. */
		efpltSound,
		
		/** \brief Video. */
		efpltVideo,
		
		/** \brief Speech animation. */
		efpltSpeechAnimation
	};
	
	/** \brief Stock icons. */
	enum eStockIcons{
		esiNew,
		esiOpen,
		esiSave,
		esiSaveAs,
		esiDelete,
		esiCopy,
		esiCut,
		esiPaste,
		esiUndo,
		esiRedo,
		esiQuit,
		
		esiPlus,
		esiMinus,
		esiUp,
		esiDown,
		esiLeft,
		esiRight,
		esiStrongUp,
		esiStrongDown,
		esiStrongLeft,
		esiStrongRight,
		esiDuplicate,
		esiSearch,
		
		esiWarning,
		
		esiSmallPlus,
		esiSmallMinus,
		esiSmallUp,
		esiSmallDown,
		esiSmallLeft,
		esiSmallRight,
		esiSmallStrongUp,
		esiSmallStrongDown,
		esiSmallStrongLeft,
		esiSmallStrongRight,
		esiSmallWarning
	};
	
	/** \brief System colors. */
	enum eSystemColors{
		/** \brief Window background color. */
		escWindowBackground,
		
		/** \brief Window foreground color. */
		escWindowForeground,
		
		/** \brief Widget background color. */
		escWidgetBackground,
		
		/** \brief Widget foreground color. */
		escWidgetForeground,
		
		/** \brief Widget hilight color. */
		escWidgetHilight,
		
		/** \brief Widget shadow color. */
		escWidgetShadow,
		
		/** \brief Widget selected background olor. */
		escWidgetSelectedBackground,
		
		/** \brief Widget selected foreground olor. */
		escWidgetSelectedForeground
	};
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor environment. */
	igdeEnvironment();
	/*@}*/
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Clean up editor environment. */
	virtual ~igdeEnvironment();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief IGDE UI Container to add module editor windows to.
	 * 
	 * Each module has to add one and only one editor window.
	 */
	virtual igdeContainer *GetUIContainer() = 0;
	
	/** \brief Retrieves a stock icon. */
	virtual igdeIcon *GetStockIcon( eStockIcons icon ) = 0;
	
	/** \brief Engine controller. */
	virtual igdeEngineController *GetEngineController() = 0;
	
	/** \brief Game definition. */
	virtual igdeGameDefinition *GetGameDefinition() = 0;
	
	/** \brief Shared game definition with identifier or NULL if not found. */
	virtual igdeGameDefinition *GetSharedGameDefinition( const char *id ) = 0;
	
	/** \brief Game definition preview manager. */
	virtual igdeGDPreviewManager *GetGDPreviewManager() = 0;
	
	/** \brief Texture property list. */
	virtual const igdeTexturePropertyList *GetTexturePropertyList() = 0;
	
	/** \brief The active game project. */
	virtual igdeGameProject *GetGameProject() = 0;
	
	/** \brief Logger. */
	virtual deLogger *GetLogger() = 0;
	
	/** \brief Game project virtual file system. */
	virtual deVirtualFileSystem *GetFileSystemGame() = 0;
	
	/** \brief Igde virtual file system. */
	virtual deVirtualFileSystem *GetFileSystemIGDE() = 0;
	
	/** \brief Default application font configuration from host platform. */
	virtual void GetApplicationFont( igdeFont::sConfiguration &config ) = 0;
	
	/**
	 * \brief Shared font with configuration.
	 * 
	 * Avoids multiple instances of the same font configuration. Used by UI widgets only
	 * to use fonts defined in GuiThemes. Do not use for custom fonts with little chance
	 * of being shared by different parties. Shared fonts stay in memory until the IGDE
	 * is shut down.
	 */
	virtual igdeFont *GetSharedFont( const igdeFont::sConfiguration &configuration ) = 0;
	
	/** \brief System color. */
	virtual decColor GetSystemColor( eSystemColors color ) = 0;
	
	/** \brief Shared model collision rig. */
	virtual deRig *GetSharedModelCollisionRig() = 0;
	
	/** \brief Request asynchronous loading of resource calling listener once finished. */
	virtual void AsyncLoadResource( const char *filename,
		deResourceLoader::eResourceType resourceType,
		igdeResourceLoaderListener *listener ) = 0;
	
	/** \brief UI Helper. */
	virtual igdeUIHelper &GetUIHelper() = 0;
	
	/** \brief UI Helper for properties panels. */
	virtual igdeUIHelper &GetUIHelperProperties() = 0;
	
	/** \brief Error skin. */
	virtual deSkin *GetErrorSkin() = 0;
	
	
	
	/** \brief Named GuiTheme or default if not found. */
	virtual igdeGuiTheme *GetGuiThemeNamed( const char *name ) = 0;
	
	/** \brief Default GuiTheme. */
	virtual igdeGuiTheme *GetDefaultGuiTheme() = 0;
	
	
	
	/** \brief Set visibility of the progress bar in the status bar. */
	virtual void SetProgressVisible( bool visible ) = 0;
	
	/** \brief Set progress bar progress. */
	virtual void SetProgress( float progress ) = 0;
	
	/** \brief Set progress text. */
	virtual void SetProgressText( const char *text ) = 0;
	
	
	
	/** \brief Active module shared menus changed. */
	virtual void ActiveModuleSharedMenusChanged() = 0;
	
	/** \brief Active module shared toolbars changed. */
	virtual void ActiveModuleSharedToolBarsChanged() = 0;
	
	
	
	/**
	 * \brief List of file patterns for open resource file dialogs.
	 * 
	 * The pattern list is build from the available engine modules.
	 */
	virtual const igdeFilePatternList *GetOpenFilePatternList( eFilePatternListTypes type ) = 0;
	
	/**
	 * \brief List of file patterns for save resource file dialogs.
	 * 
	 * The pattern list is build from the available engine modules.
	 */
	virtual const igdeFilePatternList *GetSaveFilePatternList( eFilePatternListTypes type ) = 0;
	
	
	
	/**
	 * \brief Set collider delegee.
	 * 
	 * Events generated by the game engine for collider will be send to the delegee.
	 * Set to NULL to stop receiving events.
	 */
	virtual void SetColliderDelegee( deCollider *collider, deBaseScriptingCollider *delegee ) = 0;
	
	/** \brief Get collider user pointer. */
	virtual void *GetColliderUserPointer( deCollider *collider ) = 0;
	
	/** \brief Set user pointer for collider. */
	virtual void SetColliderUserPointer( deCollider *collider, void *userPointer ) = 0;
	
	/**
	 * \brief Set delegee for touch sensor.
	 * 
	 * Events generated by the game engine for touch sensor will be send to delegee.
	 * Set to NULL to stop receiving events.
	 */
	virtual void SetTouchSensorDelegee( deTouchSensor *touchSensor,
		deBaseScriptingTouchSensor *delegee ) = 0;
	
	/**
	 * \brief Set delegee for prop field.
	 * 
	 * Events generated by the game engine for prop field will be send to delegee.
	 * Set to NULL to stop receiving events.
	 */
	virtual void SetPropFieldDelegee( dePropField *propField,
		deBaseScriptingPropField *delegee ) = 0;
	
	
	
	/**
	 * \brief Show window modal while keeping engine properly updating.
	 */
	virtual void RunModalWhileShown( igdeWindow &window ) = 0;
	
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * For use by game definition editor. All modules are notified the game definition change.
	 * This process is potentially lengthy. User input is blocked until all modules finished
	 * processing the changes. This method returns after starting the update process but
	 * before doing any actual work for the first time.
	 */
	virtual void ProjecGameDefinitionChanged() = 0;
	
	/**
	 * \brief Reload XML Element Classes.
	 * 
	 * After updating element classes calls ProjecGameDefinitionChanged().
	 * 
	 * For use by editors to tell the IGDE that XML Element Classes changed in disk. This is
	 * not done automatically since this can be an expensive operation. Editors have to trigger
	 * this if they know XML Element Classes are likely to have changed.
	 */
	virtual void ReloadXMLElementClasses() = 0;
	
	
	
	/**
	 * \brief Request opening file in editor.
	 * 
	 * Asks all editors if they can open the request file. If multiple editors can open
	 * the file the most suitable one.
	 * 
	 * \param[in] filename Filename in game data virtual file system.
	 * \returns \em true if the object has been opened or \em false otherwise.
	 */
	virtual bool OpenFileInEditor( const char *filename ) = 0;
	
	/**
	 * \brief Request saving unsaved documents.
	 * 
	 * If unsaved documents are present asks the user if he wants to save them. The user
	 * is free to decide which documents to save. He can also abort the saving.
	 * 
	 * \param[in] title Title of the unsaved documents dialog if shown.
	 * \param[in] message Message of the unsaved documents dialog if shown.
	 *                    Message is optional and can be NULL.
	 * \returns \em true if user wants to continue the action or \em false to abort.
	 */
	virtual bool RequestSaveDocuments( const char *title, const char* message ) = 0;
	
	/**
	 * \brief List of recently opened editor files.
	 */
	virtual const decStringList &GetRecentEditorFiles() = 0;
	
	/**
	 * \brief Add a file to the recent editor files list.
	 */
	virtual void AddRecentEditorFile( const char *filename ) = 0;
	/*@}*/
};

#endif
