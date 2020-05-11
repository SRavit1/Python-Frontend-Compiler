#include "nodescpp.h"

//TODO: Figure out which includes are needed
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <fstream>

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule = make_unique<Module>("my cool jit", TheContext);
static std::map<std::string, Value *> NamedValues;

std::unique_ptr<expression> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

Type* getTypeObj (int type) {
	Type *type_obj = Type::getVoidTy(TheContext);

	if (type == 0)
		type_obj = Type::getInt32Ty(TheContext);
	else if (type == 1)
		type_obj = Type::getFloatTy(TheContext);
	else if (type == 2)
		//cannot find what is appropriate type for string ??
		type_obj = Type::getVoidTy(TheContext);
	else if (type == 3)
		type_obj = Type::getInt1Ty(TheContext);

	return type_obj;
}

Value *binary_expression::codegen() {
	Value* L = LHS->codegen();
	Value* R = RHS->codegen();
	
	Value* output = nullptr;

	if (Op == "+")	
		output = Builder.CreateAdd(L, R, getName());
	else if (Op == "-")
		output = Builder.CreateSub(L, R, getName());
	else if (Op == "*")
		output = Builder.CreateMul(L, R, getName());
	else if (Op == "/")
		output = Builder.CreateSDiv(L, R, getName());
		//TODO: check what is correct div

	if (getName() != "") {
		NamedValues[getName()] = output;
	}

	return output;
}

Value *variable::codegen() {
	return NamedValues[getName()];
}

Value *integer_const::codegen() {
	return ConstantInt::get(TheContext, APInt(32, val));
}

Value *float_const::codegen() {
	return ConstantFP::get(TheContext, APFloat(val));
}

Value *string_const::codegen() {
	return ConstantDataArray::get(TheContext, llvm::arrayRefFromStringRef(StringRef(val)));
}

Value *bool_const::codegen() {
	return ConstantInt::get(TheContext, APInt(1, val));
}

Value *function_call::codegen() {
	// Look up the name in the global module table.
	Function *callee = TheModule->getFunction(function_name);

	if (!callee)
		return LogErrorV("Unknown function referenced");

	// If argument mismatch error.
	if (callee->arg_size() != args.size())
		return LogErrorV("Incorrect # arguments passed");

	std::vector<Value *> args_value;
	for (unsigned i = 0, e = args.size(); i != e; ++i) {
	args_value.push_back(args[i]->codegen());
	if (!args_value.back())
		return nullptr;
	}

	return Builder.CreateCall(callee, args_value, getName());
}

Value *return_statement::codegen() {
	return Builder.CreateRet(return_val->codegen());
}

Function *function_exp::codegen() {
	std::vector <Type *> args_obj;
	
	if (arg_types.size() != arg_names.size())
		std::cerr << "Internal error: arg_types vector size mismatch with arg_names vector";

	for (int arg_type : arg_types) {
		Type *arg_type_obj = getTypeObj(arg_type);
		args_obj.push_back(arg_type_obj);
	}

	Type *ret_type_obj = getTypeObj(ret_type);
	
	FunctionType *FT = FunctionType::get(ret_type_obj, args_obj, false);
	Function *TheFunction = Function::Create(FT, Function::ExternalLinkage, getName(), TheModule.get());

	// Set names for all arguments.
	unsigned Idx = 0;
	for (auto &arg : TheFunction->args())
		arg.setName(arg_names[Idx++]);

	// Create a new basic block to start insertion into.
	BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
	Builder.SetInsertPoint(BB);

	// Record the function arguments in the NamedValues map.
	NamedValues.clear();
	for (auto &arg : TheFunction->args())
		NamedValues[std::string(arg.getName())] = &arg;

	for (expression* stmnt : *body) {
		stmnt->codegen();
	}

	verifyFunction(std::move(*TheFunction));
	return TheFunction;

	// Error reading body, remove function.
	TheFunction->eraseFromParent();
	return nullptr;
}

void codegen(const std::vector<function_exp*> statements) {
	for (auto statement : statements) {
		statement->codegen();
	}

	TheModule->print(errs(), nullptr);
}