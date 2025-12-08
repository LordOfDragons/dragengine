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

#ifndef _DEBASEMODULE_H_
#define _DEBASEMODULE_H_

#include "../../common/string/decString.h"

#include "../../filesystem/deVirtualFileSystem.h"
class deEngine;
class deOS;
class deModuleParameter;
class deLoadableModule;
class deErrorTracePoint;
class deException;
class decPath;
class decUnicodeString;
class decUnicodeArgumentList;


/**
 * \brief Base Module Class.
 * 
 * Modules are the basic building stone of the entire engine. Each system
 * has at least one module providing specilized functionality in the given
 * set of tasks. Each module can be customized with the use of module
 * parameters. Each parameter is a decimal value which can have though
 * different meanings. The parameter information object describes what
 * values are acceptable by a certain parameter. This allows GUI scripts
 * to provide appropriate input widgets to let the user alter the values.
 * Parameters are required to be error safe. If a certain value is not
 * accepted it has to be converted to the nearest feasible one.
 */
class DE_DLL_EXPORT deBaseModule{
private:
	deLoadableModule &pLoadableModule; // loadable module hosting us
	deVirtualFileSystem::Ref pVFS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base module. */
	deBaseModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up base module. */
	virtual ~deBaseModule();
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	virtual void GetParameterInfo(int index, deModuleParameter &parameter) const;
	
	/** \brief Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed(const char *name) const;
	
	/** \brief Value of named parameter. */
	virtual decString GetParameterValue(const char *name) const;
	
	/** \brief Numeric value of named parameter. */
	int GetParameterInt(const char *name) const;
	
	/** \brief Numeric value of named parameter. */
	float GetParameterFloat(const char *name) const;
	
	/** \brief Boolean value of named parameter. */
	bool GetParameterBool(const char *name) const;
	
	/** \brief Set value of named parameter. */
	virtual void SetParameterValue(const char *name, const char *value);
	
	/** \brief Set numeric value of named parameter. */
	void SetParameterFloat(const char *name, float value);
	
	/** \brief Set numeric value of named parameter. */
	void SetParameterInt(const char *name, int value);
	
	/** \brief Set boolean value of named parameter. */
	void SetParameterBool(const char *name, bool value);
	/*@}*/
	
	
	
	/** \name Parallel Processing */
	/*@{*/
	/**
	 * \brief Parallel processing is pausing and threads are still running.
	 * 
	 * If module has special processing which potentially blocks threads from finishing
	 * running it is given the chances to finish these processings now. This method is
	 * called for all modules repeatedly until all threads successfully finished.
	 * 
	 * The default implementation does nothing.
	 */
	virtual void PauseParallelProcessingUpdate();
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/**
	 * \brief Sends a command to the module and retrieves an answer from it.
	 * 
	 * At least the 'help' command has to be understood answering a
	 * list of possible commands. The command is provided in the 'command'
	 * parameter and the answer has to be written into 'answer'. The default
	 * implementation simply answers only to help with itself.
	 * 
	 * \param[in] command The command to send to the module as a list of arguments.
	 * \param[out] answer Unicode string to hold the answer returned from the module.
	 */
	virtual void SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
	/*@}*/
	
	
	
	/** \name Accessors */
	/*@{*/
	/** \brief Game engine object. */
	deEngine *GetGameEngine() const;
	
	/** \brief Loadable module. */
	deLoadableModule &GetLoadableModule() const;
	
	/** \brief OS from game engine. */
	deOS *GetOS() const;
	
	/**
	 * \brief Module virtual file system.
	 * 
	 * The following directories are available:
	 * 
	 * \par /config
	 * Directory containing module configuration. The directory is writeable. This is an
	 * overlayed directory. A writeable version is layered ontop of a read-only version
	 * containing the module default files. This allows to directly write to configuration
	 * keeping the default configuration intact in case a user needs to look at it.
	 * 
	 * \par /share
	 * Directory containing shared module data. This directory is read-only. Place in this
	 * directory all data you need for running the module that never changes.
	 * 
	 * \par /capture
	 * Directory containing module output data. This directory is writeable. This directory
	 * is used to hold debug output of modules that are written to file. Developer can
	 * retrieve and later examine this data while the engine not running or not anymore.
	 * 
	 * \par /cache/global
	 * Global cache directory for storing cached information related to the operation of
	 * the module in general which is not application specific. The stored files are kept
	 * across restarting the application. If the disk consumption grows old files are
	 * automatically removed without asking first to keep the size limit.
	 * 
	 * \par /cache/local
	 * Local cache directory for storing cached information that is application specific.
	 * The stored files are kept across restarting the application. If the disk consumption
	 * grows old files are automatically removed without asking first to keep the size limit.
	 */
	inline deVirtualFileSystem &GetVFS() const { return *pVFS; }
	
	/**
	 * \brief Native path to read only shares directory.
	 * \version 1.6
	 * 
	 * Always use "/shared" directory inside GetVFS(). This path is provided only for
	 * certain libraries requiring absolute native path. If the shared directory is not
	 * physically present on a hard disk this call returns empty string.
	 */
	decString GetNativePathShare() const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Output information message on console. */
	void LogInfo(const char *message);
	
	/** \brief Output formated information message on console. */
	void LogInfoFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated information message on console. */
	void LogInfoFormatUsing(const char *message, va_list args);
	
	/** \brief Output warning message on console. */
	void LogWarn(const char *message);
	
	/** \brief Output formated warning message on console. */
	void LogWarnFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated warning message on console. */
	void LogWarnFormatUsing(const char *message, va_list args);
	
	/** \brief Output error message on console. */
	void LogError(const char *message);
	
	/** \brief Output formated error message on console. */
	void LogErrorFormat(const char *message, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Output formated error message on console. */
	void LogErrorFormatUsing(const char *message, va_list args);
	
	/** \brief Output exception as error message on console. */
	void LogException(const deException &exception);
	/*@}*/
	
	
	
	/** \name Error Tracing */
	/*@{*/
	/**
	 * \brief Add trace point and sets the error if the trace is empty.
	 * \param[in] error Short description of the error preferably of a single line.
	 * \param[in] sourceFunc Name of the source file where the error occurred.
	 * \param[in] sourceLine The line number where the error occurred.
	 * \return Created trace point or NULL if an error occurred.
	 */
	deErrorTracePoint *SetErrorTrace(const char *error, const char *sourceFunc, int sourceLine);
	
	/**
	 * \brief Add trace point and sets the error if the trace is empty.
	 * \param[in] exception Exception to use as source for the error information.
	 * \return Created trace point or NULL if an error occurred.
	 */
	deErrorTracePoint *SetErrorTrace(const deException &exception);
	
	/**
	 * \brief Add trace point to the engine error trace with an empty error.
	 * \param[in] sourceFunc Name of the source file where the error occurred.
	 * \param[in] sourceLine The line number where the error occurred.
	 */
	deErrorTracePoint *AddErrorTracePoint(const char *sourceFunc, int sourceLine);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateVFS();
};

#endif
