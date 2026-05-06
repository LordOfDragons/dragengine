/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWAYLANDHELPER_H_
#define _DEWAYLANDHELPER_H_

#include "../../dragengine_configuration.h"

#if defined(OS_UNIX_X11) && defined(OS_UNIX_WAYLAND)

#include "../../common/string/decString.h"
#include "../../common/exceptions.h"

#include <iostream>
#include <dlfcn.h>
#include <utility>

#include <wayland-client.h>


/**
 * \brief Wayland dynamic library.
 */
class deWlDynLib{
private:
	decString pSource, pName;
	void *pHandle = nullptr;
	
public:
	/**
	 * \brief Load library if present.
	 * \param[in] source Source of the library. Used for error messages.
	 * \param[in] name Base name of the library. Tries loading "lib{name}.so.{majorVersion}" or "lib{name}.so".
	 */
	deWlDynLib(const char *source, const char *name, int majorVersion);
	
	/** \brief Close library if open. */
	~deWlDynLib();
	
	/** \brief Source. */
	inline const decString &GetSource() const{ return pSource; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Library is loaded. */
	inline bool IsValid() const{ return pHandle != nullptr; }
	inline operator bool() const{ return IsValid(); }
	
	/** \brief Library is not loaded. */
	inline bool IsInvalid() const{ return !IsValid(); }
	inline bool operator!() const{ return !IsValid(); }
	
	/** \brief Throw exception if library is not loaded. */
	void Required();
	
	/** \brief Library is loaded printing error to console if not. */
	bool RequiredLog();
	
	/** \brief Library is loaded logging error if not. */
	template<typename L> bool RequiredLog(L &logger){
		if(!pHandle){
			logger.LogErrorFormat("lib%s not found", pName.GetString());
		}
		return pHandle != nullptr;
	}
	
	/** \brief Resolve symbol or nullptr if not found. */
	void *Resolve(const char *symbol);
	
	/** \brief Resolve symbol throwing exception if not found. */
	void *ResolveRequired(const char *symbol);
};


/**
 * \brief Wayland dynamic library function.
 */
template<typename T> class deWlDynLibFunc;

template<typename R, typename... Args>
class deWlDynLibFunc<R(Args...)>{
public:
	/** \brief Function pointer type. */
	using FuncPtr = R(*)(Args...);
	
private:
	deWlDynLib &pLibrary;
	decString pSymbol;
	FuncPtr pFunction = nullptr;
	
public:
	/** \brief Resolve dynamic library function if possible. */
	deWlDynLibFunc(deWlDynLib &library, const char *symbol) : pLibrary(library), pSymbol(symbol){
		if(library.IsValid()){
			pFunction = reinterpret_cast<FuncPtr>(library.Resolve(symbol));
		}
	}
	
	/** \brief Library. */
	inline deWlDynLib &GetLibrary() const{ return pLibrary; }
	
	/** \brief Symbol. */
	inline const decString &GetSymbol() const{ return pSymbol; }
	
	/** \brief Function is resolved. */
	inline bool IsValid() const{ return pFunction != nullptr; }
	inline operator bool() const{ return IsValid(); }
	
	/** \brief Function is not resolved. */
	inline bool IsInvalid() const{ return !IsValid(); }
	inline bool operator!() const{ return !IsValid(); }
	
	/** \brief Throw exception if function is not resolved. */
	void Required() const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Function '{0}' not found in lib{1}", pSymbol.GetString(),
					pLibrary.GetName().GetString()));
				
			}else{
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Can not resolve function '{0}', lib{1} not loaded",
					pSymbol.GetString(), pLibrary.GetName().GetString()));
			}
		}
	}
	
	/** \brief Function is resolved printing error if not. */
	bool RequiredLog() const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				printf("[%s] Function '%s' not found in lib%s\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
				
			}else{
				printf("[%s] Can not resolve function '%s', lib%s not loaded\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
			}
		}
		return pFunction != nullptr;
	}
	
	/** \brief Function is resolved logging error if not. */
	template<typename L> bool RequiredLog(L &logger) const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Function '%s' not found in library lib%s\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
				
			}else{
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Can not resolve function '%s', library lib%s not loaded\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
			}
		}
		return pFunction != nullptr;
	}
	
	/** \brief Call function throwing exception if not resolved. */
	R operator()(Args... args) const{
		if(!pFunction){
			DETHROW_INFO(deeInvalidParam, decString::Formatted(
				"Can not call function '{0}' from library lib{1}, function not resolved",
				pSymbol.GetString(), pLibrary.GetName().GetString()));
		}
		return pFunction(std::forward<Args>(args)...);
	}
	
	/** \brief Function pointer or nullptr if not resolved. */
	inline FuncPtr Get() const{ return pFunction; }
	inline operator FuncPtr() const{ return Get(); }
};


template<typename R, typename... Args>
class deWlDynLibFunc<R(Args..., ...)>{
public:
	/** \brief Function pointer type. */
	using FuncPtr = R(*)(Args..., ...);
	
private:
	deWlDynLib &pLibrary;
	decString pSymbol;
	FuncPtr pFunction = nullptr;
	
public:
	/** \brief Create dynamic library function wrapper. */
	deWlDynLibFunc(deWlDynLib &library, const char *symbol) : pLibrary(library), pSymbol(symbol){
		if(library.IsValid()){
			pFunction = reinterpret_cast<FuncPtr>(library.Resolve(symbol));
		}
	}
	
	/** \brief Library. */
	inline deWlDynLib &GetLibrary() const{ return pLibrary; }
	
	/** \brief Symbol. */
	inline const decString &GetSymbol() const{ return pSymbol; }
	
	/** \brief Function is resolved. */
	inline bool IsValid() const{ return pFunction != nullptr; }
	inline operator bool() const{ return IsValid(); }
	
	/** \brief Function is not resolved. */
	inline bool IsInvalid() const{ return !IsValid(); }
	inline bool operator!() const{ return !IsValid(); }
	
	/** \brief Throw exception if function is not resolved. */
	void Required() const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Function '{0}' not found in lib{1}", pSymbol.GetString(),
					pLibrary.GetName().GetString()));
				
			}else{
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Can not resolve function '{0}', lib{1} not loaded",
					pSymbol.GetString(), pLibrary.GetName().GetString()));
			}
		}
	}
	
	/** \brief Function is resolved printing error if not. */
	bool RequiredLog() const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				printf("[%s] Function '%s' not found in lib%s\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
				
			}else{
				printf("[%s] Can not resolve function '%s', lib%s not loaded\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
			}
		}
		return pFunction != nullptr;
	}
	
	/** \brief Function is resolved logging error if not. */
	template<typename L> bool RequiredLog(L &logger) const{
		if(!pFunction){
			if(pLibrary.IsValid()){
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Function '%s' not found in library lib%s\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
				
			}else{
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Can not resolve function '%s', library lib%s not loaded\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
			}
		}
		return pFunction != nullptr;
	}
	
	/** \brief Call function throwing exception if not resolved. */
	template<typename... VarArgs>
	R operator()(Args... args, VarArgs... varargs) const{
		if(!pFunction){
			DETHROW_INFO(deeInvalidParam, decString::Formatted(
				"Can not call function '{0}' from library lib{1}, function not resolved",
				pSymbol.GetString(), pLibrary.GetName().GetString()));
		}
		return pFunction(args..., varargs...);
	}
	
	/** \brief Function pointer or nullptr if not resolved. */
	inline FuncPtr Get() const{ return pFunction; }
	inline operator FuncPtr() const{ return Get(); }
};


/**
 * \brief Wayland dynamic library constant data struct.
 */
template<typename T> class deWlDynLibData;

template<typename T>
class deWlDynLibData{
private:
	deWlDynLib &pLibrary;
	decString pSymbol;
	T pData{};
	T *pResolvedData = nullptr;
	
public:
	/** \brief Resolve dynamic library constant data struct wrapper. */
	deWlDynLibData(deWlDynLib &library, const char *symbol) : pLibrary(library), pSymbol(symbol){
		if(library.IsValid()){
			pResolvedData = reinterpret_cast<T*>(library.Resolve(symbol));
			if(pResolvedData){
				pData = *pResolvedData;
			}
		}
	}
	
	/** \brief Library. */
	inline deWlDynLib &GetLibrary() const{ return pLibrary; }
	
	/** \brief Symbol. */
	inline const decString &GetSymbol() const{ return pSymbol; }
	
	/** \brief Data is resolved. */
	inline bool IsValid() const{ return pResolvedData != nullptr; }
	inline operator bool() const{ return IsValid(); }
	
	/** \brief Data is not resolved. */
	inline bool IsInvalid() const{ return !IsValid(); }
	inline bool operator!() const{ return !IsValid(); }
	
	/** \brief Throw exception if data is not resolved. */
	void Required() const{
		if(!pResolvedData){
			if(pLibrary.IsValid()){
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Data '{0}' not found in lib{1}", pSymbol.GetString(),
					pLibrary.GetName().GetString()));
				
			}else{
				DETHROW_INFO(deeInvalidParam, decString::Formatted(
					"Can not resolve data '{0}', lib{1} not loaded",
					pSymbol.GetString(), pLibrary.GetName().GetString()));
			}
		}
	}
	
	/** \brief Data is resolved printing error if not. */
	bool RequiredLog() const{
		if(!pResolvedData){
			if(pLibrary.IsValid()){
				printf("[%s] Data '%s' not found in lib%s\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
				
			}else{
				printf("[%s] Can not resolve data '%s', lib%s not loaded\n",
					pLibrary.GetSource().GetString(), pSymbol.GetString(),
					pLibrary.GetName().GetString());
			}
		}
		return pResolvedData != nullptr;
	}
	
	/** \brief Data is resolved logging error if not. */
	template<typename L> bool RequiredLog(L &logger) const{
		if(!pResolvedData){
			if(pLibrary.IsValid()){
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Data '%s' not found in library lib%s\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
				
			}else{
				logger.LogErrorFormat(pLibrary.GetSource(),
					"Can not resolve data '%s', library lib%s not loaded\n",
					pSymbol.GetString(), pLibrary.GetName().GetString());
			}
		}
		return pResolvedData != nullptr;
	}
	
	/**
	 * \brief Get data pointer.
	 * 
	 * Data is initialized to default value if not resolved yet.
	 */
	const T* Get() const{ return &pData; }
	inline operator const T*() const{ return Get(); }
	
	/** \brief Access data. */
	inline const T& operator*() const{ return *Get(); }
	inline const T* operator->() const{ return Get(); }
};


/** \brief Wayland manager. */
template<typename T> class deWaylandManager{
private:
	const wl_interface &pInterface;
	int pVersion;
	int pBoundVersion = 0;
	void (*pDestroyFunc)(T*) = nullptr;
	T *pManager = nullptr;
	uint32_t pName = 0;
	
	
public:
	/** \brief Create manager. */
	deWaylandManager(const wl_interface &interface, int version, void (*destroyFunc)(T*)) :
	pInterface(interface), pVersion(version), pDestroyFunc(destroyFunc){
	}
	
	/** \brief Destroy manager. */
	virtual ~deWaylandManager(){
		Unbind();
	}
	
	/** \brief Interface. */
	inline const wl_interface &GetInterface() const{ return pInterface; }
	
	/** \brief Version. */
	inline int GetVersion() const{ return pVersion; }
	
	/** \brief Bound version. */
	inline int GetBoundVersion() const{ return pBoundVersion; }
	
	/** \brief Manager. */
	inline T* GetManager() const{ return pManager; }
	inline operator T*() const{ return GetManager(); }
	
	/** \brief Is bound. */
	inline bool IsBound() const{ return pManager != nullptr; }
	inline operator bool() const{ return IsBound(); }
	
	/**
	 * \brief Bind if matching and not bound.
	 * \return True if bound, false if not matching or already bound.
	 */
	bool Bind(wl_registry *registry, uint32_t name, const char *interface, uint32_t version){
		if(pManager || strcmp(interface, pInterface.name) != 0){
			return false;
		}
		
		const int bindVersion = decMath::min(pVersion, version);
		pManager = reinterpret_cast<T*>(wl_registry_bind(registry, name, &pInterface, bindVersion));
		if(!pManager){
			return false;
		}
		
		pName = name;
		pBoundVersion = bindVersion;
		OnBound();
		return true;
	}
	
	/** \brief Unbind. */
	void Unbind(){
		if(!pManager){
			return;
		}
		
		OnUnbound();
		pDestroyFunc(pManager);
		pManager = nullptr;
		pName = 0;
		pBoundVersion = 0;
	}
	
	
protected:
	/** \brief Manager bound. */
	virtual void OnBound(){}
	
	/** \brief Manager will be unbound. */
	virtual void OnUnbound(){}
};


#endif /* OS_UNIX_WAYLAND */
#endif /* _DEWAYLANDHELPER_H_ */
