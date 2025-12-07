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

#ifndef _DEDSENGINEEXCEPTION_H_
#define _DEDSENGINEEXCEPTION_H_

#include <libdscript/exceptions.h>
#include <dragengine/common/exceptions.h>


class dedsEngineException : public duException{
private:
	const char * const pStrDescription;
	const char * const pStrFile;
	dedsEngineException(const char *description, const char *file, int line);
	
public:
	~dedsEngineException() override;
	static dedsEngineException Wrap(const deException &exception);
};

/**
 * Run code line in try/catch wrapping engine exceptions in dedsEngineException.
 * The exception trace is printed to console for debugging purpose
 */
#define DS_WITH_ENGEX(ds,...) try{\
		__VA_ARGS__\
	}catch(const deException &e){\
		ds.LogException(e);\
		throw dedsEngineException::Wrap(e);\
	}

#endif
