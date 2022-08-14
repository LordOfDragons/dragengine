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
 * Defines compiling shader programs.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create defines. */
	deoglShaderDefines();
	
	/** Create copy of defines. */
	deoglShaderDefines( const deoglShaderDefines &defines );
	
	/** Clean up defines. */
	~deoglShaderDefines();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of defines. */
	inline int GetDefineCount() const{ return pDefineCount; }
	
	/** Name of define at index. */
	const char *GetDefineNameAt( int index ) const;
	
	/** Value of define at index. */
	const char *GetDefineValueAt( int index ) const;
	
	/** Named define is present. */
	bool HasDefineNamed( const char *name ) const;
	
	/** Value of named define or default value if absent. */
	const char *GetDefineValueFor( const char *name, const char *defaultValue ) const;
	
	/** Set define. */
	void SetDefine( const char *name, const char *value );
	void SetDefine( const char *name, int value );
	void SetDefine( const char *name, bool value );
	
	/** Set multiple defines set to '1'. */
	void SetDefines( const char *name1 );
	void SetDefines( const char *name1, const char *name2 );
	void SetDefines( const char *name1, const char *name2, const char *name3 );
	void SetDefines( const char *name1, const char *name2, const char *name3, const char *name4 );
	
	/** Remove define. */
	void RemoveDefine( const char *name );
	
	/** Remove all defines. */
	void RemoveAllDefines();
	
	/** Defines are equal. */
	bool Equals( const deoglShaderDefines &defines ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Defines are equal. */
	bool operator==( const deoglShaderDefines &defines ) const;
	
	/** Replace defines. */
	deoglShaderDefines &operator=( const deoglShaderDefines &defines );
	
	/** Combine defines. */
	deoglShaderDefines operator+( const deoglShaderDefines &defines ) const;
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
