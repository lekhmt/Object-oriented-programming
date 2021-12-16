

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

void Parser::Interface() {
		S();
		while (StartOf(1)) {
			S();
		}
}

void Parser::S() {
		switch (la->kind) {
		case 2 /* "push" */: {
			Get();
			std::vector<std::pair<int, int>> data(4); 
			SetTrapezoid(data);
			auto t = Trapezoid<int>(data[0], data[1], data[2], data[3]);
			s.push(t); 
			break;
		}
		case 3 /* "print" */: {
			Get();
			std::cout << "> print\n"; s.print(); 
			break;
		}
		case 4 /* "get" */: {
			Get();
			Expect(_Num);
			int i =  wcstod(t->val, NULL);
			std::cout << "> get " << i << "\n"; s.print(i - 1); 
			break;
		}
		case 5 /* "top" */: {
			Get();
			std::cout << "> top\n"; std::shared_ptr<Trapezoid<int>> t = s.top();
			if (t){ std::cout << *t << "\n"; } 
			break;
		}
		case 6 /* "pop" */: {
			Get();
			std::cout << "> pop\n"; std::shared_ptr<Trapezoid<int>> t = s.pop();
			if (t){ std::cout << *t << "\n"; } 
			break;
		}
		case 7 /* "insert" */: {
			Get();
			std::vector<std::pair<int, int>> data(4); 
			SetTrapezoid(data);
			auto tmp = Trapezoid<int>(data[0], data[1], data[2], data[3]); 
			Expect(_Num);
			int i = wcstod(t->val, NULL); i -= 2;
			std::cout << "> insert to " << i + 2 << "\n";
			s.insert(tmp, i); 
			break;
		}
		case 8 /* "erase" */: {
			Get();
			Expect(_Num);
			int i = wcstod(t->val, NULL); i -= 1;
			std::cout << "> erase from " << i + 1 << "\n";
			s.erase(i); 
			break;
		}
		case 9 /* "count" */: {
			Get();
			Expect(_Num);
			double i = wcstod(t->val, NULL);
			std::cout << "> count if area < " << i << "\n" << s.count(i) << "\n"; 
			break;
		}
		default: SynErr(15); break;
		}
}

void Parser::SetTrapezoid(std::vector<std::pair<int, int>>& data ) {
		int x, y; 
		Expect(10 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(11 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[0] = {x, y}; 
		Expect(12 /* ")," */);
		Expect(10 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(11 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[1] = {x, y}; 
		Expect(12 /* ")," */);
		Expect(10 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(11 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[2] = {x, y}; 
		Expect(12 /* ")," */);
		Expect(10 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(11 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[3] = {x, y}; 
		Expect(13 /* ")" */);
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
	Interface();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 14;

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

	static bool set[2][16] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,T,T, T,T,T,T, T,T,x,x, x,x,x,x}
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
			case 1: s = coco_string_create(L"Num expected"); break;
			case 2: s = coco_string_create(L"\"push\" expected"); break;
			case 3: s = coco_string_create(L"\"print\" expected"); break;
			case 4: s = coco_string_create(L"\"get\" expected"); break;
			case 5: s = coco_string_create(L"\"top\" expected"); break;
			case 6: s = coco_string_create(L"\"pop\" expected"); break;
			case 7: s = coco_string_create(L"\"insert\" expected"); break;
			case 8: s = coco_string_create(L"\"erase\" expected"); break;
			case 9: s = coco_string_create(L"\"count\" expected"); break;
			case 10: s = coco_string_create(L"\"(\" expected"); break;
			case 11: s = coco_string_create(L"\",\" expected"); break;
			case 12: s = coco_string_create(L"\"),\" expected"); break;
			case 13: s = coco_string_create(L"\")\" expected"); break;
			case 14: s = coco_string_create(L"??? expected"); break;
			case 15: s = coco_string_create(L"invalid S"); break;

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


