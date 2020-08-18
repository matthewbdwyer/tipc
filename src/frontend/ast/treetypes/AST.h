#pragma once

/*
 * This include is for convenience when defining algorithms spanning
 * the AST type hierarchy, e.g., visitors, code generation, pretty printing.
 *
 * It should be used sparingly as it introduces coupling to the entire
 * AST type hierarchy.
 */

#include "ASTAccessExpr.h"
#include "ASTAllocExpr.h"
#include "ASTAssignStmt.h"
#include "ASTBinaryExpr.h"
#include "ASTBlockStmt.h"
#include "ASTDeRefExpr.h"
#include "ASTDeclNode.h"
#include "ASTDeclStmt.h"
#include "ASTErrorStmt.h"
#include "ASTExpr.h"
#include "ASTFieldExpr.h"
#include "ASTFunAppExpr.h"
#include "ASTFunction.h"
#include "ASTIfStmt.h"
#include "ASTInputExpr.h"
#include "ASTNode.h"
#include "ASTNullExpr.h"
#include "ASTNumberExpr.h"
#include "ASTOutputStmt.h"
#include "ASTProgram.h"
#include "ASTRecordExpr.h"
#include "ASTRefExpr.h"
#include "ASTReturnStmt.h"
#include "ASTStmt.h"
#include "ASTVariableExpr.h"
#include "ASTWhileStmt.h"
