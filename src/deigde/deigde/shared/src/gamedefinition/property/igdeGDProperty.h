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

#ifndef _IGDEPROPERTY_H_
#define _IGDEPROPERTY_H_

#include "../../gui/filedialog/igdeFilePattern.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Game Definition Property.
 * 
 * Describes a game definition property. Contains the type of the property
 * as well as a detailed description of the property and the possible values.
 */
class DE_DLL_EXPORT igdeGDProperty : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDProperty> Ref;
	
	
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
	
	/** \brief Number of property types. */
	static const int CountTypes = eptIdentifier + 1;
	
	
	
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
		
		/** \brief Audio model. */
		epptAudioModel,
		
		/** \brief Font file. */
		epptFont,
		
		/** \brief Sky. */
		epptSky,
		
		/** \brief Camera. */
		epptCamera,
		
		/** \brief World. */
		epptWorld,
		
		/** \brief Custom file pattern. */
		epptCustom
	};
	
	/** \brief Number of file pattern types. */
	static const int CountPatternTypes = epptCustom + 1;
	
	
	
private:
	decString pName;
	decString pDescription;
	ePropertyTypes pType;
	float pMinimumValue;
	float pMaximumValue;
	decString pDefaultValue;
	decStringList pOptions;
	ePathPatternTypes pPathPatternType;
	igdeFilePattern::List pCustomPathPattern;
	decString pIdentifierGroup;
	bool pIdentifierUsage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	igdeGDProperty(const char *name);
	
	/** \brief Create copy of property. */
	igdeGDProperty(const igdeGDProperty &property);
	
protected:
	/** \brief Clean up property. */
	virtual ~igdeGDProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Type. */
	inline ePropertyTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(ePropertyTypes type);
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinimumValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue(float value);
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaximumValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue(float value);
	
	/** \brief Select options. */
	inline decStringList &GetOptions(){ return pOptions; }
	inline const decStringList &GetOptions() const{ return pOptions; }
	
	/** \brief Path pattern type. */
	inline ePathPatternTypes GetPathPatternType() const{ return pPathPatternType; }
	
	/** \brief Set path pattern type. */
	void SetPathPatternType(ePathPatternTypes type);
	
	/** \brief Custom path pattern list. */
	inline igdeFilePattern::List &GetCustomPathPattern(){ return pCustomPathPattern; }
	inline const igdeFilePattern::List &GetCustomPathPattern() const{ return pCustomPathPattern; }
	
	/** \brief Identifier group or empty string if not set. */
	inline const decString &GetIdentifierGroup() const{ return pIdentifierGroup; }
	
	/** \brief Set identifier group or empty string if not set. */
	void SetIdentifierGroup(const char *group);
	
	/** \brief Usage count of identifier in group is altered. */
	inline bool GetIdentifierUsage() const{ return pIdentifierUsage; }
	
	/** \brief Set if usage count of identifier in group is altered. */
	void SetIdentifierUsage(bool usage);
	
	/** \brief Default value. */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const char *value);
	
	
	
	/** \brief Set from another property. */
	igdeGDProperty &operator=(const igdeGDProperty &property);
	/*@}*/
};

#endif
