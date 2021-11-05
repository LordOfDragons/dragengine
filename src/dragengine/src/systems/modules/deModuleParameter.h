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
class deModuleParameter{
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
	struct SelectionEntry{
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
	deModuleParameter( const deModuleParameter &parameter );
	
	/** \brief Clean up module parameter. */
	~deModuleParameter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eParameterTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eParameterTypes type );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Set minimum value. */
	void SetMinimumValue( float minValue );
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set maximum value. */
	void SetMaximumValue( float maxValue );
	
	/** \brief Value step size. */
	inline float GetValueStepSize() const{ return pValueStepSize; }
	
	/** \brief Set value step size. */
	void SetValueStepSize( float valueStepSize );
	
	/** \brief Count of selection entries. */
	inline int GetSelectionEntryCount() const{ return pSelectionEntryCount; }
	
	/** \brief Selection entry at index. */
	const SelectionEntry &GetSelectionEntryAt( int index ) const;
	
	/** \brief Index of selection entry with value or -1 if absent. */
	int IndexOfSelectionEntryWithValue( const char *value ) const;
	
	/** \brief Add selection entry. */
	void AddSelectionEntry( const SelectionEntry &entry );
	
	/** \brief Add selection entries. */
	void AddSelectionEntries( const SelectionEntry *entries, int entryCount );
	
	/** \brief Remove all selection entries. */
	void RemoveAllSelectionEntries();
	
	/** \brief Category. */
	inline eCategory GetCategory() const{ return pCategory; }
	
	/** \brief Set category. */
	void SetCategory( eCategory category );
	
	/** \brief Display name for use in configuration dialogs. */
	inline const decString &GetDisplayName() const{ return pDisplayName; }
	
	/** \brief Set display name for use in configuration dialogs. */
	void SetDisplayName( const char *displayName );
	
	/**
	 * \brief Default value.
	 * \version 1.7
	 */
	inline const decString &GetDefaultValue() const{ return pDefaultValue; }
	
	/**
	 * \brief Set default value.
	 * \version 1.7
	 */
	void SetDefaultValue( const decString &defaultValue );
	
	/** \brief Reset to default values. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from another parameter. */
	deModuleParameter &operator=( const deModuleParameter &parameter );
	/*@}*/
};

#endif
