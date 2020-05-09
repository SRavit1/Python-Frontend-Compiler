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

Value *binary_expression::codegen() {
	Value* L = LHS->codegen();
	Value* R = RHS->codegen();
	
	Value* output = NULL;
	switch(Op) {
		case '+':
			output = Builder.CreateAdd(L, R, name);
			break;
        case '-':
			output = Builder.CreateSub(L, R, name);
			break;
        case '*':
			output = Builder.CreateMul(L, R, name);
			break;
        case '/':
            output = Builder.CreateSDiv(L, R, name);
			break;
			//TODO: check what is correct div
		default:
			return output;
	}

	if (name != 0) {
		NamedValues[name] = output;
	}

	return output;
}

Value *variable::codegen() {
	return NamedValues[name];
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
	args_value.push_back(args[i].codegen());
	if (!args_value.back())
		return nullptr;
	}

	return Builder.CreateCall(callee, args_value, name);
}

Value *return_statement::codegen() {
	return Builder.CreateRet(return_val->codegen());
}

Function *function_exp::codegen() {
	std::vector <Type *> args_obj = {};
	
	for (int arg_type : arg_types) {
		Type *arg_type_obj = Type::getVoidTy(TheContext);
		switch(arg_type) {
			case -1: //void
				break;
			case 0: //int
				arg_type_obj = Type::getInt32Ty(TheContext);
				break;
			case 1: //float
				arg_type_obj = Type::getFloatTy(TheContext);
				break;
			case 2: //string
				//cannot find what is appropriate type for string ??
				break;
			case 3: //bool
				arg_type_obj = Type::getInt1Ty(TheContext);
				break;
			default:
				break;
		}
		args_obj.push_back(arg_type_obj);
	}

	Type *ret_type_obj = Type::getVoidTy(TheContext);
	switch(ret_type) {
		case -1: //void
			break;
		case 0: //int
			ret_type_obj = Type::getInt32Ty(TheContext);
			break;
		case 1: //float
			ret_type_obj = Type::getFloatTy(TheContext);
			break;
		case 2: //string
			//cannot find what is appropriate type for string ??
			break;
		case 3: //bool
			ret_type_obj = Type::getInt1Ty(TheContext);
			break;
		default:
			break;
	}
	
	FunctionType *FT = FunctionType::get(ret_type_obj, args_obj, false);
	Function *TheFunction = Function::Create(FT, Function::ExternalLinkage, function_name, TheModule.get());

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

	for (expression statement : body) {
		statement.codegen();
	}

	verifyFunction(std::move(*TheFunction));
	return TheFunction;

	// Error reading body, remove function.
	TheFunction->eraseFromParent();
	return nullptr;
}

void codegen(const std::vector<function_exp> statements) {
	for (auto statement : statements) {
		statement.codegen();
	}

	TheModule->print(errs(), nullptr);
}