/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHADERDEFINES_H_
#define _DEOGLSHADERDEFINES_H_



/**
 * @brief Shader Program Defines.
 * Stores defines to be used for compiling a given program.
 */
class deoglShaderDefines{
private:
	struct sDefine{
		char *name;
		char *value;
	};
	
private:
	sDefine *pDefines;
	int pDefineCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new defines object. */
	deoglShaderDefines();
	/** Creates a new defines object as a copy of another defines object. */
	deoglShaderDefines( const deoglShaderDefines &defines );
	/** Cleans up the defines object. */
	~deoglShaderDefines();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of defines. */
	inline int GetDefineCount() const{ return pDefineCount; }
	/** Retrieves the define name at the given index. */
	const char *GetDefineNameAt( int index ) const;
	/** Retrieves the define value at the given index. */
	const char *GetDefineValueAt( int index ) const;
	/** Determines if a define with the given name exists. */
	bool HasDefineNamed( const char *name ) const;
	/**
	 * Retrieves the value for the define with the given name. If such a define
	 * can not be found the default value is returned.
	 */
	const char *GetDefineValueFor( const char *name, const char *defaultValue ) const;
	
	/** Adds a new define. */
	void AddDefine( const char *name, const char *value );
	/** Removes all defines. */
	void RemoveAllDefines();
	
	/** Determines if this defines object equals another defines object. */
	bool Equals( const deoglShaderDefines &defines ) const;
	/*@}*/
	
	/** @name Operators */
	/*@{*/
	/** Determines a defines object is equal to this defines object. */
	bool operator==( const deoglShaderDefines &defines ) const;
	/** Sets this defines object to the content of another defines object. */
	deoglShaderDefines &operator=( const deoglShaderDefines &defines );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
