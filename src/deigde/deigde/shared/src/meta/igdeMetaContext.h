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

#ifndef _IGDEMETACONTEXT_H_
#define _IGDEMETACONTEXT_H_

#include "igdeTMetaData.h"
#include "property/igdeMetaProperty.h"
#include "../gui/resources/igdeIcon.h"
#include "../utils/igdeTListenerList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class igdeUndoSystem;
class igdeClipboard;


/**
 * \brief Meta context.
 * 
 * Meta contexts contain properties of some object or part of an editor. These properties are
 * typically shown in a property panel in the UI with unified look. Editors can provide multiple
 * meta contexts at the same time. Each context is shown as a separate section in the property
 * panel. Contexts use an identifier to track which section is the active one for each editor.
 */
class DE_DLL_EXPORT igdeMetaContext : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaContext>;
	
	/** \brief Context data. */
	using Data = igdeTMetaData<decTObjectOrderedSet<igdeMetaContext>>;
	
	/** \brief List of properties. */
	using PropertyList = igdeTMetaData<decTObjectOrderedSet<igdeMetaProperty>>;
	
	
	/** \brief Listener. */
	class DE_DLL_EXPORT Listener : public deObject{
	public:
		/** \brief Reference type. */
		using Ref = deTObjectReference<Listener>;
		
		/** \brief Create listener. */
		Listener();
		
	protected:
		/** \brief Destructor. */
		virtual ~Listener();
		
	public:
		/** \brief Properties changed. */
		virtual void OnPropertiesChanged(igdeMetaContext *context);
	};
	
	
private:
	decString pIdentifier, pLabel, pDescription;
	igdeIcon::Ref pIcon;
	PropertyList::Ref pProperties;
	igdeTListenerList<Listener> pListeners;
	bool pDisposed;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta context. */
	explicit igdeMetaContext(const char *identifier);
	
	/** \brief Create meta context. */
	igdeMetaContext(const char *identifier, const char *label, const char *description,
		const igdeIcon::Ref &icon, const PropertyList::Ref &properties);
	
	/** \brief Create meta context. */
	igdeMetaContext(const char *identifier, const char *translationTag,
		const igdeIcon::Ref &icon, const PropertyList::Ref &properties);
	
	/** \brief Create copy of meta context. */
	igdeMetaContext(const igdeMetaContext &copy);
	
	igdeMetaContext(igdeMetaContext &&move) = delete;
	igdeMetaContext &operator=(const igdeMetaContext &copy) = delete;
	igdeMetaContext &operator=(igdeMetaContext &&move) = delete;
	
protected:
	/** \brief Clean up meta context. */
	~igdeMetaContext() override;
	
public:
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dispose meta context. */
	void Dispose();
	
	/** \brief Meta context has been disposed. */
	inline bool IsDisposed() const{ return pDisposed; }
	
	/** \brief Context is valid. */
	virtual bool IsValid() const;
	
	
	/** \brief Get identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Get label. */
	inline const decString &GetLabel() const{ return pLabel; }
	
	/** \brief Set label. */
	void SetLabel(const char *label);
	
	/** \brief Get description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Get icon. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon. */
	void SetIcon(const igdeIcon::Ref &icon);
	
	/** \brief Properties. */
	inline const PropertyList::Ref &GetProperties() const{ return pProperties; }
	
	/** \brief Set properties. */
	void SetProperties(const PropertyList::Ref &properties);
	
	/** \brief Environment. */
	virtual igdeEnvironment &GetEnvironment() const = 0;
	
	/** \brief Undo system or nullptr to apply actions immediately. */
	virtual igdeUndoSystem *GetUndoSystem() const;
	
	/** \brief Clipboard. */
	virtual igdeClipboard &GetClipboard() const;
	
	/** \brief Listeners. */
	inline igdeTListenerList<Listener> &GetListeners(){ return pListeners; }
	inline const igdeTListenerList<Listener> &GetListeners() const{ return pListeners; }
	
	/** \brief Notify listeners about properties change. */
	void NotifyPropertiesChanged();
	/*@}*/
};


/**
 * \brief Meta context template for classes like igdeEditableEntity.
 * 
 * The meta context template stores a pointer to the object owning the meta context. Upon capturing
 * a strong reference is stored in addition to keep the owner alive. Convenience functions are
 * provided to obtain the owner reference throwing an exception if nullptr. It is possible to
 * use nullptr as owner to create a null meta context. The meta properties to use are set in the
 * constructor. The properties are passed on to the captured meta context.
 * 
 * The type T is required to expose functions GetEnvironment(), GetUndoSystem().
 */
template<typename T>
class igdeMetaContextType : public igdeMetaContext{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaContextType<T>>;
	
	/** \brief Owner type. */
	using OwnerType = T;
	
private:
	T *pOwner;
	deTObjectReference<T> pGuardOwner;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, const char *label, const char *description,
		const igdeIcon::Ref &icon, const PropertyList::Ref &properties, T *owner) :
	igdeMetaContext(identifier, label, description, icon, properties),
	pOwner(owner){}
	
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, const char *translationTag,
		const igdeIcon::Ref &icon, const PropertyList::Ref &properties, T *owner) :
	igdeMetaContext(identifier, translationTag, icon, properties),
	pOwner(owner){}
	
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, const char *label, const char *description,
		const PropertyList::Ref &properties, T *owner) :
	igdeMetaContextType(identifier, label, description, igdeIcon::Ref(), properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, const char *translationTag,
		const PropertyList::Ref &properties, T *owner) :
	igdeMetaContextType(identifier, translationTag, igdeIcon::Ref(), properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, const PropertyList::Ref &properties, T *owner) :
	igdeMetaContextType(identifier, "", "", igdeIcon::Ref(), properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextType(const char *identifier, T *owner) :
	igdeMetaContextType(identifier, "", "", igdeIcon::Ref(), PropertyList::Ref(), owner){}
	
	/** \brief Create copy of type meta context. */
	igdeMetaContextType(const igdeMetaContextType &copy) :
	igdeMetaContext(copy),
	pOwner(copy.pOwner){}
	
protected:
	/** \brief Clean up type meta context. */
	~igdeMetaContextType() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Owner or nullptr. */
	inline T *GetOwner() const{ return pOwner; }
	
	/** \brief Owner reference throwing exception if nullptr. */
	T &GetOwnerRef() const{
		DEASSERT_NOTNULL(pOwner)
		return *pOwner;
	}
	
	/** \brief Context is valid. */
	bool IsValid() const override {
		return igdeMetaContext::IsValid() && pOwner;
	}
	
	/** \brief Capture context. */
	virtual Ref Capture() const{
		auto context = Copy();
		context->pGuardOwner = pOwner;
		return context;
	}
	
	/** \brief Environment. */
	igdeEnvironment &GetEnvironment() const override {
		return GetOwnerRef().GetEnvironment();
	}
	
	/** \brief Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override {
		return GetOwnerRef().GetUndoSystem();
	}
	/*@}*/
	
	
protected:
	/** \brief Create copy of meta context. */
	virtual Ref Copy() const{
		return Ref::New(*this);
	}
};


/**
 * \brief Meta context template for classes like igdeEditableEntity inheriting from igdeMetaContextType.
 * 
 * The type S is required to be a subclass of T.
 */
template<typename S, typename T>
class igdeMetaContextTypeInherit : public igdeMetaContextType<T>{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaContextTypeInherit<S, T>>;
	
	/** \brief Owner type. */
	using OwnerType = S;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create type meta context. */
	igdeMetaContextTypeInherit(const char *identifier, const char *label, const char *description,
		const igdeIcon::Ref &icon, const igdeMetaContext::PropertyList::Ref &properties, S *owner) :
	igdeMetaContextType<T>(identifier, label, description, icon, properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextTypeInherit(const char *identifier, const char *label, const char *description,
		const igdeMetaContext::PropertyList::Ref &properties, S *owner) :
	igdeMetaContextType<T>(identifier, label, description, {}, properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextTypeInherit(const char *identifier, const igdeMetaContext::PropertyList::Ref &properties, S *owner) :
	igdeMetaContextType<T>(identifier, "", "", {}, properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextTypeInherit(const char *identifier, S *owner) :
	igdeMetaContextType<T>(identifier, "", "", {}, {}, owner){}
	
	/** \brief Create copy of type meta context. */
	igdeMetaContextTypeInherit(const igdeMetaContextTypeInherit &copy) :
	igdeMetaContextType<T>(copy){}
	
protected:
	/** \brief Clean up type meta context. */
	~igdeMetaContextTypeInherit() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Owner or nullptr. */
	inline S *GetOwner() const{ return dynamic_cast<S*>(igdeMetaContextType<T>::GetOwner()); }
	
	/** \brief Owner reference throwing exception if nullptr. */
	S &GetOwnerRef() const{
		DEASSERT_NOTNULL(igdeMetaContextType<T>::GetOwner())
		return *dynamic_cast<S*>(igdeMetaContextType<T>::GetOwner());
	}
	/*@}*/
	
	
protected:
	/** \brief Create copy of meta context. */
	virtual igdeMetaContextType<T>::Ref Copy() const{
		return Ref::New(*this);
	}
};


/**
 * \brief Custom meta context template.
 * 
 * Same as igdeMetaContextType but type T is not required to expose functions GetEnvironment()
 * or GetUndoSystem(). The subclass is responsible to implement the accessors using GetOwnerRef().
 */
template<typename T>
class igdeMetaContextTypeCustom : public igdeMetaContext{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaContextTypeCustom<T>>;
	
	
private:
	T *pOwner;
	deTObjectReference<T> pGuardOwner;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create type meta context. */
	igdeMetaContextTypeCustom(const char *identifier, const char *label, const char *description,
		const igdeIcon::Ref &icon, const PropertyList::Ref &properties, T *owner) :
	igdeMetaContext(identifier, label, description, icon, properties),
	pOwner(owner){
	}
	
	/** Create type meta context. */
	igdeMetaContextTypeCustom(const char *identifier, const PropertyList::Ref &properties, T *owner) :
	igdeMetaContextTypeCustom(identifier, "", "", {}, properties, owner){}
	
	/** Create type meta context. */
	igdeMetaContextTypeCustom(const char *identifier, T *owner) :
	igdeMetaContextTypeCustom(identifier, "", "", {}, {}, owner){}
	
	/** \brief Create copy of type meta context. */
	igdeMetaContextTypeCustom(const igdeMetaContextTypeCustom &copy) :
	igdeMetaContext(copy),
	pOwner(copy.pOwner){
	}
	
protected:
	/** \brief Clean up type meta context. */
	~igdeMetaContextTypeCustom() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Owner or nullptr. */
	inline T *GetOwner() const{ return pOwner; }
	
	/** \brief Owner reference throwing exception if nullptr. */
	T &GetOwnerRef() const{
		DEASSERT_NOTNULL(pOwner)
		return *pOwner;
	}
	
	/** \brief Context is valid. */
	virtual bool IsValid() const override {
		return igdeMetaContext::IsValid() && pOwner;
	}
	
	/** \brief Capture context. */
	virtual Ref Capture() const{
		auto context = Copy();
		context->pGuardOwner = pOwner;
		return context;
	}
	/*@}*/
	
	
protected:
	/** \brief Create copy of meta context. */
	virtual Ref Copy() const{
		return Ref::New(*this);
	}
};


/**
 * \brief Meta property template for use with igdeMetaContextType*.
 * 
 * Type T is the meta property type used as base class. Type C is the meta context type and
 * is best igdeMetaContextType, igdeMetaContextTypeInherit or igdeMetaContextTypeCustom.
 */
template <typename T, typename C>
class igdeMetaPropertyMCT : public T{
public:
	template <typename... A>
	igdeMetaPropertyMCT(A&&... args) : T(std::forward<A>(args)...) {}
	igdeMetaPropertyMCT() = delete;
	igdeMetaPropertyMCT(const igdeMetaPropertyMCT &copy) = delete;
	igdeMetaPropertyMCT(igdeMetaPropertyMCT &&move) = delete;
	
protected:
	~igdeMetaPropertyMCT() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<C>()->Capture();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<C>();
		return c && c->IsValid();
	}
	
	inline typename C::OwnerType &Owner(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<C>()->GetOwnerRef();
	}
};


/**
 * \brief Meta property template for use with igdeMetaContextType* without Capture().
 * 
 * Type T is the meta property type used as base class. Type C is the meta context type and
 * is best igdeMetaContextType, igdeMetaContextTypeInherit or igdeMetaContextTypeCustom.
 * 
 * Use this template instead of igdeMetaPropertyMCT if the property does not define Capture().
 */
template <typename T, typename C>
class igdeMetaPropertyMCTNoCapture : public T{
public:
	template <typename... A>
	igdeMetaPropertyMCTNoCapture(A&&... args) : T(std::forward<A>(args)...) {}
	
protected:
	~igdeMetaPropertyMCTNoCapture() override{}
	
public:
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<C>();
		return c && c->IsValid();
	}
	
	inline typename C::OwnerType &Owner(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<C>()->GetOwnerRef();
	}
};

#endif
