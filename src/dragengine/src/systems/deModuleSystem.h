/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEMODULESYSTEM_H_
#define _DEMODULESYSTEM_H_

#include "../common/collection/decObjectOrderedSet.h"

class deEngine;
class deLoadableModule;
class deBaseModule;


// definitions
#define DEGS_MODULES_PATH "modules"


/**
 * \brief Module System.
 *
 * This is a special system and not to be confused with single type
 * or multiple type engine systems. The module system keeps track
 * of all modules existing in the engine no matter if loaded or not.
 * All modules that can be parsed are stored in the module system.
 * If a module can not be loaded it still stays here but is marked
 * as not loaded. This is to allow the engine and frontends to
 * provide a nice view to the user showing what is working and what
 * not. Only loaded modules are used by the engine.
 */
class deModuleSystem{
public:
	/** \brief Module Types. */
	enum eModuleTypes{
		/** \brief Module type is not known or invalid. */
		emtUnknown,
		
		/** \brief Graphic system module. */
		emtGraphic,
		
		/** \brief Audio system module. */
		emtAudio,
		
		/** \brief Input system module. */
		emtInput,
		
		/** \brief Network system module. */
		emtNetwork,
		
		/** \brief Physics system module. */
		emtPhysics,
		
		/** \brief Image system module. */
		emtImage,
		
		/** \brief Video system module. */
		emtVideo,
		
		/** \brief Scripting system module. */
		emtScript,
		
		/** \brief Model system module. */
		emtModel,
		
		/** \brief Rig system module. */
		emtRig,
		
		/** \brief Skin system module. */
		emtSkin,
		
		/** \brief Animation system module. */
		emtAnimation,
		
		/** \brief Font system module. */
		emtFont,
		
		/** \brief Crash Recovery system module. */
		emtCrashRecovery,
		
		/** \brief Language pack system module. */
		emtLanguagePack,
		
		/** \brief Animator system module. */
		emtAnimator,
		
		/** \brief Sound system module. */
		emtSound,
		
		/** \brief AI system module. */
		emtAI,
		
		/** \brief Occlusion mesh module. */
		emtOcclusionMesh,
		
		/** \brief Synthesizer module. */
		emtSynthesizer,
		
		/** \brief Archive module. */
		emtArchive,
		
		/**
		 * \brief VR system module.
		 * \version 1.6
		 */
		emtVR
	};
	
	
	
private:
	deEngine *pEngine;
	decObjectOrderedSet pModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module system linked to the given engine. */
	deModuleSystem( deEngine *engine );
	
	/** \brief Clean up module system. */
	~deModuleSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Linked game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Scans the module directory for modules and loads them if possible. */
	void DetectModules();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Count of modules. */
	int GetModuleCount() const;
	
	/** \brief Count of modules of a given type. */
	int GetModuleCountFor( eModuleTypes type ) const;
	
	/** \brief Count of loaded modules of a given type. */
	int GetLoadedModuleCountFor( eModuleTypes type ) const;
	
	/** \brief Module at the given index. */
	deLoadableModule *GetModuleAt( int index ) const;
	
	/** \brief Highest version module with the given name or NULL if not found. */
	deLoadableModule *GetModuleNamed( const char *name ) const;
	
	/** \brief Module with the given name and version or NULL if not found. */
	deLoadableModule *GetModuleNamed( const char *name, const char *version ) const;
	
	/** \brief Module with the given name and at least version or NULL if not found. */
	deLoadableModule *GetModuleNamedAtLeast( const char *name, const char *version ) const;
	
	/** \brief First loaded module for the given type or NULL if not found. */
	deLoadableModule *GetFirstLoadedModuleFor( eModuleTypes type ) const;
	
	/**
	 * \brief First module of the given type able to handle the given file.
	 * 
	 * Multiple type modules provide a list of file extensions they can handle.
	 * If the extension of the given filename matches one of the entris of a
	 * module it is considered able to handle the file. If multiple versions
	 * of the same module exist the module with the highest version is returned.
	 */
	deLoadableModule *FindMatching( eModuleTypes type, const char *filename ) const;
	
	/**
	 * \brief First modules of the given type able to handle the given file.
	 * 
	 * This is the same as the FindMatching function but with the difference that
	 * the returned object is the module itself and not the wrapper around the
	 * module. This also requires that the module is loaded. In the other function
	 * Not loaded modules are matching too whereas here this is not the case.
	 * If multiple versions of the same module exist the module with the highest
	 * version is returned.
	 */
	deBaseModule *GetModuleAbleToLoad( eModuleTypes type, const char *filename ) const;
	
	/**
	 * Adds a module to the system. The module has not to exist already and
	 * has to be properly initialized to be accepted.
	 */
	void AddModule( deLoadableModule *module );
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/**
	 * \brief Compare two module versions.
	 * 
	 * The version strings have to consist of dot separated entries. The result
	 * is 0 if both strings are the same, -1 if the first differing version
	 * string part of the first version is less than the second or 1 otherwise.
	 */
	static int CompareVersion( const char *version1, const char *version2 );
	
	/** \brief Filename matches the given pattern. */
	bool MatchesPattern( const char *filename, const char *pattern ) const;
	
	/** \brief Determines if two strings are equal in a non-case sensitive comparisson. */
	bool StrEqual( const char *str1, const char *str2, int length ) const;
	
	/**
	 * Retrieves the type from eModuleTypes matching the given type string.
	 * This is used to translate type strings from the XML module definitions
	 * into an engine usable type. The following table shows the matchings.
	 * - "Graphic" returns emtGraphic
	 * - "Audio" returns emtAudio
	 * - "Input" returns emtInput
	 * - "Network" returns emtNetwork
	 * - "Physics" returns emtPhysics
	 * - "Image" returns emtImage
	 * - "Video" returns emtVideo
	 * - "Script" returns emtScript
	 * - "Model" returns emtModel
	 * - "Rig" returns emtRig
	 * - "Skin" returns emtSkin
	 * - "Animation" returns emtAnimation
	 * - "Font" returns emtFont
	 * - "CrashRecovery" returns emtCrashRecovery
	 * - "LanguagePack" returns emtLanguagePack
	 * - "Animator" returns emtAnimator
	 * - "Sound" returns emtSound
	 * - "AI" returns emtAI
	 * - "OcclusionMesh" returns emtOcclusionMesh
	 * - "Synthesizer" returns emtSynthesizer
	 * - "Archive" returns emtArchive
	 * - "VR" returns emtVR
	 *
	 * In all other cases emtUnknown is returned. Case does not matter.
	 */
	static eModuleTypes GetTypeFromString( const char *typeString );
	
	/** \brief Retrieves directory name for a given type */
	static const char *GetTypeDirectory( eModuleTypes type );
	
	/**
	 * Retrieves if the given type is a single type. The following are single types:
	 * - emtGraphic
	 * - emtAudio
	 * - emtInput
	 * - emtNetwork
	 * - emtPhysics
	 * - emtScript
	 * - emtAnimator
	 * - emtAI
	 * - emtCrashRecovery
	 * - emtSynthesizer
	 * - emtVR
	 *
	 * All others are multiple type.
	 */
	static bool IsSingleType( eModuleTypes type );
	/*@}*/
	
	
	
private:
	void pDetectModulesIn( const char *basePath, const char *directory, eModuleTypes type );
};

#endif
