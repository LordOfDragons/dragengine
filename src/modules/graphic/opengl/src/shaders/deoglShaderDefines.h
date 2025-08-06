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

#ifndef _DEOGLSHADERDEFINES_H_
#define _DEOGLSHADERDEFINES_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>


/**
 * Defines compiling shader programs.
 */
class deoglShaderDefines{
private:
	struct sDefine{
		decString name;
		decString value;
	};
	
	
private:
	sDefine *pDefines;
	int pDefineCount;
	int pDefineSize;
	
	
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
	const decString &GetDefineNameAt( int index ) const;
	
	/** Value of define at index. */
	const decString &GetDefineValueAt( int index ) const;
	
	/** Named define is present. */
	bool HasDefineNamed( const char *name ) const;
	
	/** Value of named define or default value if absent. */
	const decString &GetDefineValueFor( const char *name, const decString &defaultValue ) const;
	const char *GetDefineValueFor(const char *name, const char *defaultValue) const;
	
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
	
	/** Remove defines. */
	void RemoveDefines( const char *name1 );
	void RemoveDefines( const char *name1, const char *name2 );
	void RemoveDefines( const char *name1, const char *name2, const char *name3 );
	void RemoveDefines( const char *name1, const char *name2, const char *name3, const char *name4 );
	
	/** Remove all defines. */
	void RemoveAllDefines();
	
	/** Defines are equal. */
	bool Equals( const deoglShaderDefines &defines ) const;
	
	/** Filtered defines are equal. */
	bool Equals(const deoglShaderDefines &defines, const decStringSet &filter) const;
	
	/** Calculate cache id. */
	decString CalcCacheId() const;
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
