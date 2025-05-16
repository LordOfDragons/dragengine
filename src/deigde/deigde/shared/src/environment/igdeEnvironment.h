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

#ifndef _IGDEENVIRONMENT_H_
#define _IGDEENVIRONMENT_H_

#include "../gui/resources/igdeFont.h"

#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/model/deModel.h>

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
class igdeEditorModule;

class deBaseScriptingCollider;
class deBaseScriptingPropField;
class deBaseScriptingTouchSensor;
class deCollider;
class deLogger;
class dePropField;
class deTouchSensor;
class deVirtualFileSystem;
class deRig;
class decStringList;



/**
 * \brief Editor Environment Interface.
 * 
 * Provides access for editor modules to the editing environment.
 */
class DE_DLL_EXPORT igdeEnvironment{
public:
	/** \brief File Pattern List Types. */
	enum eFilePatternListTypes{
		efpltAll, //<! All files.
		efpltAnimation, //<! Animation.
		efpltAnimator, //<! Animator.
		efpltFont, //<! Font.
		efpltImage, //<! Image.
		efpltLanguagePack, //<! Language pack.
		efpltModel, //<! Model.
		efpltNavigationSpace, //<! Navigation space.
		efpltOcclusionMesh, //<! Occlusion mesh.
		efpltParticleEmitter, //<! Particle Emitter.
		efpltRig, //<! Rig.
		efpltSkin, //<! Skin.
		efpltSky, //<! Sky.
		efpltSynthesizer, //<! Synthesizer.
		efpltSound, //<! Sound.
		efpltVideo, //<! Video.
		efpltSpeechAnimation, //<! Speech animation.
		efpltCamera, //<! Camera file.
		efpltWorld //<! World file.
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
		esiSmallWarning,
		
		esiEdit,
		esiConfig
	};
	
	/** \brief System colors. */
	enum eSystemColors{
		escWindowBackground, //<! Window background color.
		escWindowForeground, //<! Window foreground color.
		escWidgetBackground, //<! Widget background color.
		escWidgetForeground, //<! Widget foreground color.
		escWidgetHighlight, //<! Widget highlight color.
		escWidgetShadow, //<! Widget shadow color.
		escWidgetSelectedBackground, //<! Widget selected background color.
		escWidgetSelectedForeground //<! Widget selected foreground color.
	};
	
	/** \brief Stock skins. */
	enum eStockSkins{
		/**
		 * \brief Skin is missing for a component texture.
		 * 
		 * This skin has no renderables defined.
		 */
		essMissing,
		
		/**
		 * \brief Skin to display an error condition.
		 * 
		 * This skin has no renderables defined.
		 */
		essError,
		
		/**
		 * \brief Skin with test map typically used to examine texture coordinate layout.
		 * 
		 * This skin has no renderables defined.
		 */
		essTestMap,
		
		/**
		 * \brief Skin to highlight edited objects using outline.
		 * 
		 * This skin has these renderables defined:
		 * - color: Color of the outline. Default value (1,0,0)
		 * - thickness: Thickness of the outline. Default value 0.005
		 */
		essEditOutline,
		
		/**
		 * \brief Skin to highlight edited objects using rim.
		 * 
		 * This skin has these renderables defined:
		 * - color: Color of the rim. Default value (1,0,0)
		 * - angle: Angle of rim. Default value 0.4
		 * - exponent: Exponent of rim. Default value 0.5
		 */
		essEditRim,
		
		/**
		 * \brief Skin to highlight edited objects using rim and outline.
		 * 
		 * This skin has these renderables defined:
		 * - color: Color of the outline and rim. Default value (1,0,0)
		 * - thickness: Thickness of the outline. Default value 0.005
		 * - angle: Angle of rim. Default value 0.4
		 * - exponent: Exponent of rim. Default value 0.5
		 */
		essEditRimOutline
	};
	
	/** \brief Stock models. */
	enum eStockModels{
		/** \brief Gizmo move. */
		esmGizmoMove
	};
	
	/** \brief Stock rigs. */
	enum eStockRigs{
		/**
		 * \brief Model collision rig.
		 * 
		 * This rig uses model collision.
		 */
		esrModelCollision,
		
		/**
		 * \brief Ghost collision rig.
		 * 
		 * This rig uses no collision.
		 */
		esrGhostCollision,
		
		/** \brief Gizmo move. */
		esrGizmoMove
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
	
	/** \brief Stock icon. */
	virtual igdeIcon *GetStockIcon( eStockIcons icon ) = 0;
	
	/**
	 * \brief Stock skin.
	 * 
	 * Stock skins are only available after the engine controller has been started for the
	 * first time. If not available nullptr is returned.
	 */
	virtual deSkin::Ref GetStockSkin( eStockSkins skin ) = 0;
	
	/**
	 * \brief Stock rig.
	 * 
	 * Stock rigs are only available after the engine controller has been started for the
	 * first time. If not available nullptr is returned.
	 */
	virtual deRig::Ref GetStockRig( eStockRigs rig ) = 0;
	
	/**
	 * \brief Stock model.
	 * 
	 * Stock models are only available after the engine controller has been started for the
	 * first time. If not available nullptr is returned.
	 */
	virtual deModel::Ref GetStockModel( eStockModels model ) = 0;
	
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
	
	/** \brief Activate editor and bring it to the front. */
	virtual void ActivateEditor( igdeEditorModule *editor ) = 0;
	
	
	
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
	
	/** \brief Close application. */
	virtual void CloseApplication() = 0;
	/*@}*/
};

#endif
