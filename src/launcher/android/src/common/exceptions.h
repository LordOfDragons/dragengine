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
class deeInvalidParam : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeInvalidParam( const char *file, int line );
};

/**
 * \brief Out of memory exception.
 */
class deeOutOfMemory : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeOutOfMemory( const char *file, int line );
};

/**
 * \brief Out of Boundary Exception.
 */
class deeOutOfBoundary : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeOutOfBoundary( const char *file, int line );
};

/**
 * \brief Empty Stack Exception.
 */
class deeStackEmpty : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeStackEmpty( const char *file, int line );
};

/**
 * \brief Stack Overflow Exception.
 */
class deeStackOverflow : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeStackOverflow( const char *file, int line );
};

/**
 * \brief Division by Zero Exception.
 */
class deeDivisionByZero : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeDivisionByZero( const char *file, int line );
};

/**
 * \brief Null Pointer Exception.
 */
class deeNullPointer : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeNullPointer( const char *file, int line );
};

/**
 * \brief Invalid Action Exception.
 */
class deeInvalidAction : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeInvalidAction( const char *file, int line );
};

/**
 * \brief Invalid Format Exception.
 */
class deeInvalidFormat : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeInvalidFormat( const char *file, int line );
};

/**
 * \brief File not found Exception.
 */
class deeFileNotFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeFileNotFound( const char *file, int line );
};

/**
 * \brief File does already exist Exception.
 */
class deeFileExists : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeFileExists( const char *file, int line );
};

/**
 * \brief File can not be opened Exception.
 */
class deeOpenFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeOpenFile( const char *file, int line );
};

/**
 * \brief File can not be read Exception.
 */
class deeReadFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeReadFile( const char *file, int line );
};

/**
 * \brief File can not be written Exception.
 */
class deeWriteFile : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeWriteFile( const char *file, int line );
};

/**
 * \brief Invalid File Format Exception.
 */
class deeInvalidFileFormat : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeInvalidFileFormat( const char *file, int line );
};

/**
 * \brief Directory not found Exception.
 */
class deeDirectoryNotFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeDirectoryNotFound( const char *file, int line );
};

/**
 * \brief Can not read directory Exception.
 */
class deeDirectoryRead : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeDirectoryRead( const char *file, int line );
};

/**
 * \brief Invalid Syntax Exception.
 */
class deeInvalidSyntax : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeInvalidSyntax( const char *file, int line );
};

/**
 * \brief Script Error Exception.
 */
class deeScriptError : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeScriptError( const char *file, int line );
};

/**
 * \brief No Module Found Exception.
 */
class deeNoModuleFound : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeNoModuleFound( const char *file, int line );
};

/**
 * \brief Test failed Exception.
 */
class deeTestFailed : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeTestFailed( const char *file, int line );
};

/**
 * \brief Assertion Exception.
 */
class deeAssertion : public deException{
public:
	/**
	 * \brief Create a new exception object.
	 * \param[in] info Additional information helpfull to track the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deeAssertion( const char *file, int line );
};



/**
 * \brief Throw an exception of the given type.
 * \details The type specified has to be an exception class subclassing deException. The file
 *          and line information are obtained from the location the macro is expanded.
 * \param cls Class name of the exception to throw
 */
#define DETHROW( cls ) throw cls( __FILE__, __LINE__ )

/**
 * \brief Throw an exception of the given type with additional construction parameters.
 * \details The type specified has to be an exception class subclassing deException. The file
 *          and line information are obtained from the location the macro is expanded.
 * \param cls Class name of the exception to throw
 */
#define DETHROWX( cls, ... ) throw cls( __FILE__, __LINE__, __VA_ARGS__ )

/**
 * \brief Throw an exception of the given type with additional information.
 * 
 * The type specified has to be an exception class subclassing deException. The file and line
 * information are obtained from the location the macro is expanded. In additiona the
 * information field of the exception is assigned the constant string 'info'.
 * \param cls Class name of the exception to throw
 * \param info Constant string to assign to the exception object as
 *             additional information
 */
#define DETHROW_INFO( cls, info ) throw cls( __FILE__, __LINE__ )

#endif
