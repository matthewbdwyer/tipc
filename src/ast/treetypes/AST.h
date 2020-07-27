#pragma once

/*
 * This include is for convenience when defining algorithms spanning
 * the AST type hierarchy, e.g., visitors, code generation, pretty printing.
 *
 * It should be used sparingly as it introduces coupling to the entire
 * AST type hierarchy.
 */

#include "ASTProgram.h"
#include "ASTFunction.h"
#include "ASTNode.h"
#include "ASTExpr.h"
#include "ASTAccessExpr.h"
#include "ASTAllocExpr.h"
#include "ASTBinaryExpr.h"
#include "ASTDeRefExpr.h"
#include "ASTFieldExpr.h"
#include "ASTFunAppExpr.h"
#include "ASTInputExpr.h"
#include "ASTNullExpr.h"
#include "ASTNumberExpr.h"
#include "ASTRecordExpr.h"
#include "ASTRefExpr.h"
#include "ASTVariableExpr.h"
#include "ASTDeclNode.h"
#include "ASTStmt.h"
#include "ASTAssignStmt.h"
#include "ASTBlockStmt.h"
#include "ASTDeclStmt.h"
#include "ASTErrorStmt.h"
#include "ASTIfStmt.h"
#include "ASTOutputStmt.h"
#include "ASTReturnStmt.h"
#include "ASTWhileStmt.h"
