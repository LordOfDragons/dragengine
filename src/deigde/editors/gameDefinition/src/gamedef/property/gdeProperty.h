/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEPROPERTY_H_
#define _GDEPROPERTY_H_

#include "../filepattern/gdeFilePatternList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Property.
 */
class gdeProperty : public deObject{
public:
	/** \brief Property type. */
	enum ePropertyTypes{
		/** \brief String value. */
		eptString,
		
		/** \brief Integer value. */
		eptInteger,
		
		/** \brief 2 component point. */
		eptPoint2,
		
		/** \brief 3 component point. */
		eptPoint3,
		
		/** \brief Float value. */
		eptFloat,
		
		/** \brief 2 component vector. */
		eptVector2,
		
		/** \brief 3 component vector. */
		eptVector3,
		
		/** \brief 3 component color value. */
		eptColor,
		
		/** \brief Boolean value. */
		eptBoolean,
		
		/** \brief Path value. */
		eptPath,
		
		/** \brief Numeric value inside a specific range. */
		eptRange,
		
		/** \brief Value from a selection of values or a custom value. */
		eptSelect,
		
		/** \brief List of string values. */
		eptList,
		
		/** \brief Trigger expression. */
		eptTriggerExpression,
		
		/** \brief Trigger target. */
		eptTriggerTarget,
		
		/** \brief shape. */
		eptShape,
		
		/** \brief Shape list. */
		eptShapeList,
		
		/** \brief Identifier. */
		eptIdentifier
	};
	
	/** \brief Path pattern types. */
	enum ePathPatternTypes{
		/** \brief All files. */
		epptAll,
		
		/** \brief Model file. */
		epptModel,
		
		/** \brief Skin file. */
		epptSkin,
		
		/** \brief Rig file. */
		epptRig,
		
		/** \brief Animation file. */
		epptAnimation,
		
		/** \brief Animator file. */
		epptAnimator,
		
		/** \brief Image file. */
		epptImage,
		
		/** \brief Occlusion mesh. */
		epptOcclusionMesh,
		
		/** \brief Navigation space. */
		epptNavigationSpace,
		
		/** \brief Particle emitter. */
		epptParticleEmitter,
		
		/** \brief Sound. */
		epptSound,
		
		/** \brief Synthesizer. */
		epptSynthesizer,
		
		/** \brief Video. */
		epptVideo,
		
		/** \brief Font. */
		epptFont,
		
		/** \brief Sky. */
		epptSky,
		
		/** \brief Camera. */
		epptCamera,
		
		/** \brief Custom file pattern. */
		epptCustom
	};
	
	
	
private:
	decString pName;
	decString pDescription;
	ePropertyTypes pType;
	float pMinimumValue;
	float pMaximumValue;
	decString pDefaultValue;
	decStringList pOptions;
	ePathPatternTypes pPathPatternType;
	gdeFilePatternList pCustomPathPattern;
	decString pIdentifierGroup;
	bool pIdentifierUsage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	gdeProperty();
	
	/** \brief Create property. */
	gdeProperty( const char *name );
	
	/** \brief Create copy of property. */
	gdeProperty( const gdeProperty &property );
	
	/** \brief Clean up property. */
	virtual ~gdeProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Type. */
	inline ePropertyTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( ePropertyTypes type );
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinimumValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue( float value );
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaximumValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue( float value );
	
	/** \brief Select options. */
	inline decStringList &GetOptions(){ return pOptions; }
	inline const decStringList &GetOptions() const{ return pOptions; }
	
	/** \brief Path pattern type. */
	inline ePathPatternTypes GetPathPatternType() const{ return pPathPatternType; }
	
	/** \brief Set path pattern type. */
	void SetPathPatternType( ePathPatternTypes type );
	
	/** \brief Custom path pattern list. */
	inline gdeFilePatternList &GetCustomPathPattern(){ return pCustomPathPattern; }
	inline const gdeFilePatternList &GetCustomPathPattern() const{ return pCustomPathPattern; }
	
	/** \brief Identifier group or empty string if not set. */
	inline const decString &GetIdentifierGroup() const{ return pIdentifierGroup; }
	
	/** \brief Set identifier group or empty string if not set. */
	void SetIdentifierGroup( const char *group );
	
	/** \brief Usage count of identifier in group is altered. */
	inline bool GetIdentifierUsage() const{ return pIdentifierUsage; }
	
	/** \brief Set if usage count of identifier in group is altered. */
	void SetIdentifierUsage( bool usage );
	
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue( const char *value );
	
	
	
	/** \brief Set from another property. */
	gdeProperty &operator=( const gdeProperty &property );
	/*@}*/
};

#endif
