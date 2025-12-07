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

#ifndef _DEMODULEPARAMETER_H_
#define _DEMODULEPARAMETER_H_

#include "../../common/string/decStringSet.h"
#include "../../common/string/decString.h"


/**
 * \brief Module parameter.
 *
 * Contains information about a module parameter. This information is designed
 * to help scripters to generate GUI widgets for the parameters matching the type
 * of the parameter to make it easier for the user to set the correct values.
 * The parameter can be of one of the four types boolean, numeric, ranged and
 * selection. A boolean parameter accepts values 0 for false and 1 for true. The
 * numeric parameter accepts all kinds of decimal values. The Ranged parameter
 * accepts only values in a given range spaced by a given step size. The range is
 * mandatory but the step size is optional to be cared about by the parameter
 * user. The selection parameter provides a list of values. The index into this
 * list is the value returned.
 */
class DE_DLL_EXPORT deModuleParameter{
public:
	/** \brief Parameter types. */
	enum eParameterTypes{
		/** \brief Boolean value ('0' or '1'). */
		eptBoolean,
		
		/** \brief Numeric value. */
		eptNumeric,
		
		/** \brief Numeric value which has to be inside certain range. */
		eptRanged,
		
		/** \brief String value. */
		eptString,
		
		/** \brief String value from selection of values. */
		eptSelection
	};
	
	/** \brief Display category. */
	enum eCategory {
		/**
		 * \brief Basic configuration parameter.
		 * 
		 * Parameter has to be shown to the user in configuration dialogs.
		 * Basic parameters control aspects of the game users want to change.
		 */
		ecBasic,
		
		/**
		 * \brief Advanced configuration parameter.
		 * 
		 * Parameter can be shown to the user in configuration dialogs if the user
		 * requests to see advanced parameters. Advanced parameters control advanced
		 * aspects of the game user way want to change if they are more experienced.
		 */
		ecAdvanced,
		
		/**
		 * \brief Expert parameter.
		 * 
		 * Parameter has to be hidden from user in configuration dialogs. Expert
		 * parameters should only be configurable through console commands or using
		 * configuration files for expert users only.
		 */
		ecExpert
	};
	
	/** \brief Selection entry. */
	struct DE_DLL_EXPORT SelectionEntry{
		decString value;
		decString displayName;
		decString description;
	};
	
	
	
private:
	eParameterTypes pType;
	decString pName;
	decString pDescription;
	float pMinValue;
	float pMaxValue;
	float pValueStepSize;
	SelectionEntry *pSelectionEntries;
	int pSelectionEntryCount;
	eCategory pCategory;
	decString pDisplayName;
	decString pDefaultValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module parameter. */
	deModuleParameter();
	
	/** \brief Create copy of module parameter. */
	deModuleParameter(const deModuleParameter &parameter);
	
	/** \brief Clean up module parameter. */
	~deModuleParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eParameterTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(eParameterTypes type);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue(float minValue);
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue(float maxValue);
	
	/** \brief Value step size. */
	inline float GetValueStepSize() const{ return pValueStepSize; }
	
	/** \brief Set value step size. */
	void SetValueStepSize(float valueStepSize);
	
	/** \brief Count of selection entries. */
	inline int GetSelectionEntryCount() const{ return pSelectionEntryCount; }
	
	/** \brief Selection entry at index. */
	const SelectionEntry &GetSelectionEntryAt(int index) const;
	
	/** \brief Index of selection entry with value or -1 if absent. */
	int IndexOfSelectionEntryWithValue(const char *value) const;
	
	/** \brief Add selection entry. */
	void AddSelectionEntry(const SelectionEntry &entry);
	
	/** \brief Add selection entries. */
	void AddSelectionEntries(const SelectionEntry *entries, int entryCount);
	
	/** \brief Remove all selection entries. */
	void RemoveAllSelectionEntries();
	
	/** \brief Category. */
	inline eCategory GetCategory() const{ return pCategory; }
	
	/** \brief Set category. */
	void SetCategory(eCategory category);
	
	/** \brief Display name for use in configuration dialogs. */
	inline const decString &GetDisplayName() const{ return pDisplayName; }
	
	/** \brief Set display name for use in configuration dialogs. */
	void SetDisplayName(const char *displayName);
	
	/**
	 * \brief Default value.
	 * \version 1.7
	 */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/**
	 * \brief Set default value.
	 * \version 1.7
	 */
	void SetDefaultValue(const decString &defaultValue);
	
	/** \brief Reset to default values. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from another parameter. */
	deModuleParameter &operator=(const deModuleParameter &parameter);
	/*@}*/
};

#endif
