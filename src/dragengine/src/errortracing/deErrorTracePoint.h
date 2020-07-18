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

#ifndef _DEAERRORTRACEPOINT_H_
#define _DEAERRORTRACEPOINT_H_


class deErrorTraceValue;
class deLoadableModule;


/**
 * \brief Error Trace Point.
 *
 * An Error Trace object contains a list of Error Trace Point objects.
 * describing the path of an error that occurred in the engine.
 */
class deErrorTracePoint{
private:
	deLoadableModule *pSourceModule;
	char *pSourceFunc;
	int pSourceLine;
	deErrorTraceValue **pValues;
	int pValueCount, pValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace point object. */
	deErrorTracePoint( const char *sourceFunc, int sourceLine );
	
	/** \brief Create new error trace point object. */
	deErrorTracePoint( deLoadableModule *module, const char *sourceFunc, int sourceLine );
	
	/** \brief Clean up error trace object. */
	~deErrorTracePoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Source module or NULL if the error occurred in the engine itself. */
	inline deLoadableModule *GetSourceModule() const{ return pSourceModule; }
	
	/** \brief Source function. */
	inline const char *GetSourceFunction() const{ return ( const char * )pSourceFunc; }
	
	/** \brief Source line. */
	inline int GetSourceLine() const{ return pSourceLine; }
	/*@}*/

	/** \name Trace Value Management */
	/*@{*/
	/** \brief Determines if there exist trace values. */
	inline bool HasValues() const{ return pValueCount > 0; }
	
	/** \brief Count of trace values. */
	inline int GetValueCount() const{ return pValueCount; }
	
	/** \brief Indexed trace value. */
	deErrorTraceValue *GetValue( int index ) const;
	
	/** \brief Named trace value or NULL if not found. */
	deErrorTraceValue *FindValue( const char *name ) const;
	
	/** \brief Adds a new trace value. */
	void AddValue( deErrorTraceValue *value );
	
	/** \brief Removes all trace values. */
	void RemoveAllValues();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValue( const char *name, const char *value );
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueInt( const char *name, int value );
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueFloat( const char *name, float );
	
	/** \brief Adds a new trace value with the given information. */
	deErrorTraceValue *AddValueBool( const char *name, bool value );
	/*@}*/
};

#endif
