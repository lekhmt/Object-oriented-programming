

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
		case 2 /* "add" */: {
			Get();
			std::vector<point_t> data(3); 
			SetTriangle(data);
			Triangle t = Triangle(data[0], data[1], data[2]);
			vector.append(t); 
			break;
		}
		case 3 /* "set" */: {
			Get();
			Expect(_Num);
			int i = std::stoi(t->val); --i;
			std::vector<point_t> data(3); 
			SetTriangle(data);
			Triangle t = Triangle(data[0], data[1], data[2]);
			vector.set(t, i); 
			break;
		}
		case 4 /* "get" */: {
			Get();
			Expect(_Num);
			int i = std::stoi(t->val); --i;
			std::wcout << "> get " << i + 1 << "\n";
			Triangle t = vector.get(i);
			if (!t.is_removed) { std::wcout << i + 1 << ": " << t; }; 
			break;
		}
		case 5 /* "print" */: {
			Get();
			std::wcout << "> print\n" << vector; 
			break;
		}
		case 6 /* "remove" */: {
			Get();
			Expect(_Num);
			int i = std::stoi(t->val);
			vector.remove(i - 1); 
			break;
		}
		case _Num: {
			Get();
			int n1 = std::stoi(t->val); --n1; 
			while (la->kind == 7 /* "=" */ || la->kind == 8 /* "==" */) {
				if (la->kind == 7 /* "=" */) {
					Get();
					Expect(_Num);
					int n2 = std::stoi(t->val); --n2;
					std::wcout << "> " << n1 + 1 << " = " << n2 + 1 << "\n";
					Triangle t2 = vector.get(n2);
					if (!t2.is_removed) { vector.set(t2, n1); }; 
				} else {
					Get();
					Expect(_Num);
					int n2 = std::stoi(t->val); --n2;
					std::wcout << "> " << n1 + 1 << " == " << n2 + 1 << "\n";
					Triangle t1 = vector.get(n1);
					Triangle t2 = vector.get(n2);
					if (!t1.is_removed && !t2.is_removed) {
					 if (t1 == t2){ std::wcout << "true\n"; }
					 else { std::wcout << "false\n"; }
					} 
				}
			}
			break;
		}
		default: SynErr(14); break;
		}
}

void Parser::SetTriangle(std::vector<point_t>& data ) {
		double x, y; 
		Expect(9 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(10 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[0] = {x, y}; 
		Expect(11 /* ")," */);
		Expect(9 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(10 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[1] = {x, y}; 
		Expect(11 /* ")," */);
		Expect(9 /* "(" */);
		Expect(_Num);
		x = wcstod(t->val, NULL); 
		Expect(10 /* "," */);
		Expect(_Num);
		y = wcstod(t->val, NULL);
		data[2] = {x, y}; 
		Expect(12 /* ")" */);
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
	maxT = 13;

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

	static bool set[2][15] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,T, T,T,T,x, x,x,x,x, x,x,x}
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
			case 2: s = coco_string_create(L"\"add\" expected"); break;
			case 3: s = coco_string_create(L"\"set\" expected"); break;
			case 4: s = coco_string_create(L"\"get\" expected"); break;
			case 5: s = coco_string_create(L"\"print\" expected"); break;
			case 6: s = coco_string_create(L"\"remove\" expected"); break;
			case 7: s = coco_string_create(L"\"=\" expected"); break;
			case 8: s = coco_string_create(L"\"==\" expected"); break;
			case 9: s = coco_string_create(L"\"(\" expected"); break;
			case 10: s = coco_string_create(L"\",\" expected"); break;
			case 11: s = coco_string_create(L"\"),\" expected"); break;
			case 12: s = coco_string_create(L"\")\" expected"); break;
			case 13: s = coco_string_create(L"??? expected"); break;
			case 14: s = coco_string_create(L"invalid S"); break;

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


