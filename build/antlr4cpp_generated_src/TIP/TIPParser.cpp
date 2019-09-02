
// Generated from TIP.g4 by ANTLR 4.7.2


#include "TIPVisitor.h"

#include "TIPParser.h"


using namespace antlrcpp;
using namespace antlr4;

TIPParser::TIPParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

TIPParser::~TIPParser() {
  delete _interpreter;
}

std::string TIPParser::getGrammarFileName() const {
  return "TIP.g4";
}

const std::vector<std::string>& TIPParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& TIPParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

TIPParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TIPParser::FunctionContext *> TIPParser::ProgramContext::function() {
  return getRuleContexts<TIPParser::FunctionContext>();
}

TIPParser::FunctionContext* TIPParser::ProgramContext::function(size_t i) {
  return getRuleContext<TIPParser::FunctionContext>(i);
}


size_t TIPParser::ProgramContext::getRuleIndex() const {
  return TIPParser::RuleProgram;
}


antlrcpp::Any TIPParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::ProgramContext* TIPParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, TIPParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(43);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TIPParser::IDENTIFIER) {
      setState(40);
      function();
      setState(45);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

TIPParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> TIPParser::FunctionContext::IDENTIFIER() {
  return getTokens(TIPParser::IDENTIFIER);
}

tree::TerminalNode* TIPParser::FunctionContext::IDENTIFIER(size_t i) {
  return getToken(TIPParser::IDENTIFIER, i);
}

TIPParser::ReturnStmtContext* TIPParser::FunctionContext::returnStmt() {
  return getRuleContext<TIPParser::ReturnStmtContext>(0);
}

std::vector<TIPParser::DeclarationContext *> TIPParser::FunctionContext::declaration() {
  return getRuleContexts<TIPParser::DeclarationContext>();
}

TIPParser::DeclarationContext* TIPParser::FunctionContext::declaration(size_t i) {
  return getRuleContext<TIPParser::DeclarationContext>(i);
}

std::vector<TIPParser::StatementContext *> TIPParser::FunctionContext::statement() {
  return getRuleContexts<TIPParser::StatementContext>();
}

TIPParser::StatementContext* TIPParser::FunctionContext::statement(size_t i) {
  return getRuleContext<TIPParser::StatementContext>(i);
}


size_t TIPParser::FunctionContext::getRuleIndex() const {
  return TIPParser::RuleFunction;
}


antlrcpp::Any TIPParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::FunctionContext* TIPParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 2, TIPParser::RuleFunction);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(46);
    match(TIPParser::IDENTIFIER);
    setState(47);
    match(TIPParser::T__0);
    setState(56);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TIPParser::IDENTIFIER) {
      setState(48);
      match(TIPParser::IDENTIFIER);
      setState(53);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == TIPParser::T__1) {
        setState(49);
        match(TIPParser::T__1);
        setState(50);
        match(TIPParser::IDENTIFIER);
        setState(55);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(58);
    match(TIPParser::T__2);
    setState(59);
    match(TIPParser::T__3);

    setState(63);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TIPParser::KVAR) {
      setState(60);
      declaration();
      setState(65);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }

    setState(69);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << TIPParser::T__3)
      | (1ULL << TIPParser::MUL)
      | (1ULL << TIPParser::KWHILE)
      | (1ULL << TIPParser::KIF)
      | (1ULL << TIPParser::KOUTPUT)
      | (1ULL << TIPParser::KERROR)
      | (1ULL << TIPParser::IDENTIFIER))) != 0)) {
      setState(66);
      statement();
      setState(71);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(72);
    returnStmt();
    setState(73);
    match(TIPParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

TIPParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t TIPParser::ExprContext::getRuleIndex() const {
  return TIPParser::RuleExpr;
}

void TIPParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NegNumberContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::NegNumberContext::SUB() {
  return getToken(TIPParser::SUB, 0);
}

tree::TerminalNode* TIPParser::NegNumberContext::NUMBER() {
  return getToken(TIPParser::NUMBER, 0);
}

TIPParser::NegNumberContext::NegNumberContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::NegNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitNegNumber(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomExprContext ------------------------------------------------------------------

TIPParser::AtomContext* TIPParser::AtomExprContext::atom() {
  return getRuleContext<TIPParser::AtomContext>(0);
}

TIPParser::AtomExprContext::AtomExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::AtomExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAtomExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AdditiveExprContext ------------------------------------------------------------------

std::vector<TIPParser::ExprContext *> TIPParser::AdditiveExprContext::expr() {
  return getRuleContexts<TIPParser::ExprContext>();
}

TIPParser::ExprContext* TIPParser::AdditiveExprContext::expr(size_t i) {
  return getRuleContext<TIPParser::ExprContext>(i);
}

tree::TerminalNode* TIPParser::AdditiveExprContext::ADD() {
  return getToken(TIPParser::ADD, 0);
}

tree::TerminalNode* TIPParser::AdditiveExprContext::SUB() {
  return getToken(TIPParser::SUB, 0);
}

TIPParser::AdditiveExprContext::AdditiveExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::AdditiveExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationalExprContext ------------------------------------------------------------------

std::vector<TIPParser::ExprContext *> TIPParser::RelationalExprContext::expr() {
  return getRuleContexts<TIPParser::ExprContext>();
}

TIPParser::ExprContext* TIPParser::RelationalExprContext::expr(size_t i) {
  return getRuleContext<TIPParser::ExprContext>(i);
}

tree::TerminalNode* TIPParser::RelationalExprContext::GT() {
  return getToken(TIPParser::GT, 0);
}

TIPParser::RelationalExprContext::RelationalExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::RelationalExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitRelationalExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultiplicativeExprContext ------------------------------------------------------------------

std::vector<TIPParser::ExprContext *> TIPParser::MultiplicativeExprContext::expr() {
  return getRuleContexts<TIPParser::ExprContext>();
}

TIPParser::ExprContext* TIPParser::MultiplicativeExprContext::expr(size_t i) {
  return getRuleContext<TIPParser::ExprContext>(i);
}

tree::TerminalNode* TIPParser::MultiplicativeExprContext::MUL() {
  return getToken(TIPParser::MUL, 0);
}

tree::TerminalNode* TIPParser::MultiplicativeExprContext::DIV() {
  return getToken(TIPParser::DIV, 0);
}

TIPParser::MultiplicativeExprContext::MultiplicativeExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::MultiplicativeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqualityExprContext ------------------------------------------------------------------

std::vector<TIPParser::ExprContext *> TIPParser::EqualityExprContext::expr() {
  return getRuleContexts<TIPParser::ExprContext>();
}

TIPParser::ExprContext* TIPParser::EqualityExprContext::expr(size_t i) {
  return getRuleContext<TIPParser::ExprContext>(i);
}

tree::TerminalNode* TIPParser::EqualityExprContext::EQ() {
  return getToken(TIPParser::EQ, 0);
}

TIPParser::EqualityExprContext::EqualityExprContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::EqualityExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitEqualityExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::ExprContext* TIPParser::expr() {
   return expr(0);
}

TIPParser::ExprContext* TIPParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  TIPParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  TIPParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 4;
  enterRecursionRule(_localctx, 4, TIPParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(79);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TIPParser::SUB: {
        _localctx = _tracker.createInstance<NegNumberContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(76);
        match(TIPParser::SUB);
        setState(77);
        match(TIPParser::NUMBER);
        break;
      }

      case TIPParser::T__0:
      case TIPParser::T__3:
      case TIPParser::T__5:
      case TIPParser::MUL:
      case TIPParser::NUMBER:
      case TIPParser::KALLOC:
      case TIPParser::KINPUT:
      case TIPParser::KNULL:
      case TIPParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<AtomExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(78);
        atom();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(95);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(93);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MultiplicativeExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(81);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(82);
          dynamic_cast<MultiplicativeExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == TIPParser::MUL

          || _la == TIPParser::DIV)) {
            dynamic_cast<MultiplicativeExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(83);
          expr(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AdditiveExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(84);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(85);
          dynamic_cast<AdditiveExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == TIPParser::ADD

          || _la == TIPParser::SUB)) {
            dynamic_cast<AdditiveExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(86);
          expr(5);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<RelationalExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(87);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(88);
          dynamic_cast<RelationalExprContext *>(_localctx)->op = match(TIPParser::GT);
          setState(89);
          expr(4);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<EqualityExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(90);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(91);
          dynamic_cast<EqualityExprContext *>(_localctx)->op = match(TIPParser::EQ);
          setState(92);
          expr(3);
          break;
        }

        } 
      }
      setState(97);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

TIPParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t TIPParser::AtomContext::getRuleIndex() const {
  return TIPParser::RuleAtom;
}

void TIPParser::AtomContext::copyFrom(AtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ParenAtomContext ------------------------------------------------------------------

TIPParser::ParenExprContext* TIPParser::ParenAtomContext::parenExpr() {
  return getRuleContext<TIPParser::ParenExprContext>(0);
}

TIPParser::ParenAtomContext::ParenAtomContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::ParenAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitParenAtom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeRefAtomContext ------------------------------------------------------------------

TIPParser::DeRefExprContext* TIPParser::DeRefAtomContext::deRefExpr() {
  return getRuleContext<TIPParser::DeRefExprContext>(0);
}

TIPParser::DeRefAtomContext::DeRefAtomContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::DeRefAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitDeRefAtom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NullExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::NullExprContext::KNULL() {
  return getToken(TIPParser::KNULL, 0);
}

TIPParser::NullExprContext::NullExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::NullExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitNullExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AllocExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::AllocExprContext::KALLOC() {
  return getToken(TIPParser::KALLOC, 0);
}

TIPParser::ExprContext* TIPParser::AllocExprContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}

TIPParser::AllocExprContext::AllocExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::AllocExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAllocExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RefExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::RefExprContext::IDENTIFIER() {
  return getToken(TIPParser::IDENTIFIER, 0);
}

TIPParser::RefExprContext::RefExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::RefExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitRefExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RecordAtomContext ------------------------------------------------------------------

TIPParser::RecordExprContext* TIPParser::RecordAtomContext::recordExpr() {
  return getRuleContext<TIPParser::RecordExprContext>(0);
}

TIPParser::AccessExprContext* TIPParser::RecordAtomContext::accessExpr() {
  return getRuleContext<TIPParser::AccessExprContext>(0);
}

TIPParser::RecordAtomContext::RecordAtomContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::RecordAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitRecordAtom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InputExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::InputExprContext::KINPUT() {
  return getToken(TIPParser::KINPUT, 0);
}

TIPParser::InputExprContext::InputExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::InputExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitInputExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FunAppAtomContext ------------------------------------------------------------------

TIPParser::FunAppExprContext* TIPParser::FunAppAtomContext::funAppExpr() {
  return getRuleContext<TIPParser::FunAppExprContext>(0);
}

TIPParser::FunAppAtomContext::FunAppAtomContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::FunAppAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitFunAppAtom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NumExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::NumExprContext::NUMBER() {
  return getToken(TIPParser::NUMBER, 0);
}

TIPParser::NumExprContext::NumExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::NumExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitNumExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdExprContext ------------------------------------------------------------------

tree::TerminalNode* TIPParser::IdExprContext::IDENTIFIER() {
  return getToken(TIPParser::IDENTIFIER, 0);
}

TIPParser::IdExprContext::IdExprContext(AtomContext *ctx) { copyFrom(ctx); }


antlrcpp::Any TIPParser::IdExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitIdExpr(this);
  else
    return visitor->visitChildren(this);
}
TIPParser::AtomContext* TIPParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 6, TIPParser::RuleAtom);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(111);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::ParenAtomContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(98);
      parenExpr();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::FunAppAtomContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(99);
      funAppExpr();
      break;
    }

    case 3: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::NumExprContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(100);
      match(TIPParser::NUMBER);
      break;
    }

    case 4: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::IdExprContext>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(101);
      match(TIPParser::IDENTIFIER);
      break;
    }

    case 5: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::InputExprContext>(_localctx));
      enterOuterAlt(_localctx, 5);
      setState(102);
      match(TIPParser::KINPUT);
      break;
    }

    case 6: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::AllocExprContext>(_localctx));
      enterOuterAlt(_localctx, 6);
      setState(103);
      match(TIPParser::KALLOC);
      setState(104);
      expr(0);
      break;
    }

    case 7: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::RefExprContext>(_localctx));
      enterOuterAlt(_localctx, 7);
      setState(105);
      match(TIPParser::T__5);
      setState(106);
      match(TIPParser::IDENTIFIER);
      break;
    }

    case 8: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::DeRefAtomContext>(_localctx));
      enterOuterAlt(_localctx, 8);
      setState(107);
      deRefExpr();
      break;
    }

    case 9: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::NullExprContext>(_localctx));
      enterOuterAlt(_localctx, 9);
      setState(108);
      match(TIPParser::KNULL);
      break;
    }

    case 10: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::RecordAtomContext>(_localctx));
      enterOuterAlt(_localctx, 10);
      setState(109);
      recordExpr();
      break;
    }

    case 11: {
      _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<TIPParser::RecordAtomContext>(_localctx));
      enterOuterAlt(_localctx, 11);
      setState(110);
      accessExpr();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParenExprContext ------------------------------------------------------------------

TIPParser::ParenExprContext::ParenExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TIPParser::ExprContext* TIPParser::ParenExprContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::ParenExprContext::getRuleIndex() const {
  return TIPParser::RuleParenExpr;
}


antlrcpp::Any TIPParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::ParenExprContext* TIPParser::parenExpr() {
  ParenExprContext *_localctx = _tracker.createInstance<ParenExprContext>(_ctx, getState());
  enterRule(_localctx, 8, TIPParser::RuleParenExpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    match(TIPParser::T__0);
    setState(114);
    expr(0);
    setState(115);
    match(TIPParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeRefExprContext ------------------------------------------------------------------

TIPParser::DeRefExprContext::DeRefExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::DeRefExprContext::MUL() {
  return getToken(TIPParser::MUL, 0);
}

TIPParser::AtomContext* TIPParser::DeRefExprContext::atom() {
  return getRuleContext<TIPParser::AtomContext>(0);
}


size_t TIPParser::DeRefExprContext::getRuleIndex() const {
  return TIPParser::RuleDeRefExpr;
}


antlrcpp::Any TIPParser::DeRefExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitDeRefExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::DeRefExprContext* TIPParser::deRefExpr() {
  DeRefExprContext *_localctx = _tracker.createInstance<DeRefExprContext>(_ctx, getState());
  enterRule(_localctx, 10, TIPParser::RuleDeRefExpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(117);
    match(TIPParser::MUL);
    setState(118);
    atom();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunAppExprContext ------------------------------------------------------------------

TIPParser::FunAppExprContext::FunAppExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TIPParser::ParenExprContext* TIPParser::FunAppExprContext::parenExpr() {
  return getRuleContext<TIPParser::ParenExprContext>(0);
}

tree::TerminalNode* TIPParser::FunAppExprContext::IDENTIFIER() {
  return getToken(TIPParser::IDENTIFIER, 0);
}

std::vector<TIPParser::ExprContext *> TIPParser::FunAppExprContext::expr() {
  return getRuleContexts<TIPParser::ExprContext>();
}

TIPParser::ExprContext* TIPParser::FunAppExprContext::expr(size_t i) {
  return getRuleContext<TIPParser::ExprContext>(i);
}


size_t TIPParser::FunAppExprContext::getRuleIndex() const {
  return TIPParser::RuleFunAppExpr;
}


antlrcpp::Any TIPParser::FunAppExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitFunAppExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::FunAppExprContext* TIPParser::funAppExpr() {
  FunAppExprContext *_localctx = _tracker.createInstance<FunAppExprContext>(_ctx, getState());
  enterRule(_localctx, 12, TIPParser::RuleFunAppExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TIPParser::T__0: {
        setState(120);
        parenExpr();
        break;
      }

      case TIPParser::IDENTIFIER: {
        setState(121);
        match(TIPParser::IDENTIFIER);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(124);
    match(TIPParser::T__0);
    setState(133);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << TIPParser::T__0)
      | (1ULL << TIPParser::T__3)
      | (1ULL << TIPParser::T__5)
      | (1ULL << TIPParser::MUL)
      | (1ULL << TIPParser::SUB)
      | (1ULL << TIPParser::NUMBER)
      | (1ULL << TIPParser::KALLOC)
      | (1ULL << TIPParser::KINPUT)
      | (1ULL << TIPParser::KNULL)
      | (1ULL << TIPParser::IDENTIFIER))) != 0)) {
      setState(125);
      expr(0);
      setState(130);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == TIPParser::T__1) {
        setState(126);
        match(TIPParser::T__1);
        setState(127);
        expr(0);
        setState(132);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(135);
    match(TIPParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RecordExprContext ------------------------------------------------------------------

TIPParser::RecordExprContext::RecordExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TIPParser::FieldExprContext *> TIPParser::RecordExprContext::fieldExpr() {
  return getRuleContexts<TIPParser::FieldExprContext>();
}

TIPParser::FieldExprContext* TIPParser::RecordExprContext::fieldExpr(size_t i) {
  return getRuleContext<TIPParser::FieldExprContext>(i);
}


size_t TIPParser::RecordExprContext::getRuleIndex() const {
  return TIPParser::RuleRecordExpr;
}


antlrcpp::Any TIPParser::RecordExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitRecordExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::RecordExprContext* TIPParser::recordExpr() {
  RecordExprContext *_localctx = _tracker.createInstance<RecordExprContext>(_ctx, getState());
  enterRule(_localctx, 14, TIPParser::RuleRecordExpr);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(137);
    match(TIPParser::T__3);
    setState(146);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TIPParser::IDENTIFIER) {
      setState(138);
      fieldExpr();
      setState(143);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == TIPParser::T__1) {
        setState(139);
        match(TIPParser::T__1);
        setState(140);
        fieldExpr();
        setState(145);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(148);
    match(TIPParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldExprContext ------------------------------------------------------------------

TIPParser::FieldExprContext::FieldExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::FieldExprContext::IDENTIFIER() {
  return getToken(TIPParser::IDENTIFIER, 0);
}

TIPParser::ExprContext* TIPParser::FieldExprContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::FieldExprContext::getRuleIndex() const {
  return TIPParser::RuleFieldExpr;
}


antlrcpp::Any TIPParser::FieldExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitFieldExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::FieldExprContext* TIPParser::fieldExpr() {
  FieldExprContext *_localctx = _tracker.createInstance<FieldExprContext>(_ctx, getState());
  enterRule(_localctx, 16, TIPParser::RuleFieldExpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(150);
    match(TIPParser::IDENTIFIER);
    setState(151);
    match(TIPParser::T__6);
    setState(152);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AccessExprContext ------------------------------------------------------------------

TIPParser::AccessExprContext::AccessExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> TIPParser::AccessExprContext::IDENTIFIER() {
  return getTokens(TIPParser::IDENTIFIER);
}

tree::TerminalNode* TIPParser::AccessExprContext::IDENTIFIER(size_t i) {
  return getToken(TIPParser::IDENTIFIER, i);
}

TIPParser::DeRefExprContext* TIPParser::AccessExprContext::deRefExpr() {
  return getRuleContext<TIPParser::DeRefExprContext>(0);
}

TIPParser::ParenExprContext* TIPParser::AccessExprContext::parenExpr() {
  return getRuleContext<TIPParser::ParenExprContext>(0);
}


size_t TIPParser::AccessExprContext::getRuleIndex() const {
  return TIPParser::RuleAccessExpr;
}


antlrcpp::Any TIPParser::AccessExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAccessExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::AccessExprContext* TIPParser::accessExpr() {
  AccessExprContext *_localctx = _tracker.createInstance<AccessExprContext>(_ctx, getState());
  enterRule(_localctx, 18, TIPParser::RuleAccessExpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(157);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TIPParser::IDENTIFIER: {
        setState(154);
        match(TIPParser::IDENTIFIER);
        break;
      }

      case TIPParser::MUL: {
        setState(155);
        deRefExpr();
        break;
      }

      case TIPParser::T__0: {
        setState(156);
        parenExpr();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(159);
    match(TIPParser::T__7);
    setState(160);
    match(TIPParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignableExprContext ------------------------------------------------------------------

TIPParser::AssignableExprContext::AssignableExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::AssignableExprContext::IDENTIFIER() {
  return getToken(TIPParser::IDENTIFIER, 0);
}

TIPParser::DeRefExprContext* TIPParser::AssignableExprContext::deRefExpr() {
  return getRuleContext<TIPParser::DeRefExprContext>(0);
}


size_t TIPParser::AssignableExprContext::getRuleIndex() const {
  return TIPParser::RuleAssignableExpr;
}


antlrcpp::Any TIPParser::AssignableExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAssignableExpr(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::AssignableExprContext* TIPParser::assignableExpr() {
  AssignableExprContext *_localctx = _tracker.createInstance<AssignableExprContext>(_ctx, getState());
  enterRule(_localctx, 20, TIPParser::RuleAssignableExpr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TIPParser::IDENTIFIER: {
        setState(162);
        match(TIPParser::IDENTIFIER);
        break;
      }

      case TIPParser::MUL: {
        setState(163);
        deRefExpr();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

TIPParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::DeclarationContext::KVAR() {
  return getToken(TIPParser::KVAR, 0);
}

std::vector<tree::TerminalNode *> TIPParser::DeclarationContext::IDENTIFIER() {
  return getTokens(TIPParser::IDENTIFIER);
}

tree::TerminalNode* TIPParser::DeclarationContext::IDENTIFIER(size_t i) {
  return getToken(TIPParser::IDENTIFIER, i);
}


size_t TIPParser::DeclarationContext::getRuleIndex() const {
  return TIPParser::RuleDeclaration;
}


antlrcpp::Any TIPParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::DeclarationContext* TIPParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 22, TIPParser::RuleDeclaration);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(166);
    match(TIPParser::KVAR);
    setState(167);
    match(TIPParser::IDENTIFIER);
    setState(172);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TIPParser::T__1) {
      setState(168);
      match(TIPParser::T__1);
      setState(169);
      match(TIPParser::IDENTIFIER);
      setState(174);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(175);
    match(TIPParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

TIPParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TIPParser::BlockStmtContext* TIPParser::StatementContext::blockStmt() {
  return getRuleContext<TIPParser::BlockStmtContext>(0);
}

TIPParser::AssignmentStmtContext* TIPParser::StatementContext::assignmentStmt() {
  return getRuleContext<TIPParser::AssignmentStmtContext>(0);
}

TIPParser::WhileStmtContext* TIPParser::StatementContext::whileStmt() {
  return getRuleContext<TIPParser::WhileStmtContext>(0);
}

TIPParser::IfStmtContext* TIPParser::StatementContext::ifStmt() {
  return getRuleContext<TIPParser::IfStmtContext>(0);
}

TIPParser::OutputStmtContext* TIPParser::StatementContext::outputStmt() {
  return getRuleContext<TIPParser::OutputStmtContext>(0);
}

TIPParser::ErrorStmtContext* TIPParser::StatementContext::errorStmt() {
  return getRuleContext<TIPParser::ErrorStmtContext>(0);
}


size_t TIPParser::StatementContext::getRuleIndex() const {
  return TIPParser::RuleStatement;
}


antlrcpp::Any TIPParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::StatementContext* TIPParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 24, TIPParser::RuleStatement);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(183);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TIPParser::T__3: {
        enterOuterAlt(_localctx, 1);
        setState(177);
        blockStmt();
        break;
      }

      case TIPParser::MUL:
      case TIPParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(178);
        assignmentStmt();
        break;
      }

      case TIPParser::KWHILE: {
        enterOuterAlt(_localctx, 3);
        setState(179);
        whileStmt();
        break;
      }

      case TIPParser::KIF: {
        enterOuterAlt(_localctx, 4);
        setState(180);
        ifStmt();
        break;
      }

      case TIPParser::KOUTPUT: {
        enterOuterAlt(_localctx, 5);
        setState(181);
        outputStmt();
        break;
      }

      case TIPParser::KERROR: {
        enterOuterAlt(_localctx, 6);
        setState(182);
        errorStmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentStmtContext ------------------------------------------------------------------

TIPParser::AssignmentStmtContext::AssignmentStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TIPParser::AssignableExprContext* TIPParser::AssignmentStmtContext::assignableExpr() {
  return getRuleContext<TIPParser::AssignableExprContext>(0);
}

TIPParser::ExprContext* TIPParser::AssignmentStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::AssignmentStmtContext::getRuleIndex() const {
  return TIPParser::RuleAssignmentStmt;
}


antlrcpp::Any TIPParser::AssignmentStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitAssignmentStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::AssignmentStmtContext* TIPParser::assignmentStmt() {
  AssignmentStmtContext *_localctx = _tracker.createInstance<AssignmentStmtContext>(_ctx, getState());
  enterRule(_localctx, 26, TIPParser::RuleAssignmentStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
    assignableExpr();
    setState(186);
    match(TIPParser::T__9);
    setState(187);
    expr(0);
    setState(188);
    match(TIPParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockStmtContext ------------------------------------------------------------------

TIPParser::BlockStmtContext::BlockStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TIPParser::StatementContext *> TIPParser::BlockStmtContext::statement() {
  return getRuleContexts<TIPParser::StatementContext>();
}

TIPParser::StatementContext* TIPParser::BlockStmtContext::statement(size_t i) {
  return getRuleContext<TIPParser::StatementContext>(i);
}


size_t TIPParser::BlockStmtContext::getRuleIndex() const {
  return TIPParser::RuleBlockStmt;
}


antlrcpp::Any TIPParser::BlockStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitBlockStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::BlockStmtContext* TIPParser::blockStmt() {
  BlockStmtContext *_localctx = _tracker.createInstance<BlockStmtContext>(_ctx, getState());
  enterRule(_localctx, 28, TIPParser::RuleBlockStmt);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(190);
    match(TIPParser::T__3);

    setState(194);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << TIPParser::T__3)
      | (1ULL << TIPParser::MUL)
      | (1ULL << TIPParser::KWHILE)
      | (1ULL << TIPParser::KIF)
      | (1ULL << TIPParser::KOUTPUT)
      | (1ULL << TIPParser::KERROR)
      | (1ULL << TIPParser::IDENTIFIER))) != 0)) {
      setState(191);
      statement();
      setState(196);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(197);
    match(TIPParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStmtContext ------------------------------------------------------------------

TIPParser::WhileStmtContext::WhileStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::WhileStmtContext::KWHILE() {
  return getToken(TIPParser::KWHILE, 0);
}

TIPParser::ExprContext* TIPParser::WhileStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}

TIPParser::StatementContext* TIPParser::WhileStmtContext::statement() {
  return getRuleContext<TIPParser::StatementContext>(0);
}


size_t TIPParser::WhileStmtContext::getRuleIndex() const {
  return TIPParser::RuleWhileStmt;
}


antlrcpp::Any TIPParser::WhileStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitWhileStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::WhileStmtContext* TIPParser::whileStmt() {
  WhileStmtContext *_localctx = _tracker.createInstance<WhileStmtContext>(_ctx, getState());
  enterRule(_localctx, 30, TIPParser::RuleWhileStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(199);
    match(TIPParser::KWHILE);
    setState(200);
    match(TIPParser::T__0);
    setState(201);
    expr(0);
    setState(202);
    match(TIPParser::T__2);
    setState(203);
    statement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStmtContext ------------------------------------------------------------------

TIPParser::IfStmtContext::IfStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::IfStmtContext::KIF() {
  return getToken(TIPParser::KIF, 0);
}

TIPParser::ExprContext* TIPParser::IfStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}

std::vector<TIPParser::StatementContext *> TIPParser::IfStmtContext::statement() {
  return getRuleContexts<TIPParser::StatementContext>();
}

TIPParser::StatementContext* TIPParser::IfStmtContext::statement(size_t i) {
  return getRuleContext<TIPParser::StatementContext>(i);
}

tree::TerminalNode* TIPParser::IfStmtContext::KELSE() {
  return getToken(TIPParser::KELSE, 0);
}


size_t TIPParser::IfStmtContext::getRuleIndex() const {
  return TIPParser::RuleIfStmt;
}


antlrcpp::Any TIPParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::IfStmtContext* TIPParser::ifStmt() {
  IfStmtContext *_localctx = _tracker.createInstance<IfStmtContext>(_ctx, getState());
  enterRule(_localctx, 32, TIPParser::RuleIfStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(205);
    match(TIPParser::KIF);
    setState(206);
    match(TIPParser::T__0);
    setState(207);
    expr(0);
    setState(208);
    match(TIPParser::T__2);
    setState(209);
    statement();
    setState(212);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
    case 1: {
      setState(210);
      match(TIPParser::KELSE);
      setState(211);
      statement();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OutputStmtContext ------------------------------------------------------------------

TIPParser::OutputStmtContext::OutputStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::OutputStmtContext::KOUTPUT() {
  return getToken(TIPParser::KOUTPUT, 0);
}

TIPParser::ExprContext* TIPParser::OutputStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::OutputStmtContext::getRuleIndex() const {
  return TIPParser::RuleOutputStmt;
}


antlrcpp::Any TIPParser::OutputStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitOutputStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::OutputStmtContext* TIPParser::outputStmt() {
  OutputStmtContext *_localctx = _tracker.createInstance<OutputStmtContext>(_ctx, getState());
  enterRule(_localctx, 34, TIPParser::RuleOutputStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(214);
    match(TIPParser::KOUTPUT);
    setState(215);
    expr(0);
    setState(216);
    match(TIPParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ErrorStmtContext ------------------------------------------------------------------

TIPParser::ErrorStmtContext::ErrorStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::ErrorStmtContext::KERROR() {
  return getToken(TIPParser::KERROR, 0);
}

TIPParser::ExprContext* TIPParser::ErrorStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::ErrorStmtContext::getRuleIndex() const {
  return TIPParser::RuleErrorStmt;
}


antlrcpp::Any TIPParser::ErrorStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitErrorStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::ErrorStmtContext* TIPParser::errorStmt() {
  ErrorStmtContext *_localctx = _tracker.createInstance<ErrorStmtContext>(_ctx, getState());
  enterRule(_localctx, 36, TIPParser::RuleErrorStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(218);
    match(TIPParser::KERROR);
    setState(219);
    expr(0);
    setState(220);
    match(TIPParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

TIPParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TIPParser::ReturnStmtContext::KRETURN() {
  return getToken(TIPParser::KRETURN, 0);
}

TIPParser::ExprContext* TIPParser::ReturnStmtContext::expr() {
  return getRuleContext<TIPParser::ExprContext>(0);
}


size_t TIPParser::ReturnStmtContext::getRuleIndex() const {
  return TIPParser::RuleReturnStmt;
}


antlrcpp::Any TIPParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TIPVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

TIPParser::ReturnStmtContext* TIPParser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 38, TIPParser::RuleReturnStmt);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(222);
    match(TIPParser::KRETURN);
    setState(223);
    expr(0);
    setState(224);
    match(TIPParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool TIPParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 2: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool TIPParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 5);
    case 1: return precpred(_ctx, 4);
    case 2: return precpred(_ctx, 3);
    case 3: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> TIPParser::_decisionToDFA;
atn::PredictionContextCache TIPParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN TIPParser::_atn;
std::vector<uint16_t> TIPParser::_serializedATN;

std::vector<std::string> TIPParser::_ruleNames = {
  "program", "function", "expr", "atom", "parenExpr", "deRefExpr", "funAppExpr", 
  "recordExpr", "fieldExpr", "accessExpr", "assignableExpr", "declaration", 
  "statement", "assignmentStmt", "blockStmt", "whileStmt", "ifStmt", "outputStmt", 
  "errorStmt", "returnStmt"
};

std::vector<std::string> TIPParser::_literalNames = {
  "", "'('", "','", "')'", "'{'", "'}'", "'&'", "':'", "'.'", "';'", "'='", 
  "'*'", "'/'", "'+'", "'-'", "'>'", "'=='", "'!='", "", "'alloc'", "'input'", 
  "'while'", "'if'", "'else'", "'var'", "'return'", "'null'", "'output'", 
  "'error'"
};

std::vector<std::string> TIPParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "MUL", "DIV", "ADD", "SUB", 
  "GT", "EQ", "NE", "NUMBER", "KALLOC", "KINPUT", "KWHILE", "KIF", "KELSE", 
  "KVAR", "KRETURN", "KNULL", "KOUTPUT", "KERROR", "IDENTIFIER", "WS", "BLOCKCOMMENT", 
  "COMMENT"
};

dfa::Vocabulary TIPParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> TIPParser::_tokenNames;

TIPParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x22, 0xe5, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 
    0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 0x12, 
    0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 0x9, 
    0x15, 0x3, 0x2, 0x7, 0x2, 0x2c, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x2f, 
    0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 
    0x36, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x39, 0xb, 0x3, 0x5, 0x3, 0x3b, 
    0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x40, 0xa, 0x3, 0xc, 
    0x3, 0xe, 0x3, 0x43, 0xb, 0x3, 0x3, 0x3, 0x7, 0x3, 0x46, 0xa, 0x3, 0xc, 
    0x3, 0xe, 0x3, 0x49, 0xb, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x52, 0xa, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x60, 0xa, 0x4, 0xc, 0x4, 
    0xe, 0x4, 0x63, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x5, 0x5, 0x72, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 
    0x7d, 0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x83, 
    0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x86, 0xb, 0x8, 0x5, 0x8, 0x88, 0xa, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 
    0x90, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x93, 0xb, 0x9, 0x5, 0x9, 0x95, 
    0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xa0, 0xa, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0xa7, 0xa, 0xc, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xad, 0xa, 0xd, 0xc, 0xd, 0xe, 
    0xd, 0xb0, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xba, 0xa, 0xe, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 
    0xc3, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0xc6, 0xb, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x5, 0x12, 0xd7, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 
    0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x2, 0x3, 0x6, 0x16, 0x2, 0x4, 0x6, 
    0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2, 0x4, 0x3, 0x2, 0xd, 0xe, 0x3, 0x2, 
    0xf, 0x10, 0x2, 0xf4, 0x2, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x4, 0x30, 0x3, 
    0x2, 0x2, 0x2, 0x6, 0x51, 0x3, 0x2, 0x2, 0x2, 0x8, 0x71, 0x3, 0x2, 0x2, 
    0x2, 0xa, 0x73, 0x3, 0x2, 0x2, 0x2, 0xc, 0x77, 0x3, 0x2, 0x2, 0x2, 0xe, 
    0x7c, 0x3, 0x2, 0x2, 0x2, 0x10, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x12, 0x98, 
    0x3, 0x2, 0x2, 0x2, 0x14, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x16, 0xa6, 0x3, 
    0x2, 0x2, 0x2, 0x18, 0xa8, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xb9, 0x3, 0x2, 
    0x2, 0x2, 0x1c, 0xbb, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xc0, 0x3, 0x2, 0x2, 
    0x2, 0x20, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x22, 0xcf, 0x3, 0x2, 0x2, 0x2, 
    0x24, 0xd8, 0x3, 0x2, 0x2, 0x2, 0x26, 0xdc, 0x3, 0x2, 0x2, 0x2, 0x28, 
    0xe0, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2c, 0x5, 0x4, 0x3, 0x2, 0x2b, 0x2a, 
    0x3, 0x2, 0x2, 0x2, 0x2c, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x2d, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2f, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x30, 0x31, 0x7, 0x1f, 0x2, 
    0x2, 0x31, 0x3a, 0x7, 0x3, 0x2, 0x2, 0x32, 0x37, 0x7, 0x1f, 0x2, 0x2, 
    0x33, 0x34, 0x7, 0x4, 0x2, 0x2, 0x34, 0x36, 0x7, 0x1f, 0x2, 0x2, 0x35, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x36, 0x39, 0x3, 0x2, 0x2, 0x2, 0x37, 0x35, 
    0x3, 0x2, 0x2, 0x2, 0x37, 0x38, 0x3, 0x2, 0x2, 0x2, 0x38, 0x3b, 0x3, 
    0x2, 0x2, 0x2, 0x39, 0x37, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x32, 0x3, 0x2, 
    0x2, 0x2, 0x3a, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x3d, 0x7, 0x5, 0x2, 0x2, 0x3d, 0x41, 0x7, 0x6, 0x2, 0x2, 
    0x3e, 0x40, 0x5, 0x18, 0xd, 0x2, 0x3f, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x43, 0x3, 0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x47, 0x3, 0x2, 0x2, 0x2, 0x43, 0x41, 0x3, 
    0x2, 0x2, 0x2, 0x44, 0x46, 0x5, 0x1a, 0xe, 0x2, 0x45, 0x44, 0x3, 0x2, 
    0x2, 0x2, 0x46, 0x49, 0x3, 0x2, 0x2, 0x2, 0x47, 0x45, 0x3, 0x2, 0x2, 
    0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 0x2, 0x48, 0x4a, 0x3, 0x2, 0x2, 0x2, 
    0x49, 0x47, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x5, 0x28, 0x15, 0x2, 0x4b, 
    0x4c, 0x7, 0x7, 0x2, 0x2, 0x4c, 0x5, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 
    0x8, 0x4, 0x1, 0x2, 0x4e, 0x4f, 0x7, 0x10, 0x2, 0x2, 0x4f, 0x52, 0x7, 
    0x14, 0x2, 0x2, 0x50, 0x52, 0x5, 0x8, 0x5, 0x2, 0x51, 0x4d, 0x3, 0x2, 
    0x2, 0x2, 0x51, 0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 0x61, 0x3, 0x2, 0x2, 
    0x2, 0x53, 0x54, 0xc, 0x7, 0x2, 0x2, 0x54, 0x55, 0x9, 0x2, 0x2, 0x2, 
    0x55, 0x60, 0x5, 0x6, 0x4, 0x8, 0x56, 0x57, 0xc, 0x6, 0x2, 0x2, 0x57, 
    0x58, 0x9, 0x3, 0x2, 0x2, 0x58, 0x60, 0x5, 0x6, 0x4, 0x7, 0x59, 0x5a, 
    0xc, 0x5, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x11, 0x2, 0x2, 0x5b, 0x60, 0x5, 
    0x6, 0x4, 0x6, 0x5c, 0x5d, 0xc, 0x4, 0x2, 0x2, 0x5d, 0x5e, 0x7, 0x12, 
    0x2, 0x2, 0x5e, 0x60, 0x5, 0x6, 0x4, 0x5, 0x5f, 0x53, 0x3, 0x2, 0x2, 
    0x2, 0x5f, 0x56, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x59, 0x3, 0x2, 0x2, 0x2, 
    0x5f, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x60, 0x63, 0x3, 0x2, 0x2, 0x2, 0x61, 
    0x5f, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x3, 0x2, 0x2, 0x2, 0x62, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x63, 0x61, 0x3, 0x2, 0x2, 0x2, 0x64, 0x72, 0x5, 
    0xa, 0x6, 0x2, 0x65, 0x72, 0x5, 0xe, 0x8, 0x2, 0x66, 0x72, 0x7, 0x14, 
    0x2, 0x2, 0x67, 0x72, 0x7, 0x1f, 0x2, 0x2, 0x68, 0x72, 0x7, 0x16, 0x2, 
    0x2, 0x69, 0x6a, 0x7, 0x15, 0x2, 0x2, 0x6a, 0x72, 0x5, 0x6, 0x4, 0x2, 
    0x6b, 0x6c, 0x7, 0x8, 0x2, 0x2, 0x6c, 0x72, 0x7, 0x1f, 0x2, 0x2, 0x6d, 
    0x72, 0x5, 0xc, 0x7, 0x2, 0x6e, 0x72, 0x7, 0x1c, 0x2, 0x2, 0x6f, 0x72, 
    0x5, 0x10, 0x9, 0x2, 0x70, 0x72, 0x5, 0x14, 0xb, 0x2, 0x71, 0x64, 0x3, 
    0x2, 0x2, 0x2, 0x71, 0x65, 0x3, 0x2, 0x2, 0x2, 0x71, 0x66, 0x3, 0x2, 
    0x2, 0x2, 0x71, 0x67, 0x3, 0x2, 0x2, 0x2, 0x71, 0x68, 0x3, 0x2, 0x2, 
    0x2, 0x71, 0x69, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6b, 0x3, 0x2, 0x2, 0x2, 
    0x71, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x71, 
    0x6f, 0x3, 0x2, 0x2, 0x2, 0x71, 0x70, 0x3, 0x2, 0x2, 0x2, 0x72, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x7, 0x3, 0x2, 0x2, 0x74, 0x75, 0x5, 
    0x6, 0x4, 0x2, 0x75, 0x76, 0x7, 0x5, 0x2, 0x2, 0x76, 0xb, 0x3, 0x2, 
    0x2, 0x2, 0x77, 0x78, 0x7, 0xd, 0x2, 0x2, 0x78, 0x79, 0x5, 0x8, 0x5, 
    0x2, 0x79, 0xd, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7d, 0x5, 0xa, 0x6, 0x2, 
    0x7b, 0x7d, 0x7, 0x1f, 0x2, 0x2, 0x7c, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7c, 
    0x7b, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x87, 
    0x7, 0x3, 0x2, 0x2, 0x7f, 0x84, 0x5, 0x6, 0x4, 0x2, 0x80, 0x81, 0x7, 
    0x4, 0x2, 0x2, 0x81, 0x83, 0x5, 0x6, 0x4, 0x2, 0x82, 0x80, 0x3, 0x2, 
    0x2, 0x2, 0x83, 0x86, 0x3, 0x2, 0x2, 0x2, 0x84, 0x82, 0x3, 0x2, 0x2, 
    0x2, 0x84, 0x85, 0x3, 0x2, 0x2, 0x2, 0x85, 0x88, 0x3, 0x2, 0x2, 0x2, 
    0x86, 0x84, 0x3, 0x2, 0x2, 0x2, 0x87, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x87, 
    0x88, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8a, 
    0x7, 0x5, 0x2, 0x2, 0x8a, 0xf, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x94, 0x7, 
    0x6, 0x2, 0x2, 0x8c, 0x91, 0x5, 0x12, 0xa, 0x2, 0x8d, 0x8e, 0x7, 0x4, 
    0x2, 0x2, 0x8e, 0x90, 0x5, 0x12, 0xa, 0x2, 0x8f, 0x8d, 0x3, 0x2, 0x2, 
    0x2, 0x90, 0x93, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8f, 0x3, 0x2, 0x2, 0x2, 
    0x91, 0x92, 0x3, 0x2, 0x2, 0x2, 0x92, 0x95, 0x3, 0x2, 0x2, 0x2, 0x93, 
    0x91, 0x3, 0x2, 0x2, 0x2, 0x94, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x94, 0x95, 
    0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 0x7, 
    0x7, 0x2, 0x2, 0x97, 0x11, 0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 0x7, 0x1f, 
    0x2, 0x2, 0x99, 0x9a, 0x7, 0x9, 0x2, 0x2, 0x9a, 0x9b, 0x5, 0x6, 0x4, 
    0x2, 0x9b, 0x13, 0x3, 0x2, 0x2, 0x2, 0x9c, 0xa0, 0x7, 0x1f, 0x2, 0x2, 
    0x9d, 0xa0, 0x5, 0xc, 0x7, 0x2, 0x9e, 0xa0, 0x5, 0xa, 0x6, 0x2, 0x9f, 
    0x9c, 0x3, 0x2, 0x2, 0x2, 0x9f, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9f, 0x9e, 
    0x3, 0x2, 0x2, 0x2, 0xa0, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa2, 0x7, 
    0xa, 0x2, 0x2, 0xa2, 0xa3, 0x7, 0x1f, 0x2, 0x2, 0xa3, 0x15, 0x3, 0x2, 
    0x2, 0x2, 0xa4, 0xa7, 0x7, 0x1f, 0x2, 0x2, 0xa5, 0xa7, 0x5, 0xc, 0x7, 
    0x2, 0xa6, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa5, 0x3, 0x2, 0x2, 0x2, 
    0xa7, 0x17, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xa9, 0x7, 0x1a, 0x2, 0x2, 0xa9, 
    0xae, 0x7, 0x1f, 0x2, 0x2, 0xaa, 0xab, 0x7, 0x4, 0x2, 0x2, 0xab, 0xad, 
    0x7, 0x1f, 0x2, 0x2, 0xac, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xad, 0xb0, 0x3, 
    0x2, 0x2, 0x2, 0xae, 0xac, 0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 0x3, 0x2, 
    0x2, 0x2, 0xaf, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xae, 0x3, 0x2, 0x2, 
    0x2, 0xb1, 0xb2, 0x7, 0xb, 0x2, 0x2, 0xb2, 0x19, 0x3, 0x2, 0x2, 0x2, 
    0xb3, 0xba, 0x5, 0x1e, 0x10, 0x2, 0xb4, 0xba, 0x5, 0x1c, 0xf, 0x2, 0xb5, 
    0xba, 0x5, 0x20, 0x11, 0x2, 0xb6, 0xba, 0x5, 0x22, 0x12, 0x2, 0xb7, 
    0xba, 0x5, 0x24, 0x13, 0x2, 0xb8, 0xba, 0x5, 0x26, 0x14, 0x2, 0xb9, 
    0xb3, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xb5, 
    0x3, 0x2, 0x2, 0x2, 0xb9, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xb7, 0x3, 
    0x2, 0x2, 0x2, 0xb9, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xba, 0x1b, 0x3, 0x2, 
    0x2, 0x2, 0xbb, 0xbc, 0x5, 0x16, 0xc, 0x2, 0xbc, 0xbd, 0x7, 0xc, 0x2, 
    0x2, 0xbd, 0xbe, 0x5, 0x6, 0x4, 0x2, 0xbe, 0xbf, 0x7, 0xb, 0x2, 0x2, 
    0xbf, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc4, 0x7, 0x6, 0x2, 0x2, 0xc1, 
    0xc3, 0x5, 0x1a, 0xe, 0x2, 0xc2, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc6, 
    0x3, 0x2, 0x2, 0x2, 0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc4, 0xc5, 0x3, 
    0x2, 0x2, 0x2, 0xc5, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc4, 0x3, 0x2, 
    0x2, 0x2, 0xc7, 0xc8, 0x7, 0x7, 0x2, 0x2, 0xc8, 0x1f, 0x3, 0x2, 0x2, 
    0x2, 0xc9, 0xca, 0x7, 0x17, 0x2, 0x2, 0xca, 0xcb, 0x7, 0x3, 0x2, 0x2, 
    0xcb, 0xcc, 0x5, 0x6, 0x4, 0x2, 0xcc, 0xcd, 0x7, 0x5, 0x2, 0x2, 0xcd, 
    0xce, 0x5, 0x1a, 0xe, 0x2, 0xce, 0x21, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 
    0x7, 0x18, 0x2, 0x2, 0xd0, 0xd1, 0x7, 0x3, 0x2, 0x2, 0xd1, 0xd2, 0x5, 
    0x6, 0x4, 0x2, 0xd2, 0xd3, 0x7, 0x5, 0x2, 0x2, 0xd3, 0xd6, 0x5, 0x1a, 
    0xe, 0x2, 0xd4, 0xd5, 0x7, 0x19, 0x2, 0x2, 0xd5, 0xd7, 0x5, 0x1a, 0xe, 
    0x2, 0xd6, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd7, 0x3, 0x2, 0x2, 0x2, 
    0xd7, 0x23, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xd9, 0x7, 0x1d, 0x2, 0x2, 0xd9, 
    0xda, 0x5, 0x6, 0x4, 0x2, 0xda, 0xdb, 0x7, 0xb, 0x2, 0x2, 0xdb, 0x25, 
    0x3, 0x2, 0x2, 0x2, 0xdc, 0xdd, 0x7, 0x1e, 0x2, 0x2, 0xdd, 0xde, 0x5, 
    0x6, 0x4, 0x2, 0xde, 0xdf, 0x7, 0xb, 0x2, 0x2, 0xdf, 0x27, 0x3, 0x2, 
    0x2, 0x2, 0xe0, 0xe1, 0x7, 0x1b, 0x2, 0x2, 0xe1, 0xe2, 0x5, 0x6, 0x4, 
    0x2, 0xe2, 0xe3, 0x7, 0xb, 0x2, 0x2, 0xe3, 0x29, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x2d, 0x37, 0x3a, 0x41, 0x47, 0x51, 0x5f, 0x61, 0x71, 0x7c, 0x84, 
    0x87, 0x91, 0x94, 0x9f, 0xa6, 0xae, 0xb9, 0xc4, 0xd6, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

TIPParser::Initializer TIPParser::_init;
