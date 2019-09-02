// Generated from TIP.g4 by ANTLR 4.7.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class TIPParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.7.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, MUL=11, DIV=12, ADD=13, SUB=14, GT=15, EQ=16, NE=17, NUMBER=18, 
		KALLOC=19, KINPUT=20, KWHILE=21, KIF=22, KELSE=23, KVAR=24, KRETURN=25, 
		KNULL=26, KOUTPUT=27, KERROR=28, IDENTIFIER=29, WS=30, BLOCKCOMMENT=31, 
		COMMENT=32;
	public static final int
		RULE_program = 0, RULE_function = 1, RULE_expr = 2, RULE_atom = 3, RULE_parenExpr = 4, 
		RULE_deRefExpr = 5, RULE_funAppExpr = 6, RULE_recordExpr = 7, RULE_fieldExpr = 8, 
		RULE_accessExpr = 9, RULE_assignableExpr = 10, RULE_declaration = 11, 
		RULE_statement = 12, RULE_assignmentStmt = 13, RULE_blockStmt = 14, RULE_whileStmt = 15, 
		RULE_ifStmt = 16, RULE_outputStmt = 17, RULE_errorStmt = 18, RULE_returnStmt = 19;
	private static String[] makeRuleNames() {
		return new String[] {
			"program", "function", "expr", "atom", "parenExpr", "deRefExpr", "funAppExpr", 
			"recordExpr", "fieldExpr", "accessExpr", "assignableExpr", "declaration", 
			"statement", "assignmentStmt", "blockStmt", "whileStmt", "ifStmt", "outputStmt", 
			"errorStmt", "returnStmt"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'('", "','", "')'", "'{'", "'}'", "'&'", "':'", "'.'", "';'", 
			"'='", "'*'", "'/'", "'+'", "'-'", "'>'", "'=='", "'!='", null, "'alloc'", 
			"'input'", "'while'", "'if'", "'else'", "'var'", "'return'", "'null'", 
			"'output'", "'error'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, "MUL", 
			"DIV", "ADD", "SUB", "GT", "EQ", "NE", "NUMBER", "KALLOC", "KINPUT", 
			"KWHILE", "KIF", "KELSE", "KVAR", "KRETURN", "KNULL", "KOUTPUT", "KERROR", 
			"IDENTIFIER", "WS", "BLOCKCOMMENT", "COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "TIP.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public TIPParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class ProgramContext extends ParserRuleContext {
		public List<FunctionContext> function() {
			return getRuleContexts(FunctionContext.class);
		}
		public FunctionContext function(int i) {
			return getRuleContext(FunctionContext.class,i);
		}
		public ProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_program; }
	}

	public final ProgramContext program() throws RecognitionException {
		ProgramContext _localctx = new ProgramContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_program);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(43);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==IDENTIFIER) {
				{
				{
				setState(40);
				function();
				}
				}
				setState(45);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunctionContext extends ParserRuleContext {
		public List<TerminalNode> IDENTIFIER() { return getTokens(TIPParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(TIPParser.IDENTIFIER, i);
		}
		public ReturnStmtContext returnStmt() {
			return getRuleContext(ReturnStmtContext.class,0);
		}
		public List<DeclarationContext> declaration() {
			return getRuleContexts(DeclarationContext.class);
		}
		public DeclarationContext declaration(int i) {
			return getRuleContext(DeclarationContext.class,i);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public FunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_function; }
	}

	public final FunctionContext function() throws RecognitionException {
		FunctionContext _localctx = new FunctionContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_function);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(46);
			match(IDENTIFIER);
			setState(47);
			match(T__0);
			setState(56);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==IDENTIFIER) {
				{
				setState(48);
				match(IDENTIFIER);
				setState(53);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__1) {
					{
					{
					setState(49);
					match(T__1);
					setState(50);
					match(IDENTIFIER);
					}
					}
					setState(55);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(58);
			match(T__2);
			setState(59);
			match(T__3);
			{
			setState(63);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==KVAR) {
				{
				{
				setState(60);
				declaration();
				}
				}
				setState(65);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
			{
			setState(69);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__3) | (1L << MUL) | (1L << KWHILE) | (1L << KIF) | (1L << KOUTPUT) | (1L << KERROR) | (1L << IDENTIFIER))) != 0)) {
				{
				{
				setState(66);
				statement();
				}
				}
				setState(71);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
			setState(72);
			returnStmt();
			setState(73);
			match(T__4);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExprContext extends ParserRuleContext {
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
	 
		public ExprContext() { }
		public void copyFrom(ExprContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class NegNumberContext extends ExprContext {
		public TerminalNode SUB() { return getToken(TIPParser.SUB, 0); }
		public TerminalNode NUMBER() { return getToken(TIPParser.NUMBER, 0); }
		public NegNumberContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class AtomExprContext extends ExprContext {
		public AtomContext atom() {
			return getRuleContext(AtomContext.class,0);
		}
		public AtomExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class AdditiveExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode ADD() { return getToken(TIPParser.ADD, 0); }
		public TerminalNode SUB() { return getToken(TIPParser.SUB, 0); }
		public AdditiveExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class RelationalExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode GT() { return getToken(TIPParser.GT, 0); }
		public RelationalExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class MultiplicativeExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode MUL() { return getToken(TIPParser.MUL, 0); }
		public TerminalNode DIV() { return getToken(TIPParser.DIV, 0); }
		public MultiplicativeExprContext(ExprContext ctx) { copyFrom(ctx); }
	}
	public static class EqualityExprContext extends ExprContext {
		public Token op;
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public TerminalNode EQ() { return getToken(TIPParser.EQ, 0); }
		public EqualityExprContext(ExprContext ctx) { copyFrom(ctx); }
	}

	public final ExprContext expr() throws RecognitionException {
		return expr(0);
	}

	private ExprContext expr(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExprContext _localctx = new ExprContext(_ctx, _parentState);
		ExprContext _prevctx = _localctx;
		int _startState = 4;
		enterRecursionRule(_localctx, 4, RULE_expr, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(79);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case SUB:
				{
				_localctx = new NegNumberContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(76);
				match(SUB);
				setState(77);
				match(NUMBER);
				}
				break;
			case T__0:
			case T__3:
			case T__5:
			case MUL:
			case NUMBER:
			case KALLOC:
			case KINPUT:
			case KNULL:
			case IDENTIFIER:
				{
				_localctx = new AtomExprContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(78);
				atom();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			_ctx.stop = _input.LT(-1);
			setState(95);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,7,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					setState(93);
					_errHandler.sync(this);
					switch ( getInterpreter().adaptivePredict(_input,6,_ctx) ) {
					case 1:
						{
						_localctx = new MultiplicativeExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(81);
						if (!(precpred(_ctx, 5))) throw new FailedPredicateException(this, "precpred(_ctx, 5)");
						setState(82);
						((MultiplicativeExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==MUL || _la==DIV) ) {
							((MultiplicativeExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(83);
						expr(6);
						}
						break;
					case 2:
						{
						_localctx = new AdditiveExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(84);
						if (!(precpred(_ctx, 4))) throw new FailedPredicateException(this, "precpred(_ctx, 4)");
						setState(85);
						((AdditiveExprContext)_localctx).op = _input.LT(1);
						_la = _input.LA(1);
						if ( !(_la==ADD || _la==SUB) ) {
							((AdditiveExprContext)_localctx).op = (Token)_errHandler.recoverInline(this);
						}
						else {
							if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
							_errHandler.reportMatch(this);
							consume();
						}
						setState(86);
						expr(5);
						}
						break;
					case 3:
						{
						_localctx = new RelationalExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(87);
						if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
						setState(88);
						((RelationalExprContext)_localctx).op = match(GT);
						setState(89);
						expr(4);
						}
						break;
					case 4:
						{
						_localctx = new EqualityExprContext(new ExprContext(_parentctx, _parentState));
						pushNewRecursionContext(_localctx, _startState, RULE_expr);
						setState(90);
						if (!(precpred(_ctx, 2))) throw new FailedPredicateException(this, "precpred(_ctx, 2)");
						setState(91);
						((EqualityExprContext)_localctx).op = match(EQ);
						setState(92);
						expr(3);
						}
						break;
					}
					} 
				}
				setState(97);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,7,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class AtomContext extends ParserRuleContext {
		public AtomContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_atom; }
	 
		public AtomContext() { }
		public void copyFrom(AtomContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class ParenAtomContext extends AtomContext {
		public ParenExprContext parenExpr() {
			return getRuleContext(ParenExprContext.class,0);
		}
		public ParenAtomContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class DeRefAtomContext extends AtomContext {
		public DeRefExprContext deRefExpr() {
			return getRuleContext(DeRefExprContext.class,0);
		}
		public DeRefAtomContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class NullExprContext extends AtomContext {
		public TerminalNode KNULL() { return getToken(TIPParser.KNULL, 0); }
		public NullExprContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class AllocExprContext extends AtomContext {
		public TerminalNode KALLOC() { return getToken(TIPParser.KALLOC, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public AllocExprContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class RefExprContext extends AtomContext {
		public TerminalNode IDENTIFIER() { return getToken(TIPParser.IDENTIFIER, 0); }
		public RefExprContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class RecordAtomContext extends AtomContext {
		public RecordExprContext recordExpr() {
			return getRuleContext(RecordExprContext.class,0);
		}
		public AccessExprContext accessExpr() {
			return getRuleContext(AccessExprContext.class,0);
		}
		public RecordAtomContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class InputExprContext extends AtomContext {
		public TerminalNode KINPUT() { return getToken(TIPParser.KINPUT, 0); }
		public InputExprContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class FunAppAtomContext extends AtomContext {
		public FunAppExprContext funAppExpr() {
			return getRuleContext(FunAppExprContext.class,0);
		}
		public FunAppAtomContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class NumExprContext extends AtomContext {
		public TerminalNode NUMBER() { return getToken(TIPParser.NUMBER, 0); }
		public NumExprContext(AtomContext ctx) { copyFrom(ctx); }
	}
	public static class IdExprContext extends AtomContext {
		public TerminalNode IDENTIFIER() { return getToken(TIPParser.IDENTIFIER, 0); }
		public IdExprContext(AtomContext ctx) { copyFrom(ctx); }
	}

	public final AtomContext atom() throws RecognitionException {
		AtomContext _localctx = new AtomContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_atom);
		try {
			setState(111);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,8,_ctx) ) {
			case 1:
				_localctx = new ParenAtomContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(98);
				parenExpr();
				}
				break;
			case 2:
				_localctx = new FunAppAtomContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(99);
				funAppExpr();
				}
				break;
			case 3:
				_localctx = new NumExprContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(100);
				match(NUMBER);
				}
				break;
			case 4:
				_localctx = new IdExprContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(101);
				match(IDENTIFIER);
				}
				break;
			case 5:
				_localctx = new InputExprContext(_localctx);
				enterOuterAlt(_localctx, 5);
				{
				setState(102);
				match(KINPUT);
				}
				break;
			case 6:
				_localctx = new AllocExprContext(_localctx);
				enterOuterAlt(_localctx, 6);
				{
				setState(103);
				match(KALLOC);
				setState(104);
				expr(0);
				}
				break;
			case 7:
				_localctx = new RefExprContext(_localctx);
				enterOuterAlt(_localctx, 7);
				{
				setState(105);
				match(T__5);
				setState(106);
				match(IDENTIFIER);
				}
				break;
			case 8:
				_localctx = new DeRefAtomContext(_localctx);
				enterOuterAlt(_localctx, 8);
				{
				setState(107);
				deRefExpr();
				}
				break;
			case 9:
				_localctx = new NullExprContext(_localctx);
				enterOuterAlt(_localctx, 9);
				{
				setState(108);
				match(KNULL);
				}
				break;
			case 10:
				_localctx = new RecordAtomContext(_localctx);
				enterOuterAlt(_localctx, 10);
				{
				setState(109);
				recordExpr();
				}
				break;
			case 11:
				_localctx = new RecordAtomContext(_localctx);
				enterOuterAlt(_localctx, 11);
				{
				setState(110);
				accessExpr();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ParenExprContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ParenExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parenExpr; }
	}

	public final ParenExprContext parenExpr() throws RecognitionException {
		ParenExprContext _localctx = new ParenExprContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_parenExpr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(113);
			match(T__0);
			setState(114);
			expr(0);
			setState(115);
			match(T__2);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeRefExprContext extends ParserRuleContext {
		public TerminalNode MUL() { return getToken(TIPParser.MUL, 0); }
		public AtomContext atom() {
			return getRuleContext(AtomContext.class,0);
		}
		public DeRefExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_deRefExpr; }
	}

	public final DeRefExprContext deRefExpr() throws RecognitionException {
		DeRefExprContext _localctx = new DeRefExprContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_deRefExpr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(117);
			match(MUL);
			setState(118);
			atom();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunAppExprContext extends ParserRuleContext {
		public ParenExprContext parenExpr() {
			return getRuleContext(ParenExprContext.class,0);
		}
		public TerminalNode IDENTIFIER() { return getToken(TIPParser.IDENTIFIER, 0); }
		public List<ExprContext> expr() {
			return getRuleContexts(ExprContext.class);
		}
		public ExprContext expr(int i) {
			return getRuleContext(ExprContext.class,i);
		}
		public FunAppExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_funAppExpr; }
	}

	public final FunAppExprContext funAppExpr() throws RecognitionException {
		FunAppExprContext _localctx = new FunAppExprContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_funAppExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(122);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
				{
				setState(120);
				parenExpr();
				}
				break;
			case IDENTIFIER:
				{
				setState(121);
				match(IDENTIFIER);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(124);
			match(T__0);
			setState(133);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__0) | (1L << T__3) | (1L << T__5) | (1L << MUL) | (1L << SUB) | (1L << NUMBER) | (1L << KALLOC) | (1L << KINPUT) | (1L << KNULL) | (1L << IDENTIFIER))) != 0)) {
				{
				setState(125);
				expr(0);
				setState(130);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__1) {
					{
					{
					setState(126);
					match(T__1);
					setState(127);
					expr(0);
					}
					}
					setState(132);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(135);
			match(T__2);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class RecordExprContext extends ParserRuleContext {
		public List<FieldExprContext> fieldExpr() {
			return getRuleContexts(FieldExprContext.class);
		}
		public FieldExprContext fieldExpr(int i) {
			return getRuleContext(FieldExprContext.class,i);
		}
		public RecordExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_recordExpr; }
	}

	public final RecordExprContext recordExpr() throws RecognitionException {
		RecordExprContext _localctx = new RecordExprContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_recordExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(137);
			match(T__3);
			setState(146);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==IDENTIFIER) {
				{
				setState(138);
				fieldExpr();
				setState(143);
				_errHandler.sync(this);
				_la = _input.LA(1);
				while (_la==T__1) {
					{
					{
					setState(139);
					match(T__1);
					setState(140);
					fieldExpr();
					}
					}
					setState(145);
					_errHandler.sync(this);
					_la = _input.LA(1);
				}
				}
			}

			setState(148);
			match(T__4);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FieldExprContext extends ParserRuleContext {
		public TerminalNode IDENTIFIER() { return getToken(TIPParser.IDENTIFIER, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public FieldExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fieldExpr; }
	}

	public final FieldExprContext fieldExpr() throws RecognitionException {
		FieldExprContext _localctx = new FieldExprContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_fieldExpr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(150);
			match(IDENTIFIER);
			setState(151);
			match(T__6);
			setState(152);
			expr(0);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AccessExprContext extends ParserRuleContext {
		public List<TerminalNode> IDENTIFIER() { return getTokens(TIPParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(TIPParser.IDENTIFIER, i);
		}
		public DeRefExprContext deRefExpr() {
			return getRuleContext(DeRefExprContext.class,0);
		}
		public ParenExprContext parenExpr() {
			return getRuleContext(ParenExprContext.class,0);
		}
		public AccessExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_accessExpr; }
	}

	public final AccessExprContext accessExpr() throws RecognitionException {
		AccessExprContext _localctx = new AccessExprContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_accessExpr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(157);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(154);
				match(IDENTIFIER);
				}
				break;
			case MUL:
				{
				setState(155);
				deRefExpr();
				}
				break;
			case T__0:
				{
				setState(156);
				parenExpr();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			setState(159);
			match(T__7);
			setState(160);
			match(IDENTIFIER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AssignableExprContext extends ParserRuleContext {
		public TerminalNode IDENTIFIER() { return getToken(TIPParser.IDENTIFIER, 0); }
		public DeRefExprContext deRefExpr() {
			return getRuleContext(DeRefExprContext.class,0);
		}
		public AssignableExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assignableExpr; }
	}

	public final AssignableExprContext assignableExpr() throws RecognitionException {
		AssignableExprContext _localctx = new AssignableExprContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_assignableExpr);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(164);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case IDENTIFIER:
				{
				setState(162);
				match(IDENTIFIER);
				}
				break;
			case MUL:
				{
				setState(163);
				deRefExpr();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclarationContext extends ParserRuleContext {
		public TerminalNode KVAR() { return getToken(TIPParser.KVAR, 0); }
		public List<TerminalNode> IDENTIFIER() { return getTokens(TIPParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(TIPParser.IDENTIFIER, i);
		}
		public DeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declaration; }
	}

	public final DeclarationContext declaration() throws RecognitionException {
		DeclarationContext _localctx = new DeclarationContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_declaration);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(166);
			match(KVAR);
			setState(167);
			match(IDENTIFIER);
			setState(172);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__1) {
				{
				{
				setState(168);
				match(T__1);
				setState(169);
				match(IDENTIFIER);
				}
				}
				setState(174);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(175);
			match(T__8);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StatementContext extends ParserRuleContext {
		public BlockStmtContext blockStmt() {
			return getRuleContext(BlockStmtContext.class,0);
		}
		public AssignmentStmtContext assignmentStmt() {
			return getRuleContext(AssignmentStmtContext.class,0);
		}
		public WhileStmtContext whileStmt() {
			return getRuleContext(WhileStmtContext.class,0);
		}
		public IfStmtContext ifStmt() {
			return getRuleContext(IfStmtContext.class,0);
		}
		public OutputStmtContext outputStmt() {
			return getRuleContext(OutputStmtContext.class,0);
		}
		public ErrorStmtContext errorStmt() {
			return getRuleContext(ErrorStmtContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_statement);
		try {
			setState(183);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__3:
				enterOuterAlt(_localctx, 1);
				{
				setState(177);
				blockStmt();
				}
				break;
			case MUL:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 2);
				{
				setState(178);
				assignmentStmt();
				}
				break;
			case KWHILE:
				enterOuterAlt(_localctx, 3);
				{
				setState(179);
				whileStmt();
				}
				break;
			case KIF:
				enterOuterAlt(_localctx, 4);
				{
				setState(180);
				ifStmt();
				}
				break;
			case KOUTPUT:
				enterOuterAlt(_localctx, 5);
				{
				setState(181);
				outputStmt();
				}
				break;
			case KERROR:
				enterOuterAlt(_localctx, 6);
				{
				setState(182);
				errorStmt();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AssignmentStmtContext extends ParserRuleContext {
		public AssignableExprContext assignableExpr() {
			return getRuleContext(AssignableExprContext.class,0);
		}
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public AssignmentStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assignmentStmt; }
	}

	public final AssignmentStmtContext assignmentStmt() throws RecognitionException {
		AssignmentStmtContext _localctx = new AssignmentStmtContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_assignmentStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(185);
			assignableExpr();
			setState(186);
			match(T__9);
			setState(187);
			expr(0);
			setState(188);
			match(T__8);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockStmtContext extends ParserRuleContext {
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public BlockStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_blockStmt; }
	}

	public final BlockStmtContext blockStmt() throws RecognitionException {
		BlockStmtContext _localctx = new BlockStmtContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_blockStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(190);
			match(T__3);
			{
			setState(194);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__3) | (1L << MUL) | (1L << KWHILE) | (1L << KIF) | (1L << KOUTPUT) | (1L << KERROR) | (1L << IDENTIFIER))) != 0)) {
				{
				{
				setState(191);
				statement();
				}
				}
				setState(196);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
			setState(197);
			match(T__4);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class WhileStmtContext extends ParserRuleContext {
		public TerminalNode KWHILE() { return getToken(TIPParser.KWHILE, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public WhileStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whileStmt; }
	}

	public final WhileStmtContext whileStmt() throws RecognitionException {
		WhileStmtContext _localctx = new WhileStmtContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_whileStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(199);
			match(KWHILE);
			setState(200);
			match(T__0);
			setState(201);
			expr(0);
			setState(202);
			match(T__2);
			setState(203);
			statement();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IfStmtContext extends ParserRuleContext {
		public TerminalNode KIF() { return getToken(TIPParser.KIF, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public TerminalNode KELSE() { return getToken(TIPParser.KELSE, 0); }
		public IfStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ifStmt; }
	}

	public final IfStmtContext ifStmt() throws RecognitionException {
		IfStmtContext _localctx = new IfStmtContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_ifStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(205);
			match(KIF);
			setState(206);
			match(T__0);
			setState(207);
			expr(0);
			setState(208);
			match(T__2);
			setState(209);
			statement();
			setState(212);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				{
				setState(210);
				match(KELSE);
				setState(211);
				statement();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OutputStmtContext extends ParserRuleContext {
		public TerminalNode KOUTPUT() { return getToken(TIPParser.KOUTPUT, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public OutputStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_outputStmt; }
	}

	public final OutputStmtContext outputStmt() throws RecognitionException {
		OutputStmtContext _localctx = new OutputStmtContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_outputStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(214);
			match(KOUTPUT);
			setState(215);
			expr(0);
			setState(216);
			match(T__8);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ErrorStmtContext extends ParserRuleContext {
		public TerminalNode KERROR() { return getToken(TIPParser.KERROR, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ErrorStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_errorStmt; }
	}

	public final ErrorStmtContext errorStmt() throws RecognitionException {
		ErrorStmtContext _localctx = new ErrorStmtContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_errorStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(218);
			match(KERROR);
			setState(219);
			expr(0);
			setState(220);
			match(T__8);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReturnStmtContext extends ParserRuleContext {
		public TerminalNode KRETURN() { return getToken(TIPParser.KRETURN, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public ReturnStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnStmt; }
	}

	public final ReturnStmtContext returnStmt() throws RecognitionException {
		ReturnStmtContext _localctx = new ReturnStmtContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_returnStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(222);
			match(KRETURN);
			setState(223);
			expr(0);
			setState(224);
			match(T__8);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 2:
			return expr_sempred((ExprContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean expr_sempred(ExprContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 5);
		case 1:
			return precpred(_ctx, 4);
		case 2:
			return precpred(_ctx, 3);
		case 3:
			return precpred(_ctx, 2);
		}
		return true;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\"\u00e5\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\3\2\7\2,\n\2\f\2\16\2/\13\2\3\3\3\3\3\3"+
		"\3\3\3\3\7\3\66\n\3\f\3\16\39\13\3\5\3;\n\3\3\3\3\3\3\3\7\3@\n\3\f\3\16"+
		"\3C\13\3\3\3\7\3F\n\3\f\3\16\3I\13\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4\5\4R"+
		"\n\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\7\4`\n\4\f\4\16\4"+
		"c\13\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\5\5r\n\5\3"+
		"\6\3\6\3\6\3\6\3\7\3\7\3\7\3\b\3\b\5\b}\n\b\3\b\3\b\3\b\3\b\7\b\u0083"+
		"\n\b\f\b\16\b\u0086\13\b\5\b\u0088\n\b\3\b\3\b\3\t\3\t\3\t\3\t\7\t\u0090"+
		"\n\t\f\t\16\t\u0093\13\t\5\t\u0095\n\t\3\t\3\t\3\n\3\n\3\n\3\n\3\13\3"+
		"\13\3\13\5\13\u00a0\n\13\3\13\3\13\3\13\3\f\3\f\5\f\u00a7\n\f\3\r\3\r"+
		"\3\r\3\r\7\r\u00ad\n\r\f\r\16\r\u00b0\13\r\3\r\3\r\3\16\3\16\3\16\3\16"+
		"\3\16\3\16\5\16\u00ba\n\16\3\17\3\17\3\17\3\17\3\17\3\20\3\20\7\20\u00c3"+
		"\n\20\f\20\16\20\u00c6\13\20\3\20\3\20\3\21\3\21\3\21\3\21\3\21\3\21\3"+
		"\22\3\22\3\22\3\22\3\22\3\22\3\22\5\22\u00d7\n\22\3\23\3\23\3\23\3\23"+
		"\3\24\3\24\3\24\3\24\3\25\3\25\3\25\3\25\3\25\2\3\6\26\2\4\6\b\n\f\16"+
		"\20\22\24\26\30\32\34\36 \"$&(\2\4\3\2\r\16\3\2\17\20\2\u00f4\2-\3\2\2"+
		"\2\4\60\3\2\2\2\6Q\3\2\2\2\bq\3\2\2\2\ns\3\2\2\2\fw\3\2\2\2\16|\3\2\2"+
		"\2\20\u008b\3\2\2\2\22\u0098\3\2\2\2\24\u009f\3\2\2\2\26\u00a6\3\2\2\2"+
		"\30\u00a8\3\2\2\2\32\u00b9\3\2\2\2\34\u00bb\3\2\2\2\36\u00c0\3\2\2\2 "+
		"\u00c9\3\2\2\2\"\u00cf\3\2\2\2$\u00d8\3\2\2\2&\u00dc\3\2\2\2(\u00e0\3"+
		"\2\2\2*,\5\4\3\2+*\3\2\2\2,/\3\2\2\2-+\3\2\2\2-.\3\2\2\2.\3\3\2\2\2/-"+
		"\3\2\2\2\60\61\7\37\2\2\61:\7\3\2\2\62\67\7\37\2\2\63\64\7\4\2\2\64\66"+
		"\7\37\2\2\65\63\3\2\2\2\669\3\2\2\2\67\65\3\2\2\2\678\3\2\2\28;\3\2\2"+
		"\29\67\3\2\2\2:\62\3\2\2\2:;\3\2\2\2;<\3\2\2\2<=\7\5\2\2=A\7\6\2\2>@\5"+
		"\30\r\2?>\3\2\2\2@C\3\2\2\2A?\3\2\2\2AB\3\2\2\2BG\3\2\2\2CA\3\2\2\2DF"+
		"\5\32\16\2ED\3\2\2\2FI\3\2\2\2GE\3\2\2\2GH\3\2\2\2HJ\3\2\2\2IG\3\2\2\2"+
		"JK\5(\25\2KL\7\7\2\2L\5\3\2\2\2MN\b\4\1\2NO\7\20\2\2OR\7\24\2\2PR\5\b"+
		"\5\2QM\3\2\2\2QP\3\2\2\2Ra\3\2\2\2ST\f\7\2\2TU\t\2\2\2U`\5\6\4\bVW\f\6"+
		"\2\2WX\t\3\2\2X`\5\6\4\7YZ\f\5\2\2Z[\7\21\2\2[`\5\6\4\6\\]\f\4\2\2]^\7"+
		"\22\2\2^`\5\6\4\5_S\3\2\2\2_V\3\2\2\2_Y\3\2\2\2_\\\3\2\2\2`c\3\2\2\2a"+
		"_\3\2\2\2ab\3\2\2\2b\7\3\2\2\2ca\3\2\2\2dr\5\n\6\2er\5\16\b\2fr\7\24\2"+
		"\2gr\7\37\2\2hr\7\26\2\2ij\7\25\2\2jr\5\6\4\2kl\7\b\2\2lr\7\37\2\2mr\5"+
		"\f\7\2nr\7\34\2\2or\5\20\t\2pr\5\24\13\2qd\3\2\2\2qe\3\2\2\2qf\3\2\2\2"+
		"qg\3\2\2\2qh\3\2\2\2qi\3\2\2\2qk\3\2\2\2qm\3\2\2\2qn\3\2\2\2qo\3\2\2\2"+
		"qp\3\2\2\2r\t\3\2\2\2st\7\3\2\2tu\5\6\4\2uv\7\5\2\2v\13\3\2\2\2wx\7\r"+
		"\2\2xy\5\b\5\2y\r\3\2\2\2z}\5\n\6\2{}\7\37\2\2|z\3\2\2\2|{\3\2\2\2}~\3"+
		"\2\2\2~\u0087\7\3\2\2\177\u0084\5\6\4\2\u0080\u0081\7\4\2\2\u0081\u0083"+
		"\5\6\4\2\u0082\u0080\3\2\2\2\u0083\u0086\3\2\2\2\u0084\u0082\3\2\2\2\u0084"+
		"\u0085\3\2\2\2\u0085\u0088\3\2\2\2\u0086\u0084\3\2\2\2\u0087\177\3\2\2"+
		"\2\u0087\u0088\3\2\2\2\u0088\u0089\3\2\2\2\u0089\u008a\7\5\2\2\u008a\17"+
		"\3\2\2\2\u008b\u0094\7\6\2\2\u008c\u0091\5\22\n\2\u008d\u008e\7\4\2\2"+
		"\u008e\u0090\5\22\n\2\u008f\u008d\3\2\2\2\u0090\u0093\3\2\2\2\u0091\u008f"+
		"\3\2\2\2\u0091\u0092\3\2\2\2\u0092\u0095\3\2\2\2\u0093\u0091\3\2\2\2\u0094"+
		"\u008c\3\2\2\2\u0094\u0095\3\2\2\2\u0095\u0096\3\2\2\2\u0096\u0097\7\7"+
		"\2\2\u0097\21\3\2\2\2\u0098\u0099\7\37\2\2\u0099\u009a\7\t\2\2\u009a\u009b"+
		"\5\6\4\2\u009b\23\3\2\2\2\u009c\u00a0\7\37\2\2\u009d\u00a0\5\f\7\2\u009e"+
		"\u00a0\5\n\6\2\u009f\u009c\3\2\2\2\u009f\u009d\3\2\2\2\u009f\u009e\3\2"+
		"\2\2\u00a0\u00a1\3\2\2\2\u00a1\u00a2\7\n\2\2\u00a2\u00a3\7\37\2\2\u00a3"+
		"\25\3\2\2\2\u00a4\u00a7\7\37\2\2\u00a5\u00a7\5\f\7\2\u00a6\u00a4\3\2\2"+
		"\2\u00a6\u00a5\3\2\2\2\u00a7\27\3\2\2\2\u00a8\u00a9\7\32\2\2\u00a9\u00ae"+
		"\7\37\2\2\u00aa\u00ab\7\4\2\2\u00ab\u00ad\7\37\2\2\u00ac\u00aa\3\2\2\2"+
		"\u00ad\u00b0\3\2\2\2\u00ae\u00ac\3\2\2\2\u00ae\u00af\3\2\2\2\u00af\u00b1"+
		"\3\2\2\2\u00b0\u00ae\3\2\2\2\u00b1\u00b2\7\13\2\2\u00b2\31\3\2\2\2\u00b3"+
		"\u00ba\5\36\20\2\u00b4\u00ba\5\34\17\2\u00b5\u00ba\5 \21\2\u00b6\u00ba"+
		"\5\"\22\2\u00b7\u00ba\5$\23\2\u00b8\u00ba\5&\24\2\u00b9\u00b3\3\2\2\2"+
		"\u00b9\u00b4\3\2\2\2\u00b9\u00b5\3\2\2\2\u00b9\u00b6\3\2\2\2\u00b9\u00b7"+
		"\3\2\2\2\u00b9\u00b8\3\2\2\2\u00ba\33\3\2\2\2\u00bb\u00bc\5\26\f\2\u00bc"+
		"\u00bd\7\f\2\2\u00bd\u00be\5\6\4\2\u00be\u00bf\7\13\2\2\u00bf\35\3\2\2"+
		"\2\u00c0\u00c4\7\6\2\2\u00c1\u00c3\5\32\16\2\u00c2\u00c1\3\2\2\2\u00c3"+
		"\u00c6\3\2\2\2\u00c4\u00c2\3\2\2\2\u00c4\u00c5\3\2\2\2\u00c5\u00c7\3\2"+
		"\2\2\u00c6\u00c4\3\2\2\2\u00c7\u00c8\7\7\2\2\u00c8\37\3\2\2\2\u00c9\u00ca"+
		"\7\27\2\2\u00ca\u00cb\7\3\2\2\u00cb\u00cc\5\6\4\2\u00cc\u00cd\7\5\2\2"+
		"\u00cd\u00ce\5\32\16\2\u00ce!\3\2\2\2\u00cf\u00d0\7\30\2\2\u00d0\u00d1"+
		"\7\3\2\2\u00d1\u00d2\5\6\4\2\u00d2\u00d3\7\5\2\2\u00d3\u00d6\5\32\16\2"+
		"\u00d4\u00d5\7\31\2\2\u00d5\u00d7\5\32\16\2\u00d6\u00d4\3\2\2\2\u00d6"+
		"\u00d7\3\2\2\2\u00d7#\3\2\2\2\u00d8\u00d9\7\35\2\2\u00d9\u00da\5\6\4\2"+
		"\u00da\u00db\7\13\2\2\u00db%\3\2\2\2\u00dc\u00dd\7\36\2\2\u00dd\u00de"+
		"\5\6\4\2\u00de\u00df\7\13\2\2\u00df\'\3\2\2\2\u00e0\u00e1\7\33\2\2\u00e1"+
		"\u00e2\5\6\4\2\u00e2\u00e3\7\13\2\2\u00e3)\3\2\2\2\26-\67:AGQ_aq|\u0084"+
		"\u0087\u0091\u0094\u009f\u00a6\u00ae\u00b9\u00c4\u00d6";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}