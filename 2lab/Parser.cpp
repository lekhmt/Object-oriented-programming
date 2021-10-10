

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::calc() {
		S();
		while (StartOf(1)) {
			S();
		}
}

void Parser::S() {
		Vector v; 
		switch (la->kind) {
		case _Var: {
			Get();
			std::wstring name = t->val; 
			if (la->kind == 3 /* "=" */) {
				Get();
				Expr(v);
			}
			memory.save(name, v); 
			break;
		}
		case 4 /* "print" */: {
			Get();
			Expect(_Var);
			std::wcout << t->val << ": " << memory.get(t->val); 
			break;
		}
		case 5 /* "length" */: {
			Get();
			Expect(_Var);
			std::wcout << "|" << t->val << "| = " << memory.get(t->val).length() << "\n"; 
			break;
		}
		case 6 /* "angle" */: {
			Get();
			Expect(_Var);
			Vector v1 = memory.get(t->val);
			std::wstring name1 = t->val;
			Expect(_Var);
			Vector v2 = memory.get(t->val);
			std::wstring name2 = t->val;
			double ang = angle(v1, v2);
			std::wcout << name1 << "^" << name2 << " = " << ang << "\n"; 
			break;
		}
		case 7 /* "cos" */: {
			Get();
			Expect(_Var);
			Vector v1 = memory.get(t->val);
			std::wstring name1 = t->val;
			Expect(_Var);
			Vector v2 = memory.get(t->val);
			std::wstring name2 = t->val;
			double ang = cos_angle(v1, v2);
			std::wcout << "cos " << name1 << "^" << name2 << " = " << ang << "\n"; 
			break;
		}
		case 8 /* "is" */: {
			Get();
			while (la->kind == _Var || la->kind == 10 /* "|" */) {
				if (la->kind == _Var) {
					Get();
					Vector v1 = memory.get(t->val); std::wstring name1 = t->val; 
					Expect(9 /* "==" */);
					Expect(_Var);
					Vector v2 = memory.get(t->val); std::wstring name2 = t->val;
					std::wcout << name1 << " = " << name2 << " is ";
					if (v1.is_match(v2)) { std::wcout << "true\n"; }
					else { std::wcout << "false\n"; } 
				} else {
					Get();
					Expect(_Var);
					Vector v1 = memory.get(t->val); std::wstring name1 = t->val; 
					Expect(10 /* "|" */);
					Expect(9 /* "==" */);
					Expect(10 /* "|" */);
					Expect(_Var);
					Vector v2 = memory.get(t->val); std::wstring name2 = t->val;
					std::wcout << "|" << name1 << "| == |" << name2 << "| is ";
					if (v1.is_equal(v2)) { std::wcout << "true\n"; }
					else { std::wcout << "false\n"; } 
					Expect(10 /* "|" */);
				}
			}
			break;
		}
		default: SynErr(21); break;
		}
}

void Parser::Expr(Vector& v ) {
		Vector v1, v2;
		IntExpr(v1);
		v = v1; 
		while (la->kind == 11 /* "+" */ || la->kind == 12 /* "-" */) {
			if (la->kind == 11 /* "+" */) {
				Get();
				IntExpr(v2);
				v = v + v2; 
			} else {
				Get();
				IntExpr(v2);
				v = v - v2; 
			}
		}
}

void Parser::IntExpr(Vector& v ) {
		Vector v1, v2; 
		if (la->kind == _Var || la->kind == 15 /* "(" */ || la->kind == 17 /* "{" */) {
			UniExpr(v1);
			v = v1; 
			while (la->kind == 13 /* "*" */ || la->kind == 14 /* "&" */) {
				if (la->kind == 13 /* "*" */) {
					Get();
					Expect(_Num);
					v = v * wcstod(t->val, NULL); 
				} else {
					Get();
					UniExpr(v2);
					v = (v & v2); 
				}
			}
		} else if (la->kind == _Num) {
			Get();
			double num = wcstod(t->val, NULL); 
			Expect(13 /* "*" */);
			UniExpr(v2);
			v = num * v2; 
		} else SynErr(22);
}

void Parser::UniExpr(Vector& v ) {
		Vector v1;
		if (la->kind == 17 /* "{" */) {
			Set(v1);
			v = v1; 
		} else if (la->kind == _Var) {
			Get();
			v = memory.get(t->val); 
		} else if (la->kind == 15 /* "(" */) {
			Get();
			Expr(v);
			Expect(16 /* ")" */);
		} else SynErr(23);
}

void Parser::Set(Vector& v ) {
		Expect(17 /* "{" */);
		Expect(_Num);
		double x = wcstod(t->val, NULL);   
		Expect(18 /* "," */);
		Expect(_Num);
		double y = wcstod(t->val, NULL);   
		Expect(18 /* "," */);
		Expect(_Num);
		double z = wcstod(t->val, NULL);   
		Expect(19 /* "}" */);
		v = Vector(x, y, z); 
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	calc();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 20;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[2][22] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x},
		{x,T,x,x, T,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"Var expected"); break;
			case 2: s = coco_string_create(L"Num expected"); break;
			case 3: s = coco_string_create(L"\"=\" expected"); break;
			case 4: s = coco_string_create(L"\"print\" expected"); break;
			case 5: s = coco_string_create(L"\"length\" expected"); break;
			case 6: s = coco_string_create(L"\"angle\" expected"); break;
			case 7: s = coco_string_create(L"\"cos\" expected"); break;
			case 8: s = coco_string_create(L"\"is\" expected"); break;
			case 9: s = coco_string_create(L"\"==\" expected"); break;
			case 10: s = coco_string_create(L"\"|\" expected"); break;
			case 11: s = coco_string_create(L"\"+\" expected"); break;
			case 12: s = coco_string_create(L"\"-\" expected"); break;
			case 13: s = coco_string_create(L"\"*\" expected"); break;
			case 14: s = coco_string_create(L"\"&\" expected"); break;
			case 15: s = coco_string_create(L"\"(\" expected"); break;
			case 16: s = coco_string_create(L"\")\" expected"); break;
			case 17: s = coco_string_create(L"\"{\" expected"); break;
			case 18: s = coco_string_create(L"\",\" expected"); break;
			case 19: s = coco_string_create(L"\"}\" expected"); break;
			case 20: s = coco_string_create(L"??? expected"); break;
			case 21: s = coco_string_create(L"invalid S"); break;
			case 22: s = coco_string_create(L"invalid IntExpr"); break;
			case 23: s = coco_string_create(L"invalid UniExpr"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


