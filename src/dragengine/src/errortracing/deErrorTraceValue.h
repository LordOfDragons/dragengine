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

#ifndef _DEAERRORTRACEVALUE_H_
#define _DEAERRORTRACEVALUE_H_


/**
 * \brief Error Trace Value.
 *
 * Error Trace Value objects contain additional informations about the values in
 * use by the failing function.
 */
class deErrorTraceValue{
private:
	char *pName;
	char *pValue;
	deErrorTraceValue **pSubValues;
	int pSubValueCount, pSubValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace value object with the given name and value. */
	deErrorTraceValue( const char *name, const char *value );
	
	/** \brief Clean up error trace object. */
	~deErrorTraceValue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the value. */
	inline const char *GetName() const{ return ( const char * )pName; }
	
	/** \brief Value. */
	inline const char *GetValue() const{ return ( const char * )pValue; }
	
	/** \brief Set value. */
	void SetValue( const char *value );
	
	/** \brief Set value from a numeric value. */
	void SetValueFrom( float value );
	/*@}*/

	/** \name Trace SubValue Management */
	/*@{*/
	/** \brief Determines if there exist trace values. */
	inline bool HasSubValues() const{ return pSubValueCount > 0; }
	
	/** \brief Count of trace values. */
	inline int GetSubValueCount() const{ return pSubValueCount; }
	
	/** \brief Indexed trace value. */
	deErrorTraceValue *GetSubValue( int index ) const;
	
	/** \brief Named trace value or NULL if not found. */
	deErrorTraceValue *FindSubValue( const char *name ) const;
	
	/** \brief Adds a new trace value. */
	void AddSubValue( deErrorTraceValue *value );
	
	/** \brief Removes all trace values. */
	void RemoveAllSubValues();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/** \brief Adds a new trace value with the given informations. */
	deErrorTraceValue *AddSubValue( const char *name, const char *value );
	
	/** \brief Adds a new trace value with the given informations. */
	deErrorTraceValue *AddSubValueInt( const char *name, int value );
	
	/** \brief Adds a new trace value with the given informations. */
	deErrorTraceValue *AddSubValueFloat( const char *name, float );
	
	/** \brief Adds a new trace value with the given informations. */
	deErrorTraceValue *AddSubValueBool( const char *name, bool value );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
