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

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include "exceptions/deException.h"


/**
 * \brief Invalid parameter exception.
 */
class DE_DLL_EXPORT deeInvalidParam : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeInvalidParam( const char *file, int line, const char *description = NULL );
};

/**
 * \brief Out of memory exception.
 */
class DE_DLL_EXPORT deeOutOfMemory : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeOutOfMemory( const char *file, int line );
};

/**
 * \brief Out of Boundary Exception.
 */
class DE_DLL_EXPORT deeOutOfBoundary : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeOutOfBoundary( const char *file, int line );
};

/**
 * \brief Empty Stack Exception.
 */
class DE_DLL_EXPORT deeStackEmpty : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeStackEmpty( const char *file, int line );
};

/**
 * \brief Stack Overflow Exception.
 */
class DE_DLL_EXPORT deeStackOverflow : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeStackOverflow( const char *file, int line );
};

/**
 * \brief Division by Zero Exception.
 */
class DE_DLL_EXPORT deeDivisionByZero : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeDivisionByZero( const char *file, int line );
};

/**
 * \brief Null Pointer Exception.
 */
class DE_DLL_EXPORT deeNullPointer : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeNullPointer( const char *file, int line, const char *description = NULL );
};

/**
 * \brief Invalid Action Exception.
 */
class DE_DLL_EXPORT deeInvalidAction : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeInvalidAction( const char *file, int line, const char *description = NULL );
};

/**
 * \brief Invalid Format Exception.
 */
class DE_DLL_EXPORT deeInvalidFormat : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeInvalidFormat( const char *file, int line, const char *description = NULL );
};

/**
 * \brief File not found Exception.
 */
class DE_DLL_EXPORT deeFileNotFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeFileNotFound( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief File does already exist Exception.
 */
class DE_DLL_EXPORT deeFileExists : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeFileExists( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief File can not be opened Exception.
 */
class DE_DLL_EXPORT deeOpenFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeOpenFile( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief File can not be read Exception.
 */
class DE_DLL_EXPORT deeReadFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeReadFile( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief File can not be written Exception.
 */
class DE_DLL_EXPORT deeWriteFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeWriteFile( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief Invalid File Format Exception.
 */
class DE_DLL_EXPORT deeInvalidFileFormat : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeInvalidFileFormat( const char *file, int line, const char *path = NULL );
	
private:
	static decString pCreateText( const char *path );
};

/**
 * \brief Directory not found Exception.
 */
class DE_DLL_EXPORT deeDirectoryNotFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeDirectoryNotFound( const char *file, int line );
};

/**
 * \brief Can not read directory Exception.
 */
class DE_DLL_EXPORT deeDirectoryRead : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeDirectoryRead( const char *file, int line );
};

/**
 * \brief Invalid Syntax Exception.
 */
class DE_DLL_EXPORT deeInvalidSyntax : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeInvalidSyntax( const char *file, int line );
};

/**
 * \brief Script Error Exception.
 */
class DE_DLL_EXPORT deeScriptError : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeScriptError( const char *file, int line );
};

/**
 * \brief No Module Found Exception.
 */
class DE_DLL_EXPORT deeNoModuleFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeNoModuleFound( const char *file, int line );
};

/**
 * \brief Test failed Exception.
 */
class DE_DLL_EXPORT deeTestFailed : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeTestFailed( const char *file, int line, const char *description = NULL );
};

/**
 * \brief Assertion Exception.
 */
class DE_DLL_EXPORT deeAssertion : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If NULL set to a default string.
	 */
	deeAssertion( const char *file, int line, const char *description = NULL );
};


/**
 * \brief Throw an exception of the given type.
 * 
 * The type specified has to be an exception class subclassing deException. The file
 * and line information are obtained from the location the macro is expanded.
 * 
 * \param cls Class name of the exception to throw
 */
#define DETHROW( cls ) throw cls( __FILE__, __LINE__ )

/**
 * \brief Throw an exception of the given type with additional construction parameters.
 * 
 * The type specified has to be an exception class subclassing deException. The file
 * and line information are obtained from the location the macro is expanded.
 * 
 * \param cls Class name of the exception to throw
 */
#define DETHROWX( cls, ... ) throw cls( __FILE__, __LINE__, __VA_ARGS__ )

/**
 * \brief Throw an exception of the given type with additional information.
 * 
 * The type specified has to be an exception class subclassing deException. The file and line
 * information are obtained from the location the macro is expanded. In additiona the
 * information field of the exception is assigned the constant string 'info'.
 * 
 * \param cls Class name of the exception to throw
 * \param info Constant string to assign to the exception object as additional information
 */
#define DETHROW_INFO( cls, info ) throw cls( __FILE__, __LINE__, info )

/**
 * \brief Throw deeNullPointer exception if pointer is nullptr.
 * \version 1.10
 */
#define DEASSERT_NOTNULL( pointer ) if( ! ( pointer ) ) DETHROW_INFO( deeNullPointer, "assertNotNull(" #pointer ")" );

/**
 * \brief Throw deeInvalidParam exception if pointer is not nullptr.
 * \version 1.10
 */
#define DEASSERT_NULL( pointer ) if( pointer ) DETHROW_INFO( deeInvalidParam, "assertNull(" #pointer ")" );

/**
 * \brief Throw deeInvalidParam exception if condition is false.
 * \version 1.10
 */
#define DEASSERT_TRUE( condition ) if( ! ( condition ) ) DETHROW_INFO( deeInvalidParam, "assertTrue(" #condition ")" );

/**
 * \brief Throw deeInvalidParam exception if condition is true.
 * \version 1.10
 */
#define DEASSERT_FALSE( condition ) if( condition ) DETHROW_INFO( deeInvalidParam, "assertFalse(" #condition ")" );





/**
 * \brief If precondition is true throw deeNullPointer exception if pointer is nullptr.
 * \version 1.25
 */
#define DEASSERT_NOTNULL_IF(precondition, pointer) if((precondition) && !(pointer)) \
	DETHROW_INFO(deeNullPointer, "assertNotNull(" #precondition " -> " #pointer ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if pointer is not nullptr.
 * \version 1.25
 */
#define DEASSERT_NULL_IF(precondition, pointer) if((precondition) && (pointer)) \
	DETHROW_INFO(deeInvalidParam, "assertNull(" #precondition " -> " #pointer ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if condition is false.
 * \version 1.25
 */
#define DEASSERT_TRUE_IF(precondition, condition) if((precondition) && !(condition)) \
	DETHROW_INFO(deeInvalidParam, "assertTrue(" #precondition " -> " #condition ")");

/**
 * \brief If precondition is true throw deeInvalidParam exception if condition is true.
 * \version 1.25
 */
#define DEASSERT_FALSE_IF(precondition, condition) if((precondition) && (condition)) \
	DETHROW_INFO(deeInvalidParam, "assertFalse(" #precondition " -> " #condition ")");

#endif
