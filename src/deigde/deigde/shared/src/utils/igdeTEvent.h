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

#ifndef _IGDETEVENT_H_
#define _IGDETEVENT_H_

#include <functional>


/**
 * \brief Event template allowing to store an event function with arbitrary arguments.
 */
template<typename... Args>
class igdeTEvent{
private:
	std::function<void(Args...)> pFunction;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create event. */
	igdeTEvent() = default;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Function to call or nullptr. */
	inline const std::function<void(Args...)> &Get() const{ return pFunction; }
	
	/** \brief Set function to call or nullptr to clear. */
	template <typename F>
	void Set(F&& func){
		pFunction = std::forward<F>(func);
	}
	
	/** \brief Set function to call or nullptr to clear. */
	template <typename F>
	void Set(const F& func){
		pFunction = func;
	}
	
	/** \brief Set function to call or nullptr to clear. */
	void Set(const igdeTEvent<Args...> &event){
		pFunction = event.pFunction;
	}
	
	/** \brief Set function to call or nullptr to clear. */
	void Set(igdeTEvent<Args...> &&event){
		pFunction = std::move(event.pFunction);
	}
	
	/** \brief Set function to call or nullptr to clear. */
	template <typename F>
	void operator=(F&& func){
		pFunction = std::forward<F>(func);
	}
	
	/** \brief Set function to call or nullptr to clear. */
	template <typename F>
	void operator=(const F& func){
		pFunction = func;
	}
	
	/** \brief Set function to call or nullptr to clear. */
	void operator=(const igdeTEvent<Args...> &event){
		pFunction = event.pFunction;
	}
	
	/** \brief Set function to call or nullptr to clear. */
	void operator=(igdeTEvent<Args...> &event){
		pFunction = event.pFunction;
	}
	
	/** \brief Set function to call or nullptr to clear. */
	void operator=(igdeTEvent<Args...> &&event){
		pFunction = std::move(event.pFunction);
	}
	
	/** \brief Call function if set. */
	void Call(Args... args){
		if(pFunction){
			pFunction(std::forward<Args>(args)...);
		}
	}
	
	/** \brief Call function if set. */
	void operator()(Args... args){
		if(pFunction){
			pFunction(std::forward<Args>(args)...);
		}
	}
	
	/** \brief Function is set. */
	inline bool IsSet() const{
		return static_cast<bool>(pFunction);
	}
	
	/** \brief Function is set. */
	inline operator bool() const{
		return static_cast<bool>(pFunction);
	}
	
	/** \brief Function is not set. */
	inline bool IsNotSet() const{
		return !static_cast<bool>(pFunction);
	}
	
	/** \brief Function is not set. */
	inline bool operator!() const{
		return !static_cast<bool>(pFunction);
	}
	/*@}*/
};

#endif
