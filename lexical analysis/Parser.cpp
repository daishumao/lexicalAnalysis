#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <math.h>
using namespace std;
using std::vector;

#define TAG_ADD 256				// +
#define TAG_SUB 257				// -
#define TAG_MUL 258				// *
#define TAG_DIV 259				// /
#define TAG_LESS 260			// <
#define TAG_LE 261				// <=
#define TAG_GREATER 262			// >
#define TAG_GE 263				// >=
#define TAG_EQUAL 264			// ==
#define TAG_NE 265				// !=
#define TAG_SEMICN 266			// =
#define TAG_LPARENT 267			// (
#define TAG_RPARENT 268			// )
#define TAG_LBRACE 269			// {
#define TAG_RBRACE 270			// }
#define TAG_SEMICOLON 271		// ;
#define TAG_QUOTE 272			// "
#define TAG_COLON 273			// :
#define TAG_COMMA 274			// ,
#define TAG_SINGLEQUOTE 275		// '
#define TAG_VAR 277
#define TAG_ARRAY 278
#define TAG_OF 279
#define TAG_INT	280
#define TAG_REAL 281
#define TAG_CHAR 282
#define TAG_ID 283
#define TAG_CONST 284
#define TAG_TYPEINT 285
#define TAG_TYPEREAL 286
#define TAG_TYPECHAR 287
#define TAG_IF 288
#define TAG_THEN 289
#define TAG_ELSE 290
#define TAG_DO 291
#define TAG_WHILE 292
#define TAG_FOR 293
#define TAG_TO 294
#define TAG_BY 295
#define TAG_PROCEDURE 296
#define TAG_FUNC 297
#define TAG_READ 298
#define TAG_WRITE 299
#define TAG_BEGIN 300
#define TAG_END 301
#define TAG_LINEEND 302
#define TAG_FILEEND 303
#define TAG_IDEN 304
#define TAG_PERIOD 305
#define TAG_LBRACKET 306			// ]
#define TAG_RBRACKET 307			// [
#define TAG_NEQ 308			// [
#define TAG_ASSIGN 309			// :=
#define TAG_TYPESTRING 310

enum types{inttype,realtype,chartype,arraytype,booltype,notype,stringtype};
enum kinds{procedures,functions,constants,variables};
enum operates {LIT,OPR,LDA,WRT,LOD,RED,INT,JMP,JPC,SWT,CAS,MKS,CAL,STO,IDX};

int typeNumber = 23;
ifstream input;
ofstream output;
string w = "";
char wc;
double number = 0;
int isEnd = 0;
double realValue;
string id;
types type;
int level = 1;
string symName[]={"const","integer","real","char","var","array","of",
	"if","then","else","do","while","for","to","by","procedure",
	"function","read","write","begin","end"};
int categoryName[]={TAG_CONST,TAG_INT,TAG_REAL,TAG_CHAR,TAG_VAR,TAG_ARRAY,TAG_OF,
	TAG_IF,TAG_THEN,TAG_ELSE,TAG_DO,TAG_WHILE,TAG_FOR,TAG_TO,TAG_BY,TAG_PROCEDURE,
	TAG_FUNC,TAG_READ,TAG_WRITE,TAG_BEGIN,TAG_END};

char ch = ' ';
string pLine;
string filename;
int sym;
int lineno;
int ints;
double reals;
int lineNo = 1;
char chars;
string strings;
int size;
int position;
bool sub = false;
bool add = false;

struct values{
	double realValue;
	int intValue;
	char charValue;
	string stringValue;
};
class symbolItem{
	public:
		string name;
		kinds kind;
		types type;
		int ref;
		int level;
		int link;
		int address;
		values value;
};
class btabItem{
public:
	int last;
	int lastpar;
	int psize;
	int vsize;
};
class atabItem{
public:
	types eltype;
	int high;
	int size;
	int elsize;
};
class stackItem{
	public:
		double value;
		char chars;
		string strings;
		types type = inttype;
};
class pcode{
	public:
		operates operate;
		int p1;
		double p2;
		types type;
};
vector<pcode> pcodeList(5000);
vector<symbolItem> symbolTable(5000);
vector<atabItem> atab(500);
vector<btabItem> btab(500);
vector<stackItem> stack(5000);
vector<int> display(100);
int sti = 0;
int pli = 0;
int ati = 0;
int bti = 1;
int address;
int tsti = 0;
int taddress = 0;
int baseR = 0;
void getch();
void error(int no);
int isdigit(char c);
int isalpha(char c);
void getSym();
void call(int n,int l,int a);
string idUp(string id);
void call(int position);
void Factor();
void Expression();
void Statement();
int FindId(string name);
void Enter(kinds kind);
void IfStatement();
void ForStatement();
void WhileStatement();
void WriteStatement();
void ReadStatement();
void AssignStatement();
void ProcedureStatement();
void FormalParamList();
void ProcedureHead();
void Procedure();
void FunctionHead();
void Function();
void ConstDefine();
void VarDefine();
void ConstDeclar();
void VarDeclar();
void CompoundStatement();
void Block();
void Program();
int base();

void test(){
	for(int i=0;i<=pli;i++){
		cout<<i<<","<<pcodeList[i].operate<<","<<","<<pcodeList[i].p1<<","<<pcodeList[i].p2<<endl;
	}
}
void getch(){
	if (input.peek() != EOF){
		ch = input.get();
	}else{
		printf("program incomlete");
	}
}
void error(int no){
	input.close();
	output.close();
}
int isdigit(char c)
{
	if(c >= '0' && c<= '9')
		return true;
	else
		return false;
}

int isalpha(char c)
{
	if(( c >= 'A' && c <= 'Z' ) || ( c>= 'a' && c<= 'z' ))
		return true;
	else
		return false;
}
string idUp(string id){
	for(int i = 0;i< id.size();i++){
		if(id[i] >= 'a' && id[i]<= 'z'){
			id[i] += ('A' - 'a');
		}
	}
	return id;
}
void getSym(){
	w="";
	int i;
	getch();
	while( ch == ' ' || ch == '\n'){
		if(ch == '\n')
			lineNo++;
		getch();
	}
	if (isalpha(ch)){
		w += ch;
		getch();
		while(isdigit(ch) || isalpha(ch)){
			w += ch;
			getch();
		}
		input.seekg(-1,ios::cur);
		for(i = 0; i < typeNumber; i++){
			if(w == symName[i]){
				break;
			}
		}
		if (i < typeNumber){
			sym = categoryName[i];
		} else {
			sym = TAG_ID;
			id = idUp(w);
		}

	} else if(isdigit(ch)){
		number = 0;
		while(isdigit(ch)){
			number = number * 10 + int(ch) - int('0');
			getch();
		}
		if(ch == '.'){
			int j = 1;
			while(isdigit(ch)){
				number = number + (int(ch) - int('0')) * pow(0.1,j);
				getch();
				j++;
			}
			reals = number;
			sym = TAG_TYPEREAL;
			input.seekg(-1,ios::cur);
		}else{
			ints = number;
			sym = TAG_TYPEINT;
			input.seekg(-1,ios::cur);
		}
	} else if(ch == '+'){
		sym = TAG_ADD;
	} else if(ch == '-'){
		sym = TAG_SUB;
	} else if(ch == '*'){
		sym = TAG_MUL;
	} else if(ch == '/'){
		sym = TAG_DIV;
	} else if(ch == '.'){
		sym = TAG_PERIOD;
		isEnd = 1;
		return;
	} else if(ch == '('){
		sym = TAG_LPARENT;
	} else if(ch == ')') {
		sym = TAG_RPARENT;
	} else if(ch == '[') {
		sym = TAG_LBRACKET;
	} else if(ch == ']') {
		sym = TAG_RBRACKET;
	} else if(ch == '{') {
		sym = TAG_LBRACE;
	} else if(ch == '}') {
		sym = TAG_RBRACE;
	} else if(ch == ';') {
		sym = TAG_SEMICOLON;
	} else if(ch == ',') {
		sym = TAG_COMMA;
	} else if(ch == '=') {
		sym = TAG_ASSIGN;
	} else if(ch == '<') {
		getch();
		if(ch == '='){
			sym = TAG_LE;
		} else if( ch == '>') {
			sym = TAG_NEQ;
		} else {
			sym = TAG_LESS;
			input.seekg(-1,ios::cur);
		}
	} else if(ch == '>') {
		getch();
		if(ch == '='){
			sym = TAG_GE;
		} else {
			sym = TAG_GREATER;
			input.seekg(-1,ios::cur);
		}
	} else if(ch == ':') {
		getch();
		if(ch == '='){
			sym = TAG_ASSIGN;
		} else {
			sym = TAG_SEMICN;
			input.seekg(-1,ios::cur);
		}
	} else if(ch == '\''){
		getch();
		if(!isdigit(ch) && !isalpha(ch)){
			error(1);
		}
		wc = ch;
		getch();
		if(ch != '\''){
			error(2);
		}
		chars = wc;
		sym = TAG_TYPECHAR;
	} else if(ch == '"'){
		getch();
		while(int(ch) == 32 || int(ch) == 33 || (int(ch) >= 35 && int(ch) <= 126)){
			if(ch != '"'){
				w += ch;
				getch();
			} else{
				break;
			}
		}
		if (ch != '"'){
			error(3);
		}
		strings = w;
		sym = TAG_TYPESTRING;
	}
}
void call(int n,int l,int a){

}
void gen(operates operate, int p1, int p2){
	pcodeList[pli].operate = operate;
	pcodeList[pli].p1 = p1;
	pcodeList[pli].p2 = p2;
	pcodeList[pli].type = inttype;
	pli++;
}
void gen(operates operate, int p1, double p2){
	pcodeList[pli].operate = operate;
	pcodeList[pli].p1 = p1;
	pcodeList[pli].p2 = p2;
	pcodeList[pli].type = realtype;
	pli++;
}
void Term(){
	Factor();
	int op;
	while(sym == TAG_MUL || sym == TAG_DIV){
		op = sym;
		getSym();
		Factor();
		if(op == TAG_MUL){
			gen(OPR,0,4);
		}else{
			gen(OPR,0,5);
		}
	}
}
void Factor(){
	switch (sym){
		case TAG_TYPEINT:{
			gen(LIT,0,number);
			getSym();
			break;
		};
		case TAG_TYPEREAL:{
			gen(LIT,0,number);
			getSym();
		};
		case TAG_LPARENT:{
			getSym();
			Expression();
			if (sym == TAG_RPARENT){
				getSym();
			}else{
				error(3);
			}
			break;
		}
		case TAG_ID:{
			FindId(id);
			if(position == 0){
				error(11);
			}
			symbolItem item = symbolTable[position];
			int praNumber = 1;
			switch(symbolTable[position].kind){
				case constants:{
					if(item.type == inttype){
						gen(LIT,0,item.value.intValue);
					}else if(item.type == realtype){
						gen(LIT,0,item.value.realValue);
					}else if(item.type == chartype){
						gen(LIT,0,item.value.charValue);
					}else{
						error(13);
					}
					getSym();
					break;
				}
				case variables:{
					if(item.type != arraytype){
						gen(LOD,level - item.level,address);
						getSym();
					}else{
						getSym();
						if(sym != TAG_LPARENT){
							error(5);
						}
						Expression();
						gen(IDX,0,item.ref);
						//施工中
					}
					break;
				}
				case functions:{
					getSym();
					if (sym != TAG_LPARENT){
						error(5);
					}
					Expression();
					while(sym == TAG_COMMA){
						Expression();
						praNumber++;
					}
					call(position);
					break;
					//施工中
				}
				case procedures:{
					error(24);
					break;
				}
				default:break;
			}
			getSym();
			break;
		}

	}
}
void Expression(){
	int op;
	if(sym == TAG_ADD){
		getSym();
	} else if(sym ==TAG_SUB){
		gen(OPR,0,1);
		getSym();
	}
	Term();
	while(sym == TAG_ADD || sym == TAG_SUB){
		op = sym;
		getSym();
		Term();
		if(op == TAG_ADD){
			gen(OPR,0,2);
		}else{
			gen(OPR,0,3);
		}
	}
}
void Statement(){
	if (sym == TAG_IF){
		IfStatement();
	}else if(sym == TAG_WHILE){
		WhileStatement();
	}else if(sym == TAG_FOR){
		ForStatement();
	}else if(sym == TAG_WRITE){
		WriteStatement();
	}else if(sym == TAG_READ){
		ReadStatement();
	}else if(sym == TAG_ID){
		position = FindId(id);
		if (position == 0){
			error(10);
		}
		if(symbolTable[position].kind == procedures){
			ProcedureStatement();
		}else if (symbolTable[position].kind == variables){
			AssignStatement();
		}
	}else if(sym == TAG_BEGIN){
		CompoundStatement();
	}
}
void CompoundStatement(){
	if(sym != TAG_BEGIN){
		error(22);
	}
	getSym();
	Statement();
	while(sym == TAG_COLON){
		Statement();
	}
	if(sym != TAG_END){
		error(23);
	}
	getSym();
}
int FindId(string name){
	int index = sti;
	symbolTable[0].name = name;
	while (symbolTable[index].name != name){
		index--;
	}
	return index;

}

void Enter(kinds kind){
	sti++;
	symbolTable[sti].name = id;
	symbolTable[sti].kind = kind;
	btab[display[level]].last = sti;
	switch(kind){
		case constants:{
			symbolTable[sti].type = type;
			if(type == inttype){
				symbolTable[sti].value.intValue = ints;
			}else if(type == realtype){
				symbolTable[sti].value.realValue = reals;
			}else if(type == chartype){
				symbolTable[sti].value.charValue = chars;
			}
			break;
		}
		case variables:{
			symbolTable[sti].level = level;
			symbolTable[sti].address = address;
			address++;
			break;
		}
		case procedures:{
			bti++;
			symbolTable[sti].level = level;
			symbolTable[sti].ref = bti;
			symbolTable[sti].address = pli;
			level++;
			display[level] = symbolTable[sti].ref;
			break;
		}
		case functions:{
			bti++;
			symbolTable[sti].level = level;
			symbolTable[sti].ref = bti;
			symbolTable[sti].type = type;
			symbolTable[sti].address = pli;
			level++;
			display[level] = symbolTable[sti].ref;
			break;
		}


	};
}
void IfStatement(){
	int tempSym;
	int tempp;
	int tempp2 = 0;
	if(sym != TAG_IF){
		error(1);
	}
	getSym();
	Expression();
	tempSym = sym;
	gen(OPR,0,6);
	getSym();
	Expression();
	switch (tempSym){
		case TAG_EQUAL: gen(OPR,0,8);break;
		case TAG_NEQ: gen(OPR,0,9);break;
		case TAG_LESS: gen(OPR,0,10);break;
		case TAG_GREATER: gen(OPR,0,11);break;
		case TAG_GE: gen(OPR,0,12);break;
		case TAG_LE: gen(OPR,0,13);break;
		default: error(14);
	};
	if(sym != TAG_THEN){
		error(16);
	}
	getSym();
	tempp = pli;
	gen(JPC,0,0);
	Statement();
	tempp2 = pli;
	gen(JMP,0,0);
	pcodeList[tempp].p2 = pli;
	if(sym == TAG_ELSE){
		getSym();
		Statement();
	}
	pcodeList[tempp2].p2 = pli;
}
void ForStatement(){
	int tempp1;
	int tempp2;
	int tempp3;
	int pace;
	bool sub;
	if(sym != TAG_FOR){
		error(1);
	}
	getSym();
	if(sym != TAG_ID){
		error(19);
	}
	FindId(id);
	getSym();
	if(sym != TAG_ASSIGN){
		error(20);
	}
	getSym();
	Expression();
	gen(STO,level - symbolTable[position].level,symbolTable[position].address);
	if(sym != TAG_TO){
		error(21);
	}
	getSym();
	tempp1 = pli;
	gen(LOD,level - symbolTable[position].level,symbolTable[position].address);
	Expression();
	tempp2 = pli;
	gen(OPR,0,12);
	tempp3 = pli;
	gen(JPC,0,0);
	if(sym != TAG_BY){
		error(22);
	}
	getSym();
	if(sym == TAG_ADD){
		getSym();
	}else if(sym == TAG_SUB){
		getSym();
		sub = true;
	}
	if(sym != TAG_INT){
		error(23);
	}
	if(sub){
		pcodeList[tempp3].p2n = 13;
	}
	pace = number;
	getSym();
	if(sym != TAG_DO){
		error(24);
	}
	Statement();
	gen(LOD,level - symbolTable[position].level,symbolTable[position].address);
	gen(LIT,0,pace);
	if(sub){
		gen(OPR,0,3);
	}else{
		gen(OPR,0,2);
	}
	gen(STO,level - symbolTable[position].level,symbolTable[position].address);
	gen(JMP,0,tempp1);
	pcodeList[tempp3].p2 = pli;

}
void WhileStatement(){
	int tempp1;
	int tempp2;
	tempp1 = pli;
	if(sym != TAG_WHILE){
		error(1);
	}
	getSym();
	Expression();
	gen(OPR,0,6);
	tempp2 = pli;
	gen(JPC,0,0);
	if(sym != TAG_DO){
		error(18);
	}
	getSym();
	Statement();
	gen(JMP,0,tempp1);
	pcodeList[tempp2].p2 = pli;
}
void ProcedureStatement(){
	int praNumber = 1;
	if(sym != TAG_PROCEDURE){
		error(1);
	}
	FindId(id);
	symbolItem item = symbolTable[position];
	getSym();
	if (sym != TAG_LPARENT){
		error(5);
	}
	Expression();
	while(sym == TAG_COMMA){
		Expression();
		praNumber++;
	}
	call(position);
}
void WriteStatement(){
	if(sym != TAG_WRITE){
		error(1);
	}
	getSym();
	if(sym != TAG_LPARENT){
		error(40);
	}
	getSym();
	if(sym == TAG_TYPESTRING){
		for(int i = w.length();i>=0;i--){
			gen(LIT,0,w[i]);
		}
		gen(WRT,0,w.length());
		getSym();
		if(sym == TAG_COMMA){
			getSym();
			Expression();
			gen(WRT,0,0);
		}
	}else{
		Expression();
		gen(WRT,0,0);
	}
}
void ReadStatement(){
	if(sym != TAG_READ){
		error(1);
	}
	getSym();
	if (sym != TAG_LPARENT){
		error(40);
	}
	getSym();
	while(true){
		if(sym == TAG_ID){
			FindId(id);
			if(position == 0){
				error(11);
			}
			if(symbolTable[position].kind == variables){
				gen(RED,level - symbolTable[position].level,address);
			}else{
				error(12);
			}
		}else{
			error(4);
		}
		getSym();
		if(sym != TAG_COMMA){
			break;
		}
	}
	if(sym != TAG_RPARENT){
		error(22);
	}
	getSym();
}
void AssignStatement(){
	FindId(id);
	getSym();
	if(sym != TAG_ASSIGN){
		error(11);
	}


}
void FormalParamList(){
	int t0 = sti;
	if(sym == TAG_VAR)
		getSym();
	if(sym == TAG_ID){
		Enter(variables);
		getSym();
	}else{
		error(3);
	}
	while(sym == TAG_COMMA){
		getSym();
		if(sym == TAG_ID){
			Enter(variables);
			getSym();

		}else{
			error(3);
		}
	}
	int t1 = sti;
	if(sym == TAG_COLON){
		getSym();
		if(sym == TAG_INT){
			while(t0 < t1){
				t0++;
				type = inttype;
				symbolTable[t0].type = type;
			}
		}else if(sym == TAG_REAL){
			while(t0 < t1){
				t0++;
				type = realtype;
				symbolTable[t0].type = type;
			}
		}else if(sym == TAG_CHAR){
			while(t0 < t1){
				t0++;
				type = chartype;
				symbolTable[t0].type = type;
			}
		}
	}else{
		error(5);
	}
	if(sym != TAG_SEMICOLON)
		error(5);
	getSym();
}
void ProcedureHead(){
	if(sym != TAG_PROCEDURE)
		error(2);
	getSym();
	if(sym == TAG_ID){
		Enter(procedures);
		tsti = sti;
		taddress = address;
		level++;
		getSym();
		if(sym == TAG_LPARENT){
			getSym();
			while(sym != TAG_RPARENT)
				FormalParamList();
		} else {
			error(6);
		}
		if(sym != TAG_RPARENT){
			error(7);
		}
		getSym();
		if(sym != TAG_SEMICOLON){
			error(8);
		}
	} else {
		error(9);
	}
}
void Procedure(){
	ProcedureHead();
	CompoundStatement();
	sti = tsti;
	address = taddress;
	level--;
	while(sym == TAG_PROCEDURE){
		ProcedureHead();
		CompoundStatement();
		sti = tsti;
		address = taddress;
		level--;
	}
}

void FunctionHead(){
	while(sym == TAG_FUNC){
		getSym();
		if(sym == TAG_ID){
			Enter(functions);
			tsti = sti;
			taddress = address;
			level++;
			getSym();
			if(sym == TAG_LPARENT){
				getSym();
				FormalParamList();
			} else {
				error(10);
			}
			if(sym != TAG_RPARENT){
				error(11);
			}
			getSym();
			if(sym != TAG_COLON){
				error(12);
			}
			getSym();
			if(sym == TAG_INT){

			} else if(sym == TAG_REAL){

			} else if(sym == TAG_CHAR){

			} else{
				error(13);
			}
			getSym();
			if(sym != TAG_SEMICOLON){
				error(14);
			}
		} else {
			error(15);
		}
	}
}
void Function(){
	FunctionHead();
	CompoundStatement();
	sti = tsti;
	address = taddress;
	level--;
	while(sym == TAG_FUNC){
		ProcedureHead();
		CompoundStatement();
		sti = tsti;
		address = taddress;
		level--;
	}
}
void ConstDefine(){
	if(sym == TAG_ID){
		getSym();
		if (sym == TAG_SEMICN){
			getSym();
			if(sym == TAG_ADD){
				add = true;
				getSym();
			}else if(sym == TAG_SUB){
				sub = true;
				getSym();
			}
			switch (sym){
				case TAG_TYPEINT:{
					if(sub){
						ints = -ints;
					}
					type = inttype;
					Enter(constants);
					getSym();
					break;
				};
				case TAG_TYPEREAL:{
					if(sub){
						reals = -reals;
					}
					type = realtype;
					Enter(constants);
					getSym();
					break;
				};
				case TAG_TYPECHAR:{
					if(sub || add){
						error(4);
					}
					type = inttype;
					Enter(constants);
					getSym();
					break;
				};
				default:error(1);
			}
		}else{
			error(2);
		}
	}else{
		error(3);
	}
}
void VarDefine(){
	int t0 = sti;
	if(sym == TAG_ID){
		Enter(variables);
		getSym();
	}else{
		error(3);
	}
	while(sym == TAG_COMMA){
		getSym();
		if(sym == TAG_ID){
			Enter(variables);
			getSym();

		}else{
			error(3);
		}
	}
	int t1 = sti;
	if(sym == TAG_COLON){
		getSym();
		if(sym == TAG_INT){
			while(t0 < t1){
				t0++;
				type = inttype;
				symbolTable[t0].type = type;
			}
		}else if(sym == TAG_REAL){
			while(t0 < t1){
				t0++;
				type = realtype;
				symbolTable[t0].type = type;
			}
		}else if(sym == TAG_CHAR){
			while(t0 < t1){
				t0++;
				type = chartype;
				symbolTable[t0].type = type;
			}
		}else if(sym == TAG_ARRAY){
			getSym();
			if(sym != TAG_LBRACKET){
				error(4);
			}
			getSym();
			if(sym != TAG_TYPEINT){
				error(4);
			}
			int arrayOffset = ints;
			getSym();
			if(sym != TAG_RBRACKET){
				error(4);
			}
			getSym();
			if(sym != TAG_OF){
				error(4);
			}
			getSym();
			if(sym == TAG_INT){
				address = address - t1 + t0;
				while(t0 < t1){
					t0++;
					ati++;
					type = arraytype;
					symbolTable[t0].type = type;
					symbolTable[t0].ref = ati;
					atab[symbolTable[t0].ref].high = arrayOffset;
					atab[symbolTable[t0].ref].eltype = inttype;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
			}else if (sym == TAG_REAL){
				address = address - t1 + t0;
				while(t0 < t1){
					t0++;
					ati++;
					type = arraytype;
					symbolTable[t0].type = type;
					symbolTable[t0].ref = ati;
					atab[symbolTable[t0].ref].high = arrayOffset;
					atab[symbolTable[t0].ref].eltype = realtype;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
			}else if (sym == TAG_CHAR){
				address = address - t1 + t0;
				while(t0 < t1){
					t0++;
					ati++;
					type = arraytype;
					symbolTable[t0].type = type;
					symbolTable[t0].ref = ati;
					atab[symbolTable[t0].ref].high = arrayOffset;
					atab[symbolTable[t0].ref].eltype = chartype;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
			}else{
				error(4);
			}

		}
	}else{
		error(5);
	}
	if(sym != TAG_SEMICOLON)
		error(5);
	getSym();
}
void ConstDeclar(){
	if (sym != TAG_CONST)
		error(4);
	getSym();
	ConstDefine();
	while(sym == TAG_COMMA){
		ConstDefine();
	}
	if(sym != TAG_SEMICOLON)
		error(5);
	getSym();
}
void VarDeclar(){
	if (sym != TAG_VAR)
		error(4);
	getSym();
	VarDefine();
	while(sym == TAG_ID){
		VarDefine();
	}
}
void Block(){
	address = 5;
	if(sym == TAG_CONST){
		ConstDeclar();
	}
	if(sym == TAG_VAR){
		VarDeclar();
	}
	while(sym == TAG_PROCEDURE || sym == TAG_FUNC){
		if(sym == TAG_PROCEDURE){
			Procedure();
		} else if(sym == TAG_FUNC){
			Function();
		}
	}
	CompoundStatement();
}
void Program(){
	getSym();
	Block();
	if(sym != TAG_PERIOD){
		error(16);
	}else{
	}
}
int base(int level){
	int base;
	base = baseR;
	while(level>0){
		base = stack[base].value;
	}
	return base;
}
void interpret(){
	int p = 0;
	int si = 0;
	test();
	cout<<"START PL"<<endl;
	while(true){
		pcode item = pcodeList[p];
		p++;
		switch(item.operate){
			case LIT:{
				sti++;
				if(item.type == inttype || item.type == realtype){
					stack[si].value = item.p2;
				}else if(item.type == chartype){
					stack[si].value = item.p2;
				}else{
					//wrong
				}
				break;
			}
			case OPR:{
				switch(int(item.p2)){
					case 0:{
						break;
					}
					case 1:{
						stack[si].value = -stack[si].value;
						break;
					}
					case 2:{
						si--;
						stack[si].value = stack[si].value + stack[si+1].value;
						break;
					}
					case 3:{
						si--;
						stack[si].value = stack[si].value - stack[si+1].value;
						break;
					}
					case 4:{
						si--;
						stack[si].value = stack[si].value * stack[si+1].value;
						break;
					}
					case 5:{
						si--;
						stack[si].value = stack[si].value / stack[si+1].value;
						break;
					}
					case 6:{
						break;
					}
					case 8:{
						si--;
						if(stack[si].value == stack[si+1].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 9:{
						si--;
						if(stack[si].value == stack[si+1].value){
							stack[si].value = 0;
						}else{
							stack[si].value = 1;
						}
						break;
					}
					case 10:{
						si--;
						if(stack[si].value < stack[si].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 11:{
						si--;
						if(stack[si].value >= stack[si].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 12:{
						si--;
						if(stack[si].value > stack[si].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 13:{
						si--;
						if(stack[si].value <= stack[si].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					default:{
						cout<<"no such pcode prama"<<endl;
					}
				}
				break;
			}
			case LOD:{
				si++;
				stack[si].value = stack[base(item.p1)+item.p2].value;
				break;
			}
			case STO:{
				stack[base(item.p1)+item.p2].value = stack[si].value;
				si--;
			}
			case CAL:{
				stack[si+1].value = base(item.p1);
				stack[si+2].value = baseR;
				stack[si+3].value = p;
				baseR = si+1;
				p = item.p2;
				break;
			}
			case INT:{
				si = si + item.p2;
				break;
			}
			case JMP:{
				si = item.p2;
				break;
			}
			case JPC:{
				if(stack[si].value == 1){
					p = item.p2;
				}else{
					si--;
				}
			}
			case RED:{
				cin>>stack[base(item.p1)+item.p2].value;
			}
			case IDX:{
				si--;
				int offset = stack[si].value;
				if (offset>=atab[item.p2].high){
					cout<<"array away"<<endl;
				}
				stack[si].value =stack[base(item.p1) + 		stack[si+1].value + stack[si].value].value;
			}
			case WRT:{
				if(item.p2 == 1){
					if(stack[si].type == chartype){
						cout<<char(stack[si].value);
					}else{
						cout<<stack[si].value;
					}
					si--;
				}
				else{
					for(int i = si-item.p2+1;i<=si;i++){
						cout<<stack[si].value;
					}
					si = si-item.p2;
				}
			}
			default:{
				cout<<"no such pcode operate"<<endl;
			}
		}
	}
}
int main(){
	display[1] = 1;
	btab[1].last = 0;
	input = open("input.txt");
	Program();
	interpret();


}
