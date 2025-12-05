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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_W32
	#include <direct.h>
	#include "../string/unicode/decUnicodeString.h"
#endif
#ifdef OS_UNIX
	#include <unistd.h>
#endif

#include "decPath.h"
#include "../exceptions.h"



// Definitions
////////////////

#define PATH_CURDIR				"."
#define PATH_PARENTDIR			".."

#ifdef OS_UNIX
#define PATH_SEPARATOR_CHAR		'/'
#define PATH_SEPARATOR_STRING	"/"
#endif

#ifdef OS_W32
#define PATH_SEPARATOR_CHAR		'\\'
#define PATH_SEPARATOR_STRING	"\\"
#endif



// Options for fxfilematch
enum {
	FILEMATCH_PERIOD      = 4,        // Leading `.' is matched only explicitly
//	FILEMATCH_CASEFOLD    = 16        // Compare without regard to case
};



// Class decPath
//////////////////

// Constructors and Destructors
/////////////////////////////////

decPath::decPath(){
}

decPath::decPath( const decPath &path ) :
pComponents( path.pComponents ),
pPrefix( path.pPrefix ){
}

decPath::~decPath(){
}



// Management
///////////////

decPath decPath::CreatePathUnix( const char *path ){
	decPath p;
	p.SetFromUnix( path );
	return p;
}

decPath decPath::CreatePathNative( const char *path ){
	decPath p;
	p.SetFromNative( path );
	return p;
}

decPath decPath::CreateWorkingDirectory(){
	decPath path;
	path.SetWorkingDirectory();
	return path;
}

decPath decPath::AbsolutePathUnix( const char *path, const char *baseDirectory ){
	if( IsUnixPathAbsolute( path ) ){
		return CreatePathUnix( path );
	}
	
	decPath absolute;
	absolute.SetFromUnix( baseDirectory );
	absolute.AddUnixPath( path );
	return absolute;
}

decPath decPath::AbsolutePathNative( const char *path, const char *baseDirectory ){
	if( IsNativePathAbsolute( path ) ){
		return CreatePathNative( path );
	}
	
	decPath absolute;
	absolute.SetFromNative( baseDirectory );
	absolute.AddNativePath( path );
	return absolute;
}

decPath decPath::RelativePathUnix( const char *path, const char *baseDirectory, bool onlyBelow ){
	return decPath::CreatePathUnix( path ).RelativePath( decPath::CreatePathUnix( baseDirectory ), onlyBelow );
}

decPath decPath::RelativePathNative( const char *path, const char *baseDirectory, bool onlyBelow ){
	return decPath::CreatePathNative( path ).RelativePath( decPath::CreatePathNative( baseDirectory ), onlyBelow );
}

void decPath::SetPrefix( const char *prefix ){
	pPrefix = prefix;
}

void decPath::SetFromUnix( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( path[ 0 ] == '/' ){
		pPrefix = "/";
		pComponents.RemoveAll();
		pParseRelativePath( path + 1, '/' );
		
	}else{
		pPrefix.Empty();
		pComponents.RemoveAll();
		pParseRelativePath( path, '/' );
	}
}

void decPath::SetFromNative( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int prefixLen = pGetPrefixLength( path );
	pPrefix = decString( path ).GetLeft( prefixLen );
	pComponents.RemoveAll();
	pParseRelativePath( path + prefixLen, PATH_SEPARATOR_CHAR );
}

void decPath::SetWorkingDirectory(){
	#ifdef OS_UNIX
	char buffer[ FILENAME_MAX ];
	if( ! getcwd( buffer, FILENAME_MAX ) ){
		DETHROW( deeInvalidAction );
	}
	SetFromNative( buffer );
	
	#elif defined OS_W32
	wchar_t buffer[ FILENAME_MAX ];
	if( ! _wgetcwd( buffer, FILENAME_MAX ) ){
		DETHROW( deeInvalidAction );
	}
	
	const int count = wcslen( buffer );
	decUnicodeString unicode;
	int i;
	unicode.Set( 0, count );
	for( i=0; i<count; i++ ){
		unicode.SetAt( i, buffer[ i ] );
	}
	
	SetFromNative( unicode.ToUTF8() );
	
	#else
	#error "Missing implementation for decPath::SetWorkingDirectory for platform"
	#endif
}

void decPath::SetEmpty(){
	pPrefix.Empty();
	pComponents.RemoveAll();
}

decString decPath::GetPathUnix() const{
	return pPrefix + pComponents.Join( "/" );
}

decString decPath::GetPathNative() const{
	return pPrefix + pComponents.Join( PATH_SEPARATOR_STRING );
}

bool decPath::IsEmpty() const{
	return pComponents.GetCount() == 0;
}

bool decPath::IsAbsolute() const{
	return ! pPrefix.IsEmpty();
}

bool decPath::IsRelative() const{
	return pPrefix.IsEmpty();
}

void decPath::SetFrom( const decPath &path ){
	pPrefix = path.pPrefix;
	pComponents = path.pComponents;
}

void decPath::Add( const decPath &path ){
	pComponents += path.pComponents;
}

void decPath::AddUnixPath( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( path[ 0 ] == '/' ){
		pParseRelativePath( path + 1, '/' );
		
	}else{
		pParseRelativePath( path, '/' );
	}
}

void decPath::AddNativePath( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int prefixLen = pGetPrefixLength( path );
	if( prefixLen > 0 ){
		pParseRelativePath( path + prefixLen, PATH_SEPARATOR_CHAR );
		
	}else{
		pParseRelativePath( path, PATH_SEPARATOR_CHAR );
	}
}



bool decPath::MatchesPattern( const decPath &filePattern ) const{
	if( filePattern.IsAbsolute() ){
		const int count = pComponents.GetCount();
		if( filePattern.pComponents.GetCount() != count || filePattern.pPrefix != pPrefix ){
			return false;
		}
		
		int i;
		for( i=0; i<count; i++ ){
			if( ! decPath::fxfilematch( filePattern.pComponents.GetAt( i ),
				pComponents.GetAt( i ), FILEMATCH_PERIOD ) ){
					return false;
			}
		}
		
	}else{
		const int count = pComponents.GetCount();
		const int patternCount = filePattern.pComponents.GetCount();
		if( patternCount > count ){
			return false;
		}
		
		const int offset = count - patternCount;
		int i;
		for( i=0; i<patternCount; i++ ){
			if( ! decPath::fxfilematch( filePattern.pComponents.GetAt( i ),
				pComponents.GetAt( offset + i ), FILEMATCH_PERIOD ) ){
					return false;
			}
		}
	}
	
	return true;
}

decPath decPath::AbsolutePath( const decPath &baseDirectory ) const{
	if( IsAbsolute() ){
		return decPath( *this );
		
	}else{
		return baseDirectory + *this;
	}
}

decPath decPath::RelativePath( const decPath &baseDirectory, bool onlyBelow ) const{
	if( ! IsAbsolute() || ! baseDirectory.IsAbsolute() || pPrefix != baseDirectory.pPrefix ){
		return decPath( *this );
	}
	
	// source /a/b/c/d/e
	// base   /a/b/c       => d/e
	// base   /a/x/c/y     => ../../../b/c/d/e
	
	const int baseDirCount = baseDirectory.pComponents.GetCount();
	const int count = pComponents.GetCount();
	int i;
	
	for( i=0; i<baseDirCount; i++ ){
		if( i >= count || baseDirectory.pComponents.GetAt( i ) != pComponents.GetAt( i ) ){
			break;
		}
	}
	
	if( onlyBelow && i < baseDirCount ){
		return decPath( *this );
	}
	
	const int index = i;
	decPath newPath;
	for( i=index; i<baseDirCount; i++ ){
		newPath.pComponents.Add( ".." );
	}
	for( i=index; i<count; i++ ){
		newPath.pComponents.Add( pComponents.GetAt( i ) );
	}
	return newPath;
}



// Component Management
/////////////////////////

int decPath::GetComponentCount() const{
	return pComponents.GetCount();
}

const decString &decPath::GetComponentAt( int index ) const{
	return pComponents.GetAt( index );
}

void decPath::SetComponentAt( int index, const char *component ){
	pComponents.SetAt( index, component );
}

const decString &decPath::GetLastComponent() const{
	return pComponents.GetAt( pComponents.GetCount() - 1 );
}

void decPath::SetLastComponent( const char *component ){
	pComponents.SetAt( pComponents.GetCount() - 1, component );
}

void decPath::AddComponent( const char *component ){
	pComponents.Add( component );
}

void decPath::RemoveComponentFrom( int index ){
	pComponents.RemoveFrom( index );
}

void decPath::RemoveLastComponent(){
	pComponents.RemoveFrom( pComponents.GetCount() - 1 );
}

void decPath::RemoveAllComponents(){
	pComponents.RemoveAll();
}

bool decPath::HasComponents() const{
	return pComponents.GetCount() > 0;
}



// String Path Testing
////////////////////////

bool decPath::IsUnixPathAbsolute( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	return path[ 0 ] == '/';
}

bool decPath::IsNativePathAbsolute( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	#ifdef OS_UNIX
	return path[ 0 ] == '/';
	#endif
	
	#ifdef OS_W32
	if( strlen( path ) < 3 ){
		return false;
	}
	if( path[ 2 ] != '\\' ){
		return false;
	}
	if( path[ 1 ] != ':' ){
		return false;
	}
	if( path[ 0 ] >= 'A' && path[ 0 ] <= 'Z' ){
		return true;
	}
	if( path[ 0 ] >= 'a' && path[ 0 ] <= 'z' ){
		return true;
	}
	
	return false;
	#endif
}



// Constants
//////////////

char decPath::PathSeparator(){
	return PATH_SEPARATOR_CHAR;
}

const char *decPath::PathSeparatorString(){
	return PATH_SEPARATOR_STRING;
}



// Operators
//////////////

bool decPath::operator==( const decPath &path ) const{
	return pPrefix == path.pPrefix && pComponents == path.pComponents;
}

bool decPath::operator!=( const decPath &path ) const{
	return pPrefix != path.pPrefix || pComponents != path.pComponents;
}



decPath &decPath::operator=( const decPath &path ){
	pPrefix = path.pPrefix;
	pComponents = path.pComponents;
	return *this;
}

decPath &decPath::operator+=( const decPath &path ){
	Add( path );
	return *this;
}

decPath decPath::operator+( const decPath &path ) const{
	decPath p( *this );
	p.Add( path );
	return p;
}



// private functions
//////////////////////

void decPath::pParseRelativePath( const char *filename, int separator ){
	const decString sfilename( filename );
	int offset = 0, deliIndex, len = sfilename.GetLength();
	
	// parse filename
	while( offset < len ){
		// find path separator
		deliIndex = sfilename.Find( separator, offset );
		if( deliIndex == -1 ){
			deliIndex = len;
		}
		
		// if delimiter is the first skip it
		if( deliIndex == 0 ){
			DETHROW( deeInvalidParam ); // nah, that's more an error than anything else
		}
		
		// extract component
		decString tempComp( sfilename.GetMiddle( offset, deliIndex ) );
		offset = deliIndex + 1;
		
		// merge component with current components
		if( tempComp == PATH_PARENTDIR ){
			if( pComponents.GetCount() == 0 ){
				DETHROW_INFO( deeInvalidParam, "Can not move into parent directory" );
			}
			RemoveLastComponent();
			
		}else if( tempComp != PATH_CURDIR ){
			AddComponent( tempComp );
		}
	}
}

int decPath::pGetPrefixLength( const char *path ) const{
	#ifdef OS_UNIX
	return path[ 0 ] == '/' ? 1 : 0;
	#endif
	
	#ifdef OS_W32
	if( strlen( path ) < 3 ){
		return 0;
	}
	if( path[ 2 ] != '\\' ){
		return 0;
	}
	if( path[ 1 ] != ':' ){
		return 0;
	}
	if( path[ 0 ] >= 'A' && path[ 0 ] <= 'Z' ){
		return 3;
	}
	if( path[ 0 ] >= 'a' && path[ 0 ] <= 'z' ){
		return 3;
	}
	
	return 0;
	#endif
}



/*
 * File matching function borrowed from the fox toolkit ( www.fox-toolkit.org ).
 * Original file is fxfilematch.cpp .
 *
 * Modified the sources to match the specific search requirements.
 * - remove class storage specifiers ( better safe than sorry )
 * - removed the flags since the test is already carried out on path components
 *   so path separaters have not to be tested for
 * - tidied up the formating
 */

/*
  Notes:
  - This is "upward compatible" from the standard fnmatch function in glibc,
    in addition to the basic matching, fxfilematch can also handle alternatives.

  - Match patterns are as follows:

    ?           Matches single character.
    *           Matches zero or more characters.
    [abc]       Matches a single character, which must be a, b, or c.
    [^abc]      Matches a single character, which must be anything other than a, b, or c.
    [!abc]      Ditto.
    [a-zA-Z]    Matches single character, which must be one of a-z or A-Z.
    [^a-zA-Z]   Matches single character, which must be anything other than a-z or A-Z.
    [!a-zA-Z]   Ditto.
    pat1|pat2   Matches either pat1 or pat2.
    pat1,pat2   Ditto.
    (pat1|pat2) Matches either pat1 or pat2; patterns may be nested.
    (pat1,pat2) Ditto.

  - Examples:

    *.cpp|*.cc|*.cxx|*.C  Matches some common extensions for C++ source files.

    image.(bmp,gif,jpg)   Matches a file called image given as either bmp, gif, or jpg.

    *.[^o]                Matches any file except object files.

  - You can escape meta characters like '?', '*', '(', ')', '|', '^', '!', and ','
    with the backslash '\'.

  - Match modes:

    FILEMATCH_FILE_NAME     No wildcard can ever match "/" (or "\","/" under Windows).
    FILEMATCH_NOESCAPE      Backslashes don't quote special chars ("\" is treated as "\").
    FILEMATCH_PERIOD        Leading "." is matched only explicitly (Useful to match hidden files on Unix).
    FILEMATCH_LEADING_DIR   Ignore "/..." after a match.
    FILEMATCH_CASEFOLD      Compare without regard to case.

  - Note that under Windows, FILEMATCH_NOESCAPE must be passed
*/

// If folding case, make lower case
//#define FOLD(c)          ( ( flags & FILEMATCH_CASEFOLD ) ? Ascii::toLower( c ) : ( c ) )
#define FOLD(c)			( c )

bool decPath::domatch( const char *pattern, const char *string, int flags ){
	char c, cs, ce, cc, neg;
	const char *p = pattern;
	const char *q = string;
	//const char *s;
	int level;
	
	while( ( c = *p++ ) != '\0' ){
		switch( c ){
			case '?':
				if( *q == '\0' ) return false;
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				q++;
				break;
			
			case '*':
				c = *p;
				while( c == '*' ){
					c = *++p;
				}
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				if( c == '\0'){    // Optimize for case of trailing '*'
					return true;
				}
				while( ! domatch( p, q, flags & ~FILEMATCH_PERIOD ) ){
					if( *q++ == '\0' ) return false;
				}
				return true;
			
			case '[':
				if( *q == '\0' ) return false;
				if( ( flags & FILEMATCH_PERIOD ) && ( *q == '.' ) && ( q == string ) ) return false;
				cc = FOLD( *q );
				neg = ( ( *p == '!' ) || ( *p == '^' ) );
				if( neg ) p++;
				c = *p++;
				do{
					if( c == '\\' ) c = *p++;
					cs = ce = FOLD( c );
					if( c == '\0' ) return false;
					c = *p++;
					c = FOLD( c );
					if( c == '-' && *p != ']' ){
						c = *p++;
						if( c == '\\' ) c = *p++;
						if( c == '\0' ) return false;
						ce = FOLD( c );
						c = *p++;
					}
					if( ( cs <= cc ) && ( cc <= ce ) ) goto match;
				}
				while( c != ']' );
				if( ! neg ) return false;
				q++;
				break;
				
match:			while( c != ']' ){
					if( c == '\0' ) return false;
					c = *p++;
					if( c == '\\' ) p++;
				}
				if( neg ) return false;
				q++;
				break;
			
			case '(':
nxt:			if( domatch( p, q, flags ) ) return true;
				for( level = 0; *p && 0 <= level; ){
					switch( *p++ ){
					case '\\':
						if( *p ) p++;
						break;
					
					case '(':
						level++;
						break;
					
					case ')':
						level--;
						break;
					
					case '|':
					case ',':
						if( level == 0 ) goto nxt;
					}
				}
				return false;
			
			case ')':
				break;
			
			case '|':
			case ',':
				for( level = 0; *p && 0 <= level; ){
					switch( *p++ ){
					case '\\':
						if( *p ) p++;
						break;
					
					case '(':
						level++;
						break;
					
					case ')':
						level--;
						break;
					}
				}
				break;
			
			case '\\':
				if( *p ) c = *p++;   // Trailing escape represents itself
			
			default:
				if( FOLD( c ) != FOLD( *q ) ) return false;
				q++;
				break;
		}
	}
	return ( *q == '\0' );
}

// Public API to matcher
bool decPath::fxfilematch( const char *pattern, const char *string, int flags ){
	const char *p = pattern;
	const char *q = string;
	int level;
	
	if( p && q ){
nxt:	if( decPath::domatch( p, q, flags ) ) return true;
		for( level = 0; *p && 0 <= level; ){
			switch( *p++ ){
			case '\\':
				if( *p ) p++;
				break;
			
			case '(':
				level++;
				break;
			
			case ')':
				level--;
				break;
			
			case '|':
			case ',':
				if( level == 0 ) goto nxt;
			}
		}
	}
	
	return false;
}
