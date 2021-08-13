#pragma once

#include "ASTProgram.h"
#include "SemanticAnalysis.h"
#include "llvm/IR/Module.h"

static const char *const LLVM_ASM_EXT = ".ll";
static const char *const LLVM_BC_EXT = ".bc";

/*! \class CodeGenerator
 *  \brief Routines to optimize generated code.
 *
 * A collection of routines for generating LLVM IR from a program AST and its 
 * semantic analysis results and for emitting the resulting LLVM IR to a file.
 */
class CodeGenerator {
public:

  /*! \fn generate
   *  \brief Generate LLVM IR for ast.
   *
   * Apply a series of basic optimization passes to the given LLVM module.
   * \param program the root of an AST encoding the program
   * \param analysisResults the results from semantic analysis of the program
   * \param fileName the name of the source file holding the program
   * \return the LLVM module holding the generated program
   */
  static std::unique_ptr<llvm::Module> generate(ASTProgram* program, SemanticAnalysis* analysisResults, std::string fileName);

  /*! \fn emit
   *  \brief Emit LLVM IR to a file.
   *
   * \param m the LLVM module holding the generated program
   */
  static void emit(llvm::Module* m, std::string filename = "");

  /*! \fn emitHumanReadableAssembly
   *  \brief Emit LLVM human-readable assembly to a file.
   *
   * \param m the LLVM module holding the generated program
   */
  static void emitHumanReadableAssembly(llvm::Module* m, std::string filename = "");
};
