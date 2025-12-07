#include <QList>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QPair>
#include <QMetaType>
#include <QDirIterator>

#include <QDebug>

#include <language/duchain/aliasdeclaration.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchainutils.h>
#include <language/duchain/types/containertypes.h>
#include <language/duchain/types/integraltype.h>
#include <language/duchain/types/typeutils.h>
#include <language/duchain/types/unsuretype.h>
#include <language/backgroundparser/backgroundparser.h>
#include <language/duchain/types/functiontype.h>
#include <interfaces/iproject.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/ipartcontroller.h>
#include <util/path.h>

#include <shell/partcontroller.h>

#include <KTextEditor/View>
#include <KConfigGroup>

#include "Helpers.h"
#include "ExpressionVisitor.h"

using KDevelop::AliasDeclaration;
using KDevelop::BackgroundParser;
using KDevelop::ClassDeclaration;
using KDevelop::DUChain;
using KDevelop::DUChainReadLocker;
using KDevelop::ICore;
using KDevelop::IntegralType;
using KDevelop::ParseJob;
using KDevelop::StructureType;
using KDevelop::FunctionType;

namespace DragonScript {

QMap<QString, QStringList> Helpers::cachedIncludeFiles;
QStringList Helpers::dataDirs;
QVector<IndexedString> Helpers::documentationFiles;
IndexedString Helpers::documentationFileObject;
QVector<ReferencedTopDUContext> Helpers::documentationFileContexts;
bool Helpers::documentationFileContextsReady = false;
bool Helpers::documentationFileContextsParsing = false;
QMutex Helpers::cacheMutex;

KDevelop::AbstractType::Ptr Helpers::pTypeVoid;
KDevelop::AbstractType::Ptr Helpers::pTypeNull;
KDevelop::AbstractType::Ptr Helpers::pTypeInvalid;

KDevelop::AbstractType::Ptr Helpers::pTypeByte;
KDevelop::AbstractType::Ptr Helpers::pTypeBool;
KDevelop::AbstractType::Ptr Helpers::pTypeInt;
KDevelop::AbstractType::Ptr Helpers::pTypeFloat;
KDevelop::AbstractType::Ptr Helpers::pTypeString;
KDevelop::AbstractType::Ptr Helpers::pTypeObject;
KDevelop::AbstractType::Ptr Helpers::pTypeBlock;

DeclarationPointer Helpers::pTypeDeclByte;
DeclarationPointer Helpers::pTypeDeclBool;
DeclarationPointer Helpers::pTypeDeclInt;
DeclarationPointer Helpers::pTypeDeclFloat;
DeclarationPointer Helpers::pTypeDeclString;
DeclarationPointer Helpers::pTypeDeclObject;
DeclarationPointer Helpers::pTypeDeclBlock;

static const Identifier nameConstructor("new");



KDevelop::AbstractType::Ptr Helpers::getTypeVoid(){
	if(! pTypeVoid){
		pTypeVoid = new IntegralType(IntegralType::TypeVoid);
	}
	return pTypeVoid;
}

KDevelop::AbstractType::Ptr Helpers::getTypeNull(){
	if(! pTypeNull){
		pTypeNull = new IntegralType(IntegralType::TypeNull);
	}
	return pTypeNull;
}

KDevelop::AbstractType::Ptr Helpers::getTypeInvalid(){
	if(! pTypeInvalid){
		pTypeInvalid = new IntegralType(IntegralType::TypeMixed);
	}
	return pTypeInvalid;
}

KDevelop::AbstractType::Ptr Helpers::getTypeByte(){
	if(! pTypeDeclByte){
		pTypeDeclByte = getInternalTypeDeclaration("byte");
		if(pTypeDeclByte){
			pTypeByte = pTypeDeclByte->abstractType();
		}
	}
	
	if(! pTypeByte){
		pTypeByte = new IntegralType(IntegralType::TypeByte);
	}
	
	return pTypeByte;
}

KDevelop::AbstractType::Ptr Helpers::getTypeBool(){
	if(! pTypeDeclBool){
		pTypeDeclBool = getInternalTypeDeclaration("bool");
		if(pTypeDeclBool){
			pTypeBool = pTypeDeclBool->abstractType();
		}
	}
	
	if(! pTypeBool){
		pTypeBool = new IntegralType(IntegralType::TypeBoolean);
	}
	
	return pTypeBool;
}

KDevelop::AbstractType::Ptr Helpers::getTypeInt(){
	if(! pTypeDeclInt){
		pTypeDeclInt = getInternalTypeDeclaration("int");
		if(pTypeDeclInt){
			pTypeInt = pTypeDeclInt->abstractType();
		}
	}
	
	if(! pTypeInt){
		pTypeInt = new IntegralType(IntegralType::TypeInt);
	}
	
	return pTypeInt;
}

KDevelop::AbstractType::Ptr Helpers::getTypeFloat(){
	if(! pTypeDeclFloat){
		pTypeDeclFloat = getInternalTypeDeclaration("float");
		if(pTypeDeclFloat){
			pTypeFloat = pTypeDeclFloat->abstractType();
		}
	}
	
	if(! pTypeFloat){
		pTypeFloat = new IntegralType(IntegralType::TypeFloat);
	}
	
	return pTypeFloat;
}

KDevelop::AbstractType::Ptr Helpers::getTypeString(){
	if(! pTypeDeclString){
		pTypeDeclString = getInternalTypeDeclaration("String");
		if(pTypeDeclString){
			pTypeString = pTypeDeclString->abstractType();
		}
	}
	
	if(! pTypeString){
		pTypeString = new IntegralType(IntegralType::TypeString);
	}
	
	return pTypeString;
}

KDevelop::AbstractType::Ptr Helpers::getTypeObject(){
	if(! pTypeDeclObject){
		pTypeDeclObject = getInternalTypeDeclaration("Object");
		if(pTypeDeclObject){
			pTypeObject = pTypeDeclObject->abstractType();
		}
	}
	
	if(! pTypeObject){
		pTypeObject = new IntegralType(IntegralType::TypeMixed);
	}
	
	return pTypeObject;
}

KDevelop::AbstractType::Ptr Helpers::getTypeBlock(){
	if(! pTypeDeclBlock){
		pTypeDeclBlock = getInternalTypeDeclaration("Block");
		if(pTypeDeclBlock){
			pTypeBlock = pTypeDeclBlock->abstractType();
		}
	}
	
	if(! pTypeBlock){
		pTypeBlock = new IntegralType(IntegralType::TypeMixed);
	}
	
	return pTypeBlock;
}

void Helpers::getTypeByte(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeByte();
	declaration = pTypeDeclByte;
}

void Helpers::getTypeBool(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeBool();
	declaration = pTypeDeclBool;
}

void Helpers::getTypeInt(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeInt();
	declaration = pTypeDeclInt;
}

void Helpers::getTypeFloat(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeFloat();
	declaration = pTypeDeclFloat;
}

void Helpers::getTypeString(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeString();
	declaration = pTypeDeclString;
}

void Helpers::getTypeObject(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeObject();
	declaration = pTypeDeclObject;
}

void Helpers::getTypeBlock(DeclarationPointer &declaration, KDevelop::AbstractType::Ptr &type){
	type = getTypeBlock();
	declaration = pTypeDeclBlock;
}



DUChainPointer<const DUContext> Helpers::contextForType(const TopDUContext *top,
const KDevelop::AbstractType::Ptr &type){
	const ClassDeclaration * const classDecl = classDeclFor(top, type);
	if(classDecl){
		return DUChainPointer<const DUContext>(classDecl->internalContext());
		
	}else{
		return DUChainPointer<const DUContext>();
	}
}

ClassDeclaration *Helpers::thisClassDeclFor(DUChainPointer<const DUContext> context){
	DUChainPointer<const DUContext> findContext(context);
	while(findContext){
		ClassDeclaration * const decl = classDeclFor(findContext);
		if(decl){
			return decl;
		}
		findContext = findContext->parentContext();
	}
	
	return nullptr;
}

ClassDeclaration *Helpers::superClassDeclFor(DUChainPointer<const DUContext> context){
	ClassDeclaration * const decl = thisClassDeclFor(context);
	if(! decl){
		return nullptr;
	}
	
	context = context->parentContext();
	if(! context){
		return nullptr;
	}
	
	return superClassOf(context->topContext(), static_cast<ClassDeclaration*>(decl));
}

ClassDeclaration *Helpers::classDeclFor(DUChainPointer<const DUContext> context){
	if(! context){
		return nullptr;
	}
	
	Declaration * const declaration = context->owner();
	if(! declaration){
		return nullptr;
	}
	
	return dynamic_cast<ClassDeclaration*>(declaration);
}

ClassDeclaration *Helpers::classDeclFor(const TopDUContext *top, const KDevelop::AbstractType::Ptr &type){
	if(! type){
		return nullptr;
	}
	
	StructureType::Ptr structType = type.cast<StructureType>();
	if(! structType){
		return nullptr;
	}
	
	Declaration * const declType = structType->declaration(top);
	if(! declType){
		return nullptr;
	}
	
	return static_cast<ClassDeclaration*>(declType);
}

ClassDeclaration *Helpers::superClassOf(const TopDUContext *top, const ClassDeclaration *declaration){
	if(! declaration){
		return nullptr;
	}
	
	if(declaration->baseClassesSize() == 0){
		return nullptr;
	}
	
	return classDeclFor(top, declaration->baseClasses()[0].baseClass.abstractType());
}

QVector<ClassDeclaration*> Helpers::implementsOf(const TopDUContext *top, const ClassDeclaration *declaration){
	QVector<ClassDeclaration*> implements;
	if(! declaration){
		return implements;
	}
	
	uint i;
	for(i=1; i<declaration->baseClassesSize(); i++){
		ClassDeclaration * const implementDecl = classDeclFor(top,
			declaration->baseClasses()[i].baseClass.abstractType());
		if(implementDecl){
			implements.append(implementDecl);
		}
	}
	
	return implements;
}

QVector<ClassDeclaration*> Helpers::baseClassesOf(const TopDUContext *top, const ClassDeclaration *declaration){
	QVector<ClassDeclaration*> baseClasses;
	if(! declaration){
		return baseClasses;
	}
	
	uint i;
	for(i=0; i<declaration->baseClassesSize(); i++){
		ClassDeclaration * const baseClassDecl = classDeclFor(top,
			declaration->baseClasses()[i].baseClass.abstractType());
		if(baseClassDecl){
			baseClasses.append(baseClassDecl);
		}
	}
	
	return baseClasses;
}

bool Helpers::equals(const TopDUContext *top, const KDevelop::AbstractType::Ptr &type1,
const KDevelop::AbstractType::Ptr &type2){
	Q_UNUSED(top);
	return type1 && type2 && type1->equals(type2.operator->());
}

bool Helpers::castable(const TopDUContext *top, const KDevelop::AbstractType::Ptr &type,
const KDevelop::AbstractType::Ptr &targetType){
	if(equals(top, type, targetType)){
		return true;
	}
	
	// if type is null-type cast is always possible
	if(equals(top, type, getTypeNull())){
		return true;
	}
	
	// test against supper class and interfaces of type1
	ClassDeclaration * const classDecl = classDeclFor(top, type);
	if(! classDecl){
		return false;
	}
	
	ClassDeclaration * const superClassDecl = superClassOf(top, classDecl);
	if(superClassDecl){
		if(castable(top, superClassDecl->abstractType(), targetType)){
			return true;
		}
	}
	
	const QVector<ClassDeclaration*> implements(implementsOf(top, classDecl));
	foreach(ClassDeclaration* implement, implements){
		if(castable(top, implement->abstractType(), targetType)){
			return true;
		}
	}
	
	return false;
}

bool Helpers::sameSignature(const TopDUContext *top, const KDevelop::FunctionType::Ptr &func1,
const KDevelop::FunctionType::Ptr &func2){
	return sameSignatureAnyReturnType(top, func1, func2)
		&& equals(top, func1->returnType(), func2->returnType());
}

bool Helpers::sameSignatureAnyReturnType(const TopDUContext *top,
const KDevelop::FunctionType::Ptr &func1, const KDevelop::FunctionType::Ptr &func2){
	if(! func1 || ! func2){
		return false;
	}
	
	if(func1->arguments().size() != func2->arguments().size()){
		return false;
	}
	
	int i;
	for(i=0; i<func1->arguments().size(); i++){
		if(! equals(top, func1->arguments().at(i), func2->arguments().at(i))){
			return false;
		}
	}
	
	return true;
}

bool Helpers::overrides(const TopDUContext *top, const ClassFunctionDeclaration *func1,
const ClassFunctionDeclaration *func2){
	return func1 && func1->context() && func1->context()->owner()
		&& func2 && func2->context() && func2->context()->owner()
		&& castable(top, func1->context()->owner()->abstractType(),
			func2->context()->owner()->abstractType());
}



IndexedDeclaration Helpers::declarationUnderCursor(bool allowUse){
	KDevelop::IDocument * const doc = ICore::self()->documentController()->activeDocument();
	const auto view = static_cast<KDevelop::PartController*>(ICore::self()->partController())->activeView();
	if(doc && doc->textDocument() && view){
		DUChainReadLocker lock;
		const auto cursor = view->cursorPosition();
		if(allowUse){
			return IndexedDeclaration(KDevelop::DUChainUtils::itemUnderCursor(doc->url(), cursor).declaration);
			
		}else{
			return KDevelop::DUChainUtils::declarationInLine(cursor, KDevelop::DUChainUtils::standardContextForUrl(doc->url()));
		}
	}

	return KDevelop::IndexedDeclaration();
}

KDevelop::AbstractType::Ptr Helpers::resolveAliasType(const KDevelop::AbstractType::Ptr eventualAlias){
	return TypeUtils::resolveAliasType(eventualAlias);
}

static const QVector<DUChainPointer<const DUContext>> vEmptyPinned;



Declaration *Helpers::declarationForName(const QString& name,
const CursorInRevision &location, KDevelop::DUChainPointer<const DUContext> context){
	return declarationForName(name, location, context, vEmptyPinned);
}

Declaration *Helpers::declarationForName(const QString& name,
const CursorInRevision &location, KDevelop::DUChainPointer<const DUContext> context,
const QVector<DUChainPointer<const DUContext>> &pinned){
	const Identifier identifier(name);
	QList<Declaration*> declarations;
	
	// find declaration in local context
	declarations = context->findLocalDeclarations(identifier, location /*,
		nullptr, KDevelop::AbstractType::Ptr(), DUContext::DontResolveAliases */);
	if(! declarations.isEmpty()){
		return declarations.last();
	}
	
	// find declaration in this context, base class and up the parent chain
	declarations = context->findDeclarations(identifier);
	if(! declarations.isEmpty()){
		return declarations.last();
	}
	
	Declaration * const foundDeclaration = declarationForNameInBase(name, context);
	if(foundDeclaration){
		return foundDeclaration;
	}
	
	// find declaration in pinned contexts
	for(const DUChainPointer<const DUContext> &pinnedContext : pinned){
		declarations = pinnedContext->findDeclarations(identifier);
		if(! declarations.isEmpty()){
			return declarations.last();
		}
	}
	
	// find declaration in basic language documentation
	const QVector<ReferencedTopDUContext> basicContexts(getDocumentationFileContexts());
	foreach(const ReferencedTopDUContext &basicContext, basicContexts){
		declarations = basicContext->findDeclarations(identifier);
		if(! declarations.isEmpty()){
			return declarations.last();
		}
	}
	
	return nullptr;
}

Declaration *Helpers::declarationForNameInBase(const QString& name,
KDevelop::DUChainPointer<const DUContext> context){
	const ClassDeclaration * const classDecl = dynamic_cast<ClassDeclaration*>(context->owner());
	if(! classDecl || classDecl->baseClassesSize() == 0){
		return nullptr;
	}
	
	TopDUContext * const topContext = context->topContext();
	const Identifier identifier(name);
	uint i;
	
	for(i=0; i<classDecl->baseClassesSize(); i++){
		const StructureType::Ptr structType = classDecl->baseClasses()[i]
			.baseClass.abstractType().cast<StructureType>();
		if(! structType){
			continue;
		}
		
		DUContext * const baseContext = structType->internalContext(topContext);
		if(! baseContext){
			continue;
		}
		
		const QList<Declaration*> declarations(baseContext->findDeclarations(identifier));
		if(! declarations.isEmpty()){
			return declarations.last();
		}
		
		Declaration * const foundDeclaration = declarationForNameInBase(name,
			DUChainPointer<const DUContext>(baseContext));
		if(foundDeclaration){
			return foundDeclaration;
		}
	}
	
	return nullptr;
}



QVector<Declaration*> Helpers::declarationsForName(const QString& name,
const CursorInRevision &location, KDevelop::DUChainPointer<const DUContext> context){
	return declarationsForName(name, location, context, vEmptyPinned);
}

QVector<Declaration*> Helpers::declarationsForName(const QString& name,
const CursorInRevision &location, KDevelop::DUChainPointer<const DUContext> context,
const QVector<DUChainPointer<const DUContext>> &pinned){
	QVector<Declaration*> foundDeclarations;
	const Identifier identifier(name);
	QList<Declaration*> declarations;
	
	// find declaration in local context
	declarations = context->findLocalDeclarations(identifier, location /*,
		nullptr, KDevelop::AbstractType::Ptr(), DUContext::DontResolveAliases */);
	foreach(Declaration *declaration, declarations){
		foundDeclarations.append(declaration);
	}
	
	// find declaration in this context, base class and up the parent chain
	declarations = context->findDeclarations(identifier);
	foreach(Declaration *declaration, declarations){
		if(! foundDeclarations.contains(declaration)){
			foundDeclarations.append(declaration);
		}
	}
	
	foundDeclarations.append(declarationsForNameInBase(name, context));
	
	// find declaration in pinned contexts
	for(const DUChainPointer<const DUContext> &pinnedContext : pinned){
		declarations = pinnedContext->findDeclarations(identifier);
		foreach(Declaration* declaration, declarations){
			foundDeclarations.append(declaration);
		}
	}
	
	// find declaration in imported contexts
	// TODO we need to find this somehow.
	//QVector<ReferencedTopDUContext> importedContexts( getDocumentationFileContexts() );
	
	// find declaration in basic language documentation
	QVector<ReferencedTopDUContext> basicContexts(getDocumentationFileContexts());
	foreach(const ReferencedTopDUContext &basicContext, basicContexts){
		declarations = basicContext->findDeclarations(identifier);
		foreach(Declaration *declaration, declarations){
			foundDeclarations.append(declaration);
		}
	}
	
	return foundDeclarations;
}

QVector<Declaration*> Helpers::declarationsForNameInBase(const QString& name,
KDevelop::DUChainPointer<const DUContext> context){
	QVector<Declaration*> foundDeclarations;
	
	const ClassDeclaration * const classDecl = dynamic_cast<ClassDeclaration*>(context->owner());
	if(! classDecl || classDecl->baseClassesSize() == 0){
		return foundDeclarations;
	}
	
	TopDUContext * const topContext = context->topContext();
	const Identifier identifier(name);
	uint i;
	
	for(i=0; i<classDecl->baseClassesSize(); i++){
		const StructureType::Ptr structType = classDecl->baseClasses()[i]
			.baseClass.abstractType().cast<StructureType>();
		if(! structType){
			continue;
		}
		
		DUContext * const baseContext = structType->internalContext(topContext);
		if(! baseContext){
			continue;
		}
		
		const QList<Declaration*> declarations(baseContext->findDeclarations(identifier));
		foreach(Declaration *declaration, declarations){
			if(! foundDeclarations.contains(declaration)){
				foundDeclarations.append(declaration);
			}
		}
		
		foundDeclarations.append(declarationsForNameInBase(name,
			DUChainPointer<const DUContext>(baseContext)));
	}
	
	return foundDeclarations;
}

QVector<Declaration*> Helpers::constructorsInClass(DUChainPointer<const DUContext> context){
	QVector<Declaration*> foundDeclarations;
	QList<Declaration*> declarations;
	
	// find declaration in this context without base class or parent chain
	declarations = context->findDeclarations(nameConstructor);
	foreach(Declaration *declaration, declarations){
		if(! foundDeclarations.contains(declaration)){
			foundDeclarations.append(declaration);
		}
	}
	
	return foundDeclarations;
}



ClassFunctionDeclaration *Helpers::bestMatchingFunction(const TopDUContext *top,
const QVector<KDevelop::AbstractType::Ptr> &signature, const QVector<Declaration*> &declarations){
	ClassFunctionDeclaration *bestMatchingDecl = nullptr;
	const int argCount = signature.size();
	int i;
	
	foreach(Declaration* declaration, declarations){
		ClassFunctionDeclaration * const funcDecl = dynamic_cast<ClassFunctionDeclaration*>(declaration);
		if(! funcDecl){
			continue;
		}
		
		const FunctionType::Ptr funcType = funcDecl->type<FunctionType>();
		if(funcType->arguments().size() != argCount){
			continue;
		}
		
		for(i=0; i<argCount; i++){
			if(! equals(top, signature.at(i), funcType->arguments().at(i))){
				break;
			}
		}
		if(i == argCount){
			bestMatchingDecl = funcDecl;
			break;
		}
	}
	
	return bestMatchingDecl;
}

QVector<ClassFunctionDeclaration*> Helpers::autoCastableFunctions(const TopDUContext *top,
const QVector<KDevelop::AbstractType::Ptr> &signature, const QVector<Declaration*> &declarations){
	QVector<ClassFunctionDeclaration*> possibleFunctions;
	const int argCount = signature.size();
	int i;
	
	foreach(Declaration *declaration, declarations){
		ClassFunctionDeclaration * const funcDecl = dynamic_cast<ClassFunctionDeclaration*>(declaration);
		if(! funcDecl){
			continue;
		}
		
		const FunctionType::Ptr funcType = funcDecl->type<FunctionType>();
		if(funcType->arguments().size() != argCount){
			continue;
		}
		
		for(i=0; i<argCount; i++){
			if(! castable(top, signature.at(i), funcType->arguments().at(i))){
				break;
			}
		}
		if(i < argCount){
			continue;
		}
		
		// this function could be identical in signature to another one found so far.
		// if this is the case and the already found function is a reimplmentation
		// of this function then ignore this function
		bool ignoreFunction = false;
		foreach(const ClassFunctionDeclaration *checkFuncDecl, possibleFunctions){
			// if this is a constructor call do not check the return type
			if(nameConstructor == declaration->identifier()){
				if(sameSignatureAnyReturnType(top, funcType, checkFuncDecl->type<FunctionType>())
				&& overrides(top, checkFuncDecl, funcDecl)){
					ignoreFunction = true;
					break;
				}
				
			}else{
				if(sameSignature(top, funcType, checkFuncDecl->type<FunctionType>())
				&& overrides(top, checkFuncDecl, funcDecl)){
					ignoreFunction = true;
					break;
				}
			}
		}
		if(ignoreFunction){
			continue;
		}
		
		// function matches and is a suitable pick
		possibleFunctions.append(funcDecl);
	}
	
	return possibleFunctions;
}



void Helpers::dropIncludeFileContexts(const QString &includeDir){
	QMap<QString, QStringList>::iterator iter = cachedIncludeFiles.find(includeDir);
	if(iter != cachedIncludeFiles.end()){
		cachedIncludeFiles.erase(iter);
	}
}

bool Helpers::addImportsIncludePath(TopDUContext *top, const QString &includeDir){
	QStringList list;
	
	// ensure file listing exists
	if(! cachedIncludeFiles.contains(includeDir)){
		qDebug() << "KDevDScript: Helpers::getIncludeFileContexts: searching include directory" << includeDir;
		QDirIterator iter(includeDir, QStringList() << "*.ds", QDir::Files, QDirIterator::Subdirectories);
		QMutexLocker lock(&cacheMutex);
		while(iter.hasNext()){
			list.append(iter.next());
		}
		
		cachedIncludeFiles.insert(includeDir, list).value();
		
		foreach(const QString &filename, list){
			//qDebug() << "KDevDScript: Helpers::getIncludeFileContexts: parsing" << filename;
			ICore::self()->languageController()->backgroundParser()->addDocument(
				IndexedString(filename), KDevelop::TopDUContext::ForceUpdate,
				BackgroundParser::BestPriority, 0, ParseJob::FullSequentialProcessing);
		}
	}
	
	// TODO make this correct later on
	const QVector<ReferencedTopDUContext> contexts(getImportsIncludeContexts(includeDir));
	if(! contexts.isEmpty()){
		QList<QPair<TopDUContext*, CursorInRevision>> imports;
		foreach(const ReferencedTopDUContext &each, contexts){
			imports.append(qMakePair(each, CursorInRevision(1, 0)));
		}
		top->addImportedParentContexts(imports);
	}
	
	return true;
}

QVector<ReferencedTopDUContext> Helpers::getImportsIncludeContexts(const QString &includeDir){
	QVector<ReferencedTopDUContext> contexts;
	
	if(! cachedIncludeFiles.contains(includeDir)){
		return contexts;
	}
	
	const QStringList &files = cachedIncludeFiles[includeDir];
	foreach(const QString &file, files){
		const IndexedString isfile(file);
		TopDUContext * const context = DUChain::self()->chainForDocument(isfile);
		if(context){
			contexts.append(ReferencedTopDUContext(context));
		}
	}
	
	return contexts;
}



QVector<DUContext*> Helpers::internalContextsForClass(const KDevelop::StructureType::Ptr classType,
const TopDUContext *context, ContextSearchFlags flags, int depth){
	QVector<DUContext*> searchContexts;
	if(! classType){
		return searchContexts;
	}
	if(auto c = classType->internalContext(context)){
		searchContexts << c;
	}
	auto decl = Helpers::resolveAliasDeclaration(classType->declaration(context));
	if(auto classDecl = dynamic_cast<ClassDeclaration*>(decl)){
		FOREACH_FUNCTION (const auto &base, classDecl->baseClasses){
			if(flags == PublicOnly && base.access == KDevelop::Declaration::Private){
				continue;
			}
			auto baseClassType = base.baseClass.type<StructureType>();
			// recursive call, because the base class will have more base classes eventually
			if(depth < 10){
				searchContexts.append(Helpers::internalContextsForClass(baseClassType, context, flags, depth + 1));
			}
		}
	}
	return searchContexts;
}

Declaration *Helpers::resolveAliasDeclaration(Declaration *decl){
	AliasDeclaration * const alias = dynamic_cast<AliasDeclaration*>(decl);
	if(alias){
		DUChainReadLocker lock;
		return alias->aliasedDeclaration().data();
		
	}else{
		return decl;
	}
}

QStringList Helpers::getDataDirs() {
	if(dataDirs.isEmpty()){
		dataDirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
			"kdevdragonscriptsupport/dslangdoc", QStandardPaths::LocateDirectory);
	}
	return dataDirs;
}

const QVector<IndexedString> &Helpers::getDocumentationFiles(){
	if(documentationFiles.isEmpty()){
		QStringList files;
		files.append("bool.ds");
		files.append("int.ds");
		files.append("byte.ds");
		files.append("float.ds");
		files.append("Object.ds");
		files.append("Exception.ds");
		files.append("String.ds");
		files.append("Array.ds");
		files.append("Dictionary.ds");
		files.append("Block.ds");
		files.append("Byte.ds");
		files.append("Boolean.ds");
		files.append("Integer.ds");
		files.append("Float.ds");
		files.append("WeakReference.ds");
		files.append("math/Math.ds");
		files.append("exceptions/EDivisionByZero.ds");
		files.append("exceptions/EInvalidCast.ds");
		files.append("exceptions/EInvalidParam.ds");
		files.append("exceptions/ENullPointer.ds");
		files.append("exceptions/EOutOfBounds.ds");
		files.append("exceptions/EOutOfMemory.ds");
		files.append("introspection/DS.ds");
		files.append("introspection/Package.ds");
		files.append("introspection/Class.ds");
		files.append("introspection/Variable.ds");
		files.append("introspection/Function.ds");
		
		foreach(const QString &file, files){
			auto path = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
				QString("kdevdragonscriptsupport/dslangdoc/") + file);
			Helpers::documentationFiles.append(IndexedString(path));
			//qDebug() << "KDevDScript: Helpers::getDocumentationFiles add" << path;
		}
	}
	
	return documentationFiles;
}

IndexedString Helpers::getDocumentationFileObject(){
	if(documentationFileObject.isEmpty()){
		documentationFileObject = IndexedString(
			QStandardPaths::locate(QStandardPaths::GenericDataLocation,
				QString("kdevdragonscriptsupport/dslangdoc/Object.ds")));
	}
	return documentationFileObject;
}

QVector<ReferencedTopDUContext> Helpers::getDocumentationFileContexts(){
// 	qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: ready" << documentationFileContextsReady;
	if(! documentationFileContextsReady){
		const QVector<IndexedString> &files = getDocumentationFiles();
		documentationFileContexts.clear();
		
		if(documentationFileContextsParsing){
			foreach(const IndexedString &file, files){
				TopDUContext * const context = DUChain::self()->chainForDocument(file);
				if(context){
// 					qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: File is ready:" << file;
					documentationFileContexts.append(ReferencedTopDUContext(context));
					
				}else{
// 					qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: Parsing file not finished yet:" << file;
					documentationFileContexts.clear();
					break;
				}
			}
			
		}else{
			bool allReady = true;
			documentationFileContextsParsing = true;
			
			foreach(const IndexedString &file, files){
				TopDUContext * const context = DUChain::self()->chainForDocument(file);
				if(context){
// 					qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: File has context:" << file;
					documentationFileContexts.append(ReferencedTopDUContext(context));
					
				}else{
					qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: File has no context, parsing it:" << file;
					ICore::self()->languageController()->backgroundParser()->addDocument(file,
						KDevelop::TopDUContext::ForceUpdate, BackgroundParser::BestPriority,
						0, ParseJob::FullSequentialProcessing);
					allReady = false;
				}
			}
			
			if(! allReady){
				documentationFileContexts.clear();
			}
		}
		
		if(! documentationFileContexts.isEmpty()){
			qDebug() << "KDevDScript: Helpers::getDocumentationFileContexts: All files read";
			
			if(documentationFileContextsParsing){
				// re-parse all language files. this is required since language files depend
				// on the definition of each other and the parsing order can be messed up.
				// this reparsing has no effect on the context but will trigger reparsing
				// of source files once the individual files are updated
				foreach(const IndexedString &file, files){
					ICore::self()->languageController()->backgroundParser()->addDocument(file,
						KDevelop::TopDUContext::ForceUpdate, BackgroundParser::BestPriority,
						0, ParseJob::FullSequentialProcessing);
				}
			}
			
			documentationFileContextsParsing = false;
			documentationFileContextsReady = true;
		}
	}
	
	QVector<ReferencedTopDUContext> contexts;
	foreach(const ReferencedTopDUContext &context, documentationFileContexts){
		contexts.append(context);
	}
	return contexts;
}

bool Helpers::addImportsDocumentationFileContexts(TopDUContext *top){
	const QVector<ReferencedTopDUContext> contexts(Helpers::getDocumentationFileContexts());
	if(contexts.isEmpty()){
		return false;
	}
	
	QList<QPair<TopDUContext*, CursorInRevision>> imports;
	
	foreach(const ReferencedTopDUContext &each, contexts){
		imports.append(qMakePair(each, CursorInRevision(1, 0)));
	}
	top->addImportedParentContexts(imports);
	
	return true;
}

Declaration *Helpers::getInternalTypeDeclaration(const QString &name){
	QVector<ReferencedTopDUContext> contexts(getDocumentationFileContexts());
	const Identifier identifier(name);
	
	foreach(const ReferencedTopDUContext &context, contexts){
		const QList<Declaration*> declarations(context->findDeclarations(identifier));
		if(! declarations.isEmpty()){
			return declarations.last();
		}
	}
	
	return nullptr;
}

}
