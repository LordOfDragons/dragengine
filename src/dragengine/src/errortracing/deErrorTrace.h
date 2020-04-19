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

#ifndef _DEAERRORTRACE_H_
#define _DEAERRORTRACE_H_

class deErrorTracePoint;
class deLoadableModule;
class deLogger;


/**
 * \brief Error Trace.
 *
 * An Error Trace object contains a list of Error Trace Point objects.
 * describing the path of an error that occured in the engine.
 */
class deErrorTrace{
private:
	char *pError;
	deErrorTracePoint **pPoints;
	int pPointCount;
	int pPointSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new error trace object. */
	deErrorTrace();
	
	/** \brief Clean up error trace object. */
	~deErrorTrace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Error. */
	inline const char *GetError() const{ return ( const char * )pError; }
	
	/** \brief Set error. */
	void SetError( const char *error );
	
	/** \brief Clears the error trace. */
	void Clear();
	/*@}*/
	
	
	
	/** \name Trace Point Management */
	/*@{*/
	/** \brief Determines if there exist trace points. */
	inline bool HasPoints() const{ return pPointCount > 0; }
	
	/** \brief Trace is empty. */
	inline bool IsEmpty() const{ return pPointCount == 0; }
	
	/** \brief Count of trace points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Indexed trace point. */
	deErrorTracePoint *GetPoint( int index ) const;
	
	/** \brief Adds a new trace point. */
	void AddPoint( deErrorTracePoint *point );
	
	/** \brief Removes all trace points. */
	void RemoveAllPoints();
	/*@}*/
	
	
	
	/** \name Convenience Functions */
	/*@{*/
	/**
	 * \brief Add given error to the trace and sets the error if there exists no trace point yet.
	 * 
	 * Returns the new trace point if added or NULL otherwise.
	 */
	deErrorTracePoint *AddAndSetIfEmpty( const char *error, deLoadableModule *sourceModule,
		const char *sourceFunc, int sourceLine );
	
	/** \brief Add new trace point with the given informations. */
	deErrorTracePoint *AddPoint( deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine );
	
	/**
	 * \brief Print error trace out to the given logger.
	 * 
	 * This is more of a temporary function and should not be used.
	 */
	void PrintTrace( deLogger &logger );
	/*@}*/
};

#endif
