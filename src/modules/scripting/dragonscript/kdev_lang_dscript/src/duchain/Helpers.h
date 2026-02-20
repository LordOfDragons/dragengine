#ifndef _HELPERS_H
#define _HELPERS_H

#include <interfaces/iproject.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/classfunctiondeclaration.h>
#include <language/duchain/declaration.h>
#include <language/duchain/functiondeclaration.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/types/integraltype.h>
#include <language/duchain/types/functiontype.h>
#include <language/duchain/types/structuretype.h>
#include <language/duchain/types/unsuretype.h>

#include <QList>

#include <functional>

#include "duchainexport.h"
#include "dsp_ast.h"


using KDevelop::ClassDeclaration;
using KDevelop::ClassFunctionDeclaration;
using KDevelop::CursorInRevision;
using KDevelop::Declaration;
using KDevelop::DeclarationPointer;
using KDevelop::DUChainPointer;
using KDevelop::DUContext;
using KDevelop::DUContextPointer;
using KDevelop::IndexedDeclaration;
using KDevelop::IndexedString;
using KDevelop::IProject;
using KDevelop::ReferencedTopDUContext;
using KDevelop::TopDUContext;

namespace DragonScript {

class KDEVDSDUCHAIN_EXPORT Helpers {
private:
	static KDevelop::AbstractType::Ptr pTypeVoid;
	static KDevelop::AbstractType::Ptr pTypeNull;
	static KDevelop::AbstractType::Ptr pTypeInvalid;
	
	static KDevelop::AbstractType::Ptr pTypeByte;
	static KDevelop::AbstractType::Ptr pTypeBool;
	static KDevelop::AbstractType::Ptr pTypeInt;
	static KDevelop::AbstractType::Ptr pTypeFloat;
	static KDevelop::AbstractType::Ptr pTypeString;
	static KDevelop::AbstractType::Ptr pTypeObject;
	static KDevelop::AbstractType::Ptr pTypeBlock;
	
	static DeclarationPointer pTypeDeclByte;
	static DeclarationPointer pTypeDeclBool;
	static DeclarationPointer pTypeDeclInt;
	static DeclarationPointer pTypeDeclFloat;
	static DeclarationPointer pTypeDeclString;
	static DeclarationPointer pTypeDeclObject;
	static DeclarationPointer pTypeDeclBlock;
	
	static QStringList dataDirs;
	static QVector<IndexedString> documentationFiles;
	static IndexedString documentationFileObject;
	static QVector<ReferencedTopDUContext> documentationFileContexts;
	static bool documentationFileContextsReady;
	static bool documentationFileContextsParsing;
	
	
	
public:
	/**
	 * \brief Void type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeVoid();
	
	/**
	 * \brief Null type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeNull();
	
	/**
	 * \brief Invalid type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeInvalid();
	
	/**
	 * \brief Byte type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeByte();
	
	/**
	 * \brief Bool type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeBool();
	
	/**
	 * \brief Int type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeInt();
	
	/**
	 * \brief Float type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeFloat();
	
	/**
	 * \brief String type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeString();
	
	/**
	 * \brief Object type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeObject();
	
	/**
	 * \brief Block type.
	 * \note DUChainReadLocker required.
	 */
	static KDevelop::AbstractType::Ptr getTypeBlock();
	
	/**
	 * \brief Byte type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeByte(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief Bool type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeBool(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief Int type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeInt(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief Float type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeFloat(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief String type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeString(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief Object type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeObject(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	/**
	 * \brief Block type.
	 * \note DUChainReadLocker required.
	 */
	static void getTypeBlock(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type);
	
	
	
	/** \brief Internal class context for type or null-context if not found. */
	static DUChainPointer<const DUContext> contextForType(const TopDUContext *top,
		const KDevelop::AbstractType::Ptr &type);
	
	/** \brief This class declaration for context or nullptr if not found. */
	static ClassDeclaration *thisClassDeclFor(DUChainPointer<const DUContext> context);
	
	/** \brief Super class declaration for context or nullptr if not found. */
	static ClassDeclaration *superClassDeclFor(DUChainPointer<const DUContext> context);
	
	/** \brief Class declaration for context or nullptr if not found. */
	static ClassDeclaration *classDeclFor(DUChainPointer<const DUContext> context);
	
	/** \brief Class declaration for type or nullptr if not found. */
	static ClassDeclaration *classDeclFor(const TopDUContext *top,
		const KDevelop::AbstractType::Ptr &type);
	
	/** \brief Super class of class declaration or nullptr if not set. */
	static ClassDeclaration *superClassOf(const TopDUContext *top,
		const ClassDeclaration *declaration);
	
	/** \brief Implementation classes of class declaration. */
	static QVector<ClassDeclaration*> implementsOf(const TopDUContext *top,
		const ClassDeclaration *declaration);
	
	/** \brief All base classes of class declaration. */
	static QVector<ClassDeclaration*> baseClassesOf(const TopDUContext *top,
		const ClassDeclaration *declaration);
	
	/** \brief Two type points are the same. */
	static bool equals(const TopDUContext *top, const KDevelop::AbstractType::Ptr &type1,
		const KDevelop::AbstractType::Ptr &type2);
	
	/** \brief Type is castable to another. */
	static bool castable(const TopDUContext *top, const KDevelop::AbstractType::Ptr &type,
		const KDevelop::AbstractType::Ptr &targetType);
	
	/** \brief Functions have the same signature. */
	static bool sameSignature(const TopDUContext *top, const KDevelop::FunctionType::Ptr &func1,
		const KDevelop::FunctionType::Ptr &func2);
	
	/** \brief Functions have the same signature but potentially different return type. */
	static bool sameSignatureAnyReturnType(const TopDUContext *top,
		const KDevelop::FunctionType::Ptr &func1, const KDevelop::FunctionType::Ptr &func2);
	
	/** \brief Function overrides another. */
	static bool overrides(const TopDUContext *top, const ClassFunctionDeclaration *func1,
		const ClassFunctionDeclaration *func2);
	
	static QStringList getDataDirs();
	
	/** \brief Get documentation files. */
	static const QVector<IndexedString> &getDocumentationFiles();
	
	/**
	 * \brief Get list with documentation file contexts.
	 * 
	 * If language documents are not fully parsed yet returns empty list. In this case
	 * cancel the parsing of the document and schedule it again with a low priority.
	 * \note DUChainReadLocker required.
	 */
	static QVector<ReferencedTopDUContext> getDocumentationFileContexts();
	
	/**
	 * \brief Add documentation file contexts as imports to a top context.
	 * \note DUChainWriteLocker required.
	 * \retval true Successfully added language documentation file contexts.
	 * \retval false Lanugage documentation is not fully parse yet. Cancel parsing of
	 *               document and schedule it again with a low priority.
	 */
	static bool addImportsDocumentationFileContexts(TopDUContext *top);
	
	/** \brief Get documentation file for Object class. */
	static IndexedString getDocumentationFileObject();
	
	/**
	 * \brief Get internal type declaration if loaded.
	 * \note DUChainReadLocker required.
	 */
	static Declaration *getInternalTypeDeclaration(const QString &name);
	
	static KDevelop::AbstractType::Ptr resolveAliasType(const KDevelop::AbstractType::Ptr eventualAlias);
	
	static IndexedDeclaration declarationUnderCursor(bool allowUse = true);
	
	enum ContextSearchFlags {
		NoFlags,
		PublicOnly
	};
	
	static QMutex cacheMutex;
	static QMap<QString, QStringList> cachedIncludeFiles;

	/**
	 * \brief Remove cached include files context for include directory.
	 */
	static void dropIncludeFileContexts(const QString &includeDir);
	
	/**
	 * \brief Add include directory file contexts as includes to a top context.
	 * 
	 * If contexts are ready \em includeDir is filled and \em true returned. \em includeDir can
	 * be an empty list. If one or more contexts is not yet ready returns \em false only.
	 * 
	 * \note DUChainWriteLocker required.
	 */
	static bool addImportsIncludePath(TopDUContext *top, const QString &includeDir);
	
	/**
	 * \brief Get list with include directory contexts.
	 * 
	 * \note DUChainReadLocker required.
	 */
	static QVector<ReferencedTopDUContext> getImportsIncludeContexts(const QString &includeDir);
	
	/**
	 * \brief Find all internal contexts for this class and its base classes recursively
	 *
	 * \param classType Type object for the class to search contexts
	 * \param context TopContext for finding the declarations for types
	 * \return list of contexts which were found
	 **/
	static QVector<DUContext*> internalContextsForClass(
		const KDevelop::StructureType::Ptr classType, const TopDUContext* context,
		ContextSearchFlags flags = NoFlags, int depth = 0);
	
	/**
	 * \brief Resolve the given declaration if it is an alias declaration.
	 *
	 * \param decl the declaration to resolve
	 * \return :Declaration* decl if not an alias declaration, decl->aliasedDeclaration().data otherwise
	 * DUChain must be read locked
	 **/
	static Declaration *resolveAliasDeclaration(Declaration *decl);
	
	/**
	 * \brief Find first matching declaration.
	 * \note DUChainReadLocker required.
	 **/
	static Declaration *declarationForName(const QString& name,
		const CursorInRevision& location, DUChainPointer<const DUContext> context);
	
	/**
	 * \brief Find first matching declaration.
	 * \note DUChainReadLocker required.
	 **/
	static Declaration *declarationForName(const QString& name,
		const CursorInRevision& location, DUChainPointer<const DUContext> context,
		const QVector<DUChainPointer<const DUContext>> &pinned);
	
	/**
	 * \brief Find first matching declaration in base classes only.
	 * \note DUChainReadLocker required.
	 **/
	static Declaration *declarationForNameInBase(const QString& name,
		DUChainPointer<const DUContext> context);
	
	/**
	 * \brief Find all matching declarations.
	 * \note DUChainReadLocker required.
	 **/
	static QVector<Declaration*> declarationsForName(const QString& name,
		const CursorInRevision& location, DUChainPointer<const DUContext> context);
	
	/**
	 * \brief Find all matching declarations.
	 * \note DUChainReadLocker required.
	 **/
	static QVector<Declaration*> declarationsForName(const QString& name,
		const CursorInRevision& location, DUChainPointer<const DUContext> context,
		const QVector<DUChainPointer<const DUContext>> &pinned);
	
	/**
	 * \brief Find all matching declarations in base classes only.
	 * \note DUChainReadLocker required.
	 **/
	static QVector<Declaration*> declarationsForNameInBase(const QString& name,
		DUChainPointer<const DUContext> context);
	
	/**
	 * \brief Find all constructor declarations in class.
	 * \note DUChainReadLocker required.
	 **/
	static QVector<Declaration*> constructorsInClass(DUChainPointer<const DUContext> context);
	
	/**
	 * \brief Find best matching function.
	 * 
	 * Best matching means all parameters match the signature without casting.
	 * 
	 * \note DUChainReadLocker required.
	 * 
	 * \param[in] top Top context required for matching types.
	 * \param[in] signature Signature to match.
	 * \param[in] declarations Declarations to match against.
	 */
	static ClassFunctionDeclaration *bestMatchingFunction(const TopDUContext *top,
		const QVector<KDevelop::AbstractType::Ptr> &signature,
		const QVector<Declaration*> &declarations);
	
	/**
	 * \brief Find suitable functions requiring auto-casting.
	 * 
	 * Functions are suitable if one or more of the signature arguments have to be
	 * auto-castable to the function arguments.
	 * 
	 * \note DUChainReadLocker required.
	 * 
	 * \param[in] top Top context required for matching types.
	 * \param[in] signature Signature to match.
	 * \param[in] declarations Declarations to match against.
	 */
	static QVector<ClassFunctionDeclaration*> autoCastableFunctions(const TopDUContext *top,
		const QVector<KDevelop::AbstractType::Ptr> &signature,
		const QVector<Declaration*> &declarations);
};

}

#endif
