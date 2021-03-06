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
#define TAG_LBRACKET 306			// [
#define TAG_RBRACKET 307			// ]
#define TAG_NEQ 308			// 
#define TAG_ASSIGN 309			// :=
#define TAG_TYPESTRING 310

enum types{inttype,realtype,chartype,arraytype,booltype,notype,stringtype};
enum kinds{procedures,functions,constants,variables};
enum operates {LIT,OPR,LDA,WRT,LOD,ADR,RED,INT,JMP,JPC,SWT,CAS,MKS,CAL,RTN,STO,STR,STI,IDX};
string operateArray[20] = {"LIT","OPR","LDA","WRT","LOD","ADR","RED","INT","JMP","JPC","SWT","CAS","MKS","CAL","RTN","STO","STR","STI","IDX"};
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
int level = 0;
int pramaIndex = 0;
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
	int address;
	types type;
	int sign[20];
	types typeSign[20];
};
class atabItem{
public:
	types eltype;
	int high;
	int size;
	int elsize;
	int address;
};
class stackItem{
public:
		double value;
		int level;
		char chars;
		string strings;
		types type;
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
int bti = 0;
int address;
int baseR = 0;
void getch();
void error(int no);
int isdigit(char c);
int isalpha(char c);
void getSym();
string idUp(string id);

void Factor();
void Expression();
void Expression_v();
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
	for(int i=0;i<pli;i++){
		cout<<i<<","<<operateArray[pcodeList[i].operate]<<","<<pcodeList[i].p1<<","<<pcodeList[i].p2<<endl;
	}
}
void getch(){
	if (input.peek() != EOF){
		ch = input.get();
	}else{
		cout<<"program incomlete"<<endl;
		system("pause");
	}
}
void error(int no1,int no2){
	cout<<"error: "<<no2<<"   "<<sym<<endl;
	input.close();
	output.close();
	if(no1 == 0){
		switch(no2){
			case 0:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为字母、空格或者数字"<<" in Line "<< lineNo<<endl;
				break;
			}
			case 1:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为 ' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 2:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为 \" "<<" in Line "<< lineNo<<endl;
				break;
			}
			default:{
				cout<<"UNKOWN ERROR"<<no1<<"."<<no2<<""<<" in Line "<< lineNo<<endl;
			}
		}
	}else if(no1 == 1){
		switch(no2){
			case 0:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"标识符未定义 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 1:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"标识符重复定义 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 2:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为'[' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 3:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为']' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 4:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为'(' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 5:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为')' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 6:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为'\"' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 7:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为':' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 8:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为'=' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 9:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为':=' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 10:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为';' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 11:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为标识符 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 12:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为begin "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 13:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为end "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 14:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为for "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 15:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为while "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 16:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为to "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 17:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为do "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 18:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为by "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 19:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为if "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 20:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为then "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 21:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为':=' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 22:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为write "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 23:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为read "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 24:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为procedure "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 25:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为function "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 26:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"字符不能参于运算 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 27:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为of "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 28:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为const "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 29:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为var "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 30:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为'.' "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 36:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为实数 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 37:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为整型数"<<" in Line "<< lineNo<<endl;
				break;
			}
			case 38:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"此处应为变量 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 39:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"未指定的比较运算符 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 40:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"过程的调用方式有误 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 41:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"变量数据类型未声明 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 42:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"常量数据类型未声明 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 43:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"标识符未声明 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 44:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"函数调用时参数数量不符 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 45:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"过程调用时参数数量不符 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 46:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"参数只能是变量 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 47:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"变量的类型有误 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 48:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"参数的种类有误 "<<" in Line "<< lineNo<<endl;
				break;
			}
			case 49:{
				cout<<"ERROR"<<no1<<"."<<no2<<": "<<"错误的数组元素类型 "<<" in Line "<< lineNo<<endl;
				break;
			}
			default:{
				cout<<"UNKOWN ERROR"<<no1<<"."<<no2<<""<<endl;
			}
		}

	}
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
	while( ch == ' ' || ch == '\n' || ch== '\t'){
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
				cout<<w<<endl;
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
			getch();
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
		cout<<sym<<endl;
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
		sym = TAG_SEMICN;
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
			sym = TAG_COLON;
			input.seekg(-1,ios::cur);
		}
	} else if(ch == '\''){
		getch();
		if(!isdigit(ch) && !isalpha(ch)){
			error(0,0);
		}
		wc = ch;
		getch();
		if(ch != '\''){
			error(0,1);
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
			error(0,2);
		}
		strings = w;
		sym = TAG_TYPESTRING;
	}else{
		cout<<"unknow ch"<<endl;
	}
	cout<<sym<<endl;
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
			gen(LIT,2,number);
			getSym();
			break;
		};
		case TAG_LPARENT:{
			getSym();
			Expression();
			if (sym == TAG_RPARENT){
				getSym();
			}else{
				error(1,3);
			}
			break;
		}
		case TAG_ID:{
			position = FindId(id);
			if(position == 0){
				error(1,43);
			}
			symbolItem item = symbolTable[position];
			switch(item.kind){
				case constants:{
					if(item.type == inttype){
						gen(LIT,0,item.value.intValue);
					}else if(item.type == realtype){
						gen(LIT,2,item.value.realValue);
					}else if(item.type == chartype){
						gen(LIT,3,item.value.charValue);
					}else{
						error(1,42);
					}
					getSym();
					break;
				}
				case variables:{
					if(item.type != arraytype){
						gen(ADR,level - item.level,item.address);
						if(item.type == inttype){
							gen(LOD,0,0);
						}else if(item.type == realtype){
							gen(LOD,2,0);
						}else if(item.type == chartype){
							gen(LOD,3,0);
						}else{
							error(1,41);
						}
						getSym();
					}else{
						getSym();
						if(sym != TAG_LBRACKET){
							error(1,2);
						}
						getSym();
						Expression();
						if(sym != TAG_RBRACKET){
							error(1,3);
						}
						gen(IDX,level - item.level,item.ref);
						getSym();
						//æ–½å·¥ä¸?
					}
					break;
				}
				case functions:{
					int praNumber = 1;
					getSym();
					if(sym != TAG_LPARENT){
						error(1,4);
					}
					getSym();
					if(sym == TAG_RPARENT){
						praNumber = 0;
						if(btab[item.ref].psize != praNumber){
							error(1,44);
						}
						gen(CAL,item.level,item.ref);
						getSym();
						return;
					}
					if(btab[item.ref].sign[praNumber-1] == 1){
						Expression_v();
					}else{
						Expression();
					}
					while(sym == TAG_COMMA){
						getSym();
						praNumber++;
						if(btab[item.ref].sign[praNumber-1] == 1){
							Expression_v();
						}else{
							Expression();
						}
					}		
					if(btab[item.ref].psize != praNumber){
						error(1,44);
					}
					if(sym != TAG_RPARENT){
						error(1,5);
					}
					getSym();
					gen(CAL,item.level,item.ref);
					break;
					//é‚è—‰ä¼æ¶“?
				}
				case procedures:{
					error(1,40);
					break;
				}
				default:break;
			}
		}

	}
}
void Expression(){
	int op;
	int sub = 0;
	if(sym == TAG_SUB){
		sub = 1;
		getSym();
	}else if(sym == TAG_ADD){
		getSym();
	}
	Term();
	if(sub){
		gen(OPR,0,1);
	}
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
void Expression_v(){
	if(sym != TAG_ID){
		error(1,46);
	}
	position = FindId(id);
	symbolItem item = symbolTable[position];
	if (item.kind == variables){
		if(item.type == arraytype){
			getSym();
			if(sym != TAG_LBRACKET){
				error(1,2);
			}
			getSym();
			Expression();
			gen(LIT,0,item.address);
			gen(OPR,0,2);
			gen(LIT,1,item.level);
			if(sym != TAG_RBRACKET){
				error(1,3);
			}
		}else{
			position = FindId(id);
			gen(LIT,0,item.address);
			gen(LIT,1,item.level);
		}
	}else{
		error(1,46);
	}
	getSym();
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
			error(1,0);
		}
		if(symbolTable[position].kind == procedures){
			ProcedureStatement();
		}else if (symbolTable[position].kind == variables || symbolTable[position].kind == functions){
			AssignStatement();
		}
	}else if(sym == TAG_BEGIN){
		CompoundStatement();
	}
}
void CompoundStatement(){
	if(sym != TAG_BEGIN){
		error(1,12);
	}
	getSym();
	Statement();
	while(sym == TAG_SEMICOLON){
		getSym();
		Statement();
	}
	if(sym != TAG_END){
		error(1,13);
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
			btab[bti].address = pli;
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
			btab[bti].address = pli;
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
		error(1,1);
	}
	getSym();
	Expression();
	tempSym = sym;
	getSym();
	Expression();
	switch (tempSym){
		case TAG_SEMICN: gen(OPR,0,8);break;
		case TAG_NEQ: gen(OPR,0,9);break;
		case TAG_LESS: gen(OPR,0,10);break;
		case TAG_GREATER: gen(OPR,0,11);break;
		case TAG_GE: gen(OPR,0,12);break;
		case TAG_LE: gen(OPR,0,13);break;
		default: error(1,39);
	};
	if(sym != TAG_THEN){
		error(1,20);
	}
	getSym();
	tempp = pli;
	gen(JPC,1,0);
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
	bool sub = false;
	if(sym != TAG_FOR){
		error(1,14);
	}
	getSym();
	if(sym != TAG_ID){
		error(1,38);
	}
	position = FindId(id);
	getSym();
	if(sym != TAG_ASSIGN){
		error(1,21);
	}
	getSym();
	gen(ADR,level - symbolTable[position].level,symbolTable[position].address);
	Expression();
	if(symbolTable[position].type == inttype){
		gen(STO,0,0);
	}else{
		gen(STO,0,1);
	}
	if(sym != TAG_TO){
		error(1,16);
	}
	getSym();
	tempp1 = pli;
	gen(ADR,level - symbolTable[position].level,symbolTable[position].address);
	if(symbolTable[position].type == inttype){
		gen(LOD,0,0);
	}else if(symbolTable[position].type == realtype){
		gen(LOD,2,0);
	}else if(symbolTable[position].type == chartype){
		gen(LOD,3,0);
	}
	Expression();
	tempp2 = pli;
	gen(OPR,0,12);
	tempp3 = pli;
	gen(JPC,0,0);
	if(sym != TAG_BY){
		error(1,18);
	}
	getSym();
	if(sym == TAG_ADD){
		getSym();
	}else if(sym == TAG_SUB){
		getSym();
		sub = true;
	}
	if(sym != TAG_TYPEINT){
		error(1,37);
	}
	if(sub){
		pcodeList[tempp3].p2 = 13;
	}
	pace = number;
	getSym();
	if(sym != TAG_DO){
		error(1,17);
	}
	getSym();
	Statement();
	gen(ADR,level - symbolTable[position].level,symbolTable[position].address);
	gen(ADR,level - symbolTable[position].level,symbolTable[position].address);
	if(symbolTable[position].type == inttype){
		gen(LOD,0,0);
	}else if(symbolTable[position].type == realtype){
		gen(LOD,2,0);
	}else if(symbolTable[position].type == chartype){
		gen(LOD,3,0);
	}
	gen(LIT,0,pace);
	if(sub){
		gen(OPR,0,3);
	}else{
		gen(OPR,0,2);
	}
	if(symbolTable[position].type == inttype){
		gen(STO,0,0);
	}else{
		gen(STO,0,1);
	}
	gen(JMP,0,tempp1);
	pcodeList[tempp3].p2 = pli;

}
void WhileStatement(){
	int op;
	int tempp1;
	int tempp2;
	tempp1 = pli;
	if(sym != TAG_WHILE){
		error(1,15);
	}
	getSym();
	Expression();
	op = sym;
	getSym();
	Expression();
	if(op == TAG_GREATER){
		gen(OPR,0,12);
	}else if(op == TAG_GE){
		gen(OPR,0,11);
	}else if(op == TAG_LESS){
		gen(OPR,0,10);
	}else if(op == TAG_LE){
		gen(OPR,0,13);
	}else if(op == TAG_SEMICN){
		gen(OPR,0,8);
	}else if(op == TAG_NEQ){
		gen(OPR,0,9);
	}
	tempp2 = pli;
	gen(JPC,1,0);
	if(sym != TAG_DO){
		error(1,17);
	}
	getSym();
	Statement();
	gen(JMP,0,tempp1);
	pcodeList[tempp2].p2 = pli;
}
void ProcedureStatement(){
	int praNumber = 1;
	position = FindId(id);
	symbolItem item = symbolTable[position];
	getSym();
	if (sym != TAG_LPARENT){
		error(1,4);
	}
	getSym();
	if(sym == TAG_RPARENT){
		praNumber = 0;
		if(btab[item.ref].psize != praNumber){
			error(1,45);
		}	
		gen(CAL,level - item.level,item.ref);
		getSym();
		return;
	}
	if(btab[item.ref].sign[praNumber-1] == 1){
		Expression_v();
	}else{
		Expression();
	}
	while(sym == TAG_COMMA){
		getSym();
		praNumber++;
		if(btab[item.ref].sign[praNumber-1] == 1){
			Expression_v();
		}else{
			Expression();
		}
	}						
	if(btab[item.ref].psize != praNumber){
		error(1,45);
	}
	if (sym != TAG_RPARENT){
		error(1,5);
	}
	getSym();
	gen(CAL,level - item.level,item.ref);
}
void WriteStatement(){
	if(sym != TAG_WRITE){
		error(1,22);
	}
	getSym();
	if(sym != TAG_LPARENT){
		error(1,4);
	}
	getSym();
	if(sym == TAG_TYPESTRING){
		for(int i = w.length();i>=0;i--){
			gen(LIT,0,w[i]);
		}
		gen(WRT,0,int(w.length()));
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
	if(sym != TAG_RPARENT){
		error(1,5);
	}
	getSym();
}
void ReadStatement(){
	if(sym != TAG_READ){
		error(1,23);
	}
	getSym();
	if (sym != TAG_LPARENT){
		error(1,4);
	}
	getSym();
	while(true){
		if(sym == TAG_ID){
			position = FindId(id);
			if(position == 0){
				error(1,43);
			}
			if(symbolTable[position].kind == variables){
				gen(RED,level - symbolTable[position].level,symbolTable[position].address);
			}else{
				error(1,38);
			}
		}else{
			error(1,46);
		}
		getSym();
		if(sym != TAG_COMMA){
			break;
		}
		getSym();
	}
	if(sym != TAG_RPARENT){
		error(1,5);
	}
	getSym();
}
void AssignStatement(){
	position = FindId(id);
	getSym();
	symbolItem item = symbolTable[position];
	if(item.kind == variables){
		if(item.type == arraytype){
			if(sym != TAG_LBRACKET){
				error(1,2);
			}
			getSym();
			gen(ADR,level - item.level,item.address);
			Expression();
			if(sym != TAG_RBRACKET){
				error(1,3);
			}
			getSym();
			if(sym != TAG_ASSIGN){
				error(1,9);
			}
			getSym();
			Expression();
			gen(STI,0,item.ref);;
		}else if(item.type == inttype || item.type == chartype || item.type == realtype){
			if(sym != TAG_ASSIGN){
				error(1,9);
			}
			getSym();
			gen(ADR,level - item.level,item.address);
			Expression();
			if(item.type == inttype){
				gen(STO,0,0);
			}else{
				gen(STO,0,1);
			}
		}else{
			error(1,47);
		}
	}else if(item.kind == functions){
		if(sym != TAG_ASSIGN){
			error(1,9);
		}
		getSym();
		Expression();
		if(item.type == inttype){
			gen(STR,0,0);
		}else{
			gen(STR,0,1);
		}		
	}else{
		error(1,48);
	}
}
void FormalParamList(){
	int t0 = sti;
	int var = 0;
	if(sym == TAG_RPARENT){
		btab[symbolTable[position].ref].psize += 0;
		return;
	}
	if(sym == TAG_VAR){
		getSym();
		var = 1;
	}
	if(sym == TAG_ID){
		Enter(variables);
		if(var){
			btab[symbolTable[position].ref].sign[pramaIndex++] = 1;
		}else{
			btab[symbolTable[position].ref].sign[pramaIndex++] = 0;
		}
		getSym();
	}else{
		error(1,11);
	}
	while(sym == TAG_COMMA){
		getSym();
		if(sym == TAG_ID){
			Enter(variables);
			if(var){
				btab[symbolTable[position].ref].sign[pramaIndex++] = 1;
			}else{
				btab[symbolTable[position].ref].sign[pramaIndex++] = 0;
			}
			getSym();

		}else{
			error(1,11);
		}
	}
	int t1 = sti;
	btab[symbolTable[position].ref].psize += (t1 - t0);
	if(sym == TAG_COLON){
		getSym();
		if(sym == TAG_INT){
			while(t0 < t1){
				t0++;
				type = inttype;
				symbolTable[t0].type = type;
				for(int i = t1 - t0;i>0;i--){
					btab[symbolTable[position].ref].typeSign[pramaIndex - i] = inttype;
				}
			}
		}else if(sym == TAG_REAL){
			while(t0 < t1){
				t0++;
				type = realtype;
				symbolTable[t0].type = type;
				for(int i = t1 - t0;i>0;i--){
					btab[symbolTable[position].ref].typeSign[pramaIndex - i] = realtype;
				}
			}
		}else if(sym == TAG_CHAR){
			while(t0 < t1){
				t0++;
				type = chartype;
				symbolTable[t0].type = type;
				for(int i = t1 - t0;i>0;i--){
					btab[symbolTable[position].ref].typeSign[pramaIndex - i] = chartype;
				}
			}
		}
	}else{
		error(1,7);
	}
	getSym();
}
void ProcedureHead(){
	if(sym != TAG_PROCEDURE)
		error(1,23);
	pramaIndex = 0;
	getSym();
	if(sym == TAG_ID){
		Enter(procedures);
		address = 5;
		position = FindId(id);
		getSym();
		if(sym == TAG_LPARENT){
			getSym();
			FormalParamList();
			while(sym == TAG_SEMICOLON){
				getSym();
				FormalParamList();
			}
		} else {
			error(1,4);
		}
		if(sym != TAG_RPARENT){
			error(1,5);
		}
		getSym();
		if(sym != TAG_SEMICOLON){
			error(1,10);
		}
		getSym();
	} else {
		error(1,11);
	}
}
void Procedure(){
	int tsti = sti+1;
	ProcedureHead();
	Block();
	gen(RTN,0,0);
	sti = tsti;
	level--;
	if(sym != TAG_SEMICOLON){
		error(1,10);
	}
	getSym();
	while(sym == TAG_PROCEDURE){
		tsti = sti+1;
		ProcedureHead();
		Block();
		gen(RTN,0,0);
		sti = tsti;
		level--;
		if(sym != TAG_SEMICOLON){
			error(1,10);
		}
		getSym();
	}
}

void FunctionHead(){
	if(sym != TAG_FUNC)
		error(1,23);
	pramaIndex = 0;
	getSym();
	if(sym == TAG_ID){
		Enter(functions);
		address = 5;
		position = FindId(id);
		getSym();
		if(sym == TAG_LPARENT){
			getSym();
			FormalParamList();
			while(sym == TAG_SEMICOLON){
				getSym();
				FormalParamList();
			}
		} else {
			error(1,4);
		}
		if(sym != TAG_RPARENT){
			error(1,5);
		}
		getSym();
		if(sym != TAG_COLON){
			error(1,7);
		}
		getSym();
		if(sym == TAG_INT){
			btab[symbolTable[position].ref].type = inttype;
		}else if(sym == TAG_REAL){
			btab[symbolTable[position].ref].type = realtype;
		}else if(sym == TAG_CHAR){
			btab[symbolTable[position].ref].type = chartype;
		}
		getSym();
		if(sym != TAG_SEMICOLON){
			error(1,10);
		}
		getSym();
	} else {
		error(1,11);
	}
}
void Function(){
	int tsti = sti+1;
	FunctionHead();
	Block();
	gen(RTN,0,1);
	sti = tsti;
	level--;
	if(sym != TAG_SEMICOLON){
		error(1,10);
	}
	getSym();
	while(sym == TAG_FUNC){
		tsti = sti+1;
		FunctionHead();
		Block();
		gen(RTN,0,1);
		sti = tsti;
		level--;
		if(sym != TAG_SEMICOLON){
			error(1,10);
		}
		getSym();
	}
}
void ConstDefine(){
	bool sub =false;
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
						error(1,25);
					}
					type = inttype;
					Enter(constants);
					getSym();
					break;
				};
				default:;
			}
		}else{
			error(1,8);
		}
	}else{
		error(1,11);
	}
}
void VarDefine(){
	int t0 = sti;
	if(sym == TAG_ID){
		Enter(variables);
		getSym();
	}else{
		error(1,11);
	}
	while(sym == TAG_COMMA){
		getSym();
		if(sym == TAG_ID){
			Enter(variables);
			getSym();

		}else{
			error(1,11);
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
			getSym();
		}else if(sym == TAG_REAL){
			while(t0 < t1){
				t0++;
				type = realtype;
				symbolTable[t0].type = type;
				getSym();
			}
		}else if(sym == TAG_CHAR){
			while(t0 < t1){
				t0++;
				type = chartype;
				symbolTable[t0].type = type;
				getSym();
			}
		}else if(sym == TAG_ARRAY){
			getSym();
			if(sym != TAG_LBRACKET){
				error(1,2);
			}
			getSym();
			if(sym != TAG_TYPEINT){
				error(1,36);
			}
			int arrayOffset = ints;
			getSym();
			if(sym != TAG_RBRACKET){
				error(1,3);
			}
			getSym();
			if(sym != TAG_OF){
				error(1,26);
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
					atab[symbolTable[t0].ref].address = address;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
				getSym();
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
					atab[symbolTable[t0].ref].address = address;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
				getSym();
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
					atab[symbolTable[t0].ref].address = address;
					symbolTable[t0].address = address;
					address += arrayOffset;
				}
				getSym();
			}else{
				error(1,49);
			}

		}
	}else{
		error(1,7);
	}
	if(sym != TAG_SEMICOLON)
		error(1,10);
	getSym();
}
void ConstDeclar(){
	if (sym != TAG_CONST)
		error(1,27);
	getSym();
	ConstDefine();
	while(sym == TAG_COMMA){
		getSym();
		ConstDefine();
	}
	if(sym != TAG_SEMICOLON)
		error(1,10);
	getSym();
}
void VarDeclar(){
	if (sym != TAG_VAR)
		error(1,28);
	getSym();
	VarDefine();
	while(sym == TAG_ID){
		VarDefine();
	}
}
void Block(){
	int tempp;
	int tempAdress = 0;
	int tempSti = sti;
	if(sym == TAG_CONST){
		ConstDeclar();
	}
	if(sym == TAG_VAR){
		VarDeclar();
	}
	tempp = pli;
	tempAdress = address;
	gen(JMP,0,0);
	while(sym == TAG_PROCEDURE || sym == TAG_FUNC){
		if(sym == TAG_PROCEDURE){
			Procedure();
		} else if(sym == TAG_FUNC){
			Function();
		}
	}
	pcodeList[tempp].p2 = pli;
	gen(INT,0,tempAdress);
	CompoundStatement();
	sti = tempSti;
}
void Program(){
	getSym();
	address = 5;
	btab[0].address = 0;
	btab[0].psize = 0;
	gen(MKS,0,0);
	Block();
	if(sym != TAG_PERIOD){
		error(1,29);
	}else{
	}
}
int base(int level){
	int base;
	base = baseR;
	while(level>0){
		base = stack[base].value;
		level--;
	}
	return base;
}
void interpret(){
	int p = 0;
	int si = 0;
	int l = 0;
	baseR = 0;
	level =0;
	display[0] = 0;
	test();
	cout<<"START PL"<<endl;
	while(p<pli){
		pcode item = pcodeList[p];
		p++;
		switch(item.operate){
			case LIT:{
				if(item.p1 == 0){
					si++;
					stack[si].value = item.p2;
				}else{
					stack[si].level = item.p2;
				}
				if(item.p1 < 2){
					stack[si].type = inttype;
				}else if(item.p1 == 2){
					si++;
					stack[si].type = realtype;
					stack[si].value = item.p2;
				}else if(item.p1 == 3){
					si++;
					stack[si].type = chartype;
					stack[si].value = item.p2;
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
						if(stack[si].type == inttype && stack[si+1].type == inttype){
							stack[si].value = int(stack[si].value / stack[si+1].value);
						}else{
							stack[si].value = stack[si].value / stack[si+1].value;
						}
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
						if(stack[si].value != stack[si+1].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 10:{
						si--;
						if(stack[si].value < stack[si+1].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 11:{
						si--;
						if(stack[si].value >= stack[si+1].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 12:{
						si--;
						if(stack[si].value > stack[si+1].value){
							stack[si].value = 1;
						}else{
							stack[si].value = 0;
						}
						break;
					}
					case 13:{
						si--;
						if(stack[si].value <= stack[si+1].value){
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
				stack[si].value = stack[stack[si].value].value;
				if(item.p1 == 0){
					stack[si].type = inttype;
				}else if(item.p1 == 2){
					stack[si].type = realtype;
				}else if(item.p1 == 3){
					stack[si].type = chartype;
				}
				break;
			}
			case ADR:{
				si++;
				int tempAdress = item.p2;
				int tempLevel = item.p1;
				if(btab[stack[base(tempLevel)+4].value].psize >= (tempAdress - 4) && btab[stack[base(tempLevel)+4].value].sign[tempAdress-5] == 1){
					tempAdress = stack[base(item.p1) + item.p2].value;
					tempLevel = stack[base(item.p1) + item.p2].level;
					stack[si].value = base(level - tempLevel)+tempAdress;
					
				}else{
					stack[si].value = base(tempLevel) + tempAdress;
				}
				break;
			}
			case STO:{
				if(item.p2 == 0){
					stack[stack[si-1].value].value = int(stack[si].value);	
				}else if(item.p2 == 1){
					stack[stack[si-1].value].value = stack[si].value;
				}
				si=si-2;
				break;
			}
			case STR:{
				if(item.p2 == 0){
					stack[baseR + 3].value = int(stack[si].value);	
				}else if(item.p2 == 1){
					stack[baseR + 3].value = stack[si].value;
				}
				si--;	
				break;			
			}
			case STI:{
				int offset = stack[si-1].value;
				if (offset>=atab[item.p2].high){
					cout<<"array away"<<endl;
				}
				if(atab[item.p2].eltype == inttype){
					stack[stack[si-2].value + stack[si-1].value].value = int(stack[si].value);	
				}else if(atab[item.p2].eltype == realtype){
					stack[stack[si-2].value + stack[si-1].value].value = int(stack[si].value);	
				}else{

				}
				si = si-3;
				break;
			}
			case CAL:{
				for(int i = btab[item.p2].psize - 1;i>=0;i--){
					if(btab[item.p2].sign[btab[item.p2].psize-i-1] == 1){
						stack[si-i+5].value = stack[si-i].value;
						stack[si-i+5].level = stack[si-i].level;
						stack[si-i+5].type = btab[item.p2].typeSign[btab[item.p2].psize-i-1];

					}else{
						stack[si-i+5].value = stack[si-i].value;
						stack[si-i+5].type = btab[item.p2].typeSign[btab[item.p2].psize-i-1];
					}
				}
				si = si - btab[item.p2].psize + 1;
				stack[si].value =  base(level - item.p1);
				stack[si+1].value = baseR;
				stack[si+2].value = p;
				stack[si+3].value = -200;
				stack[si+4].value = item.p2;
				baseR = si;
				p = btab[item.p2].address;
				level = item.p1 + 1;
				display[++l] = level;
			//	si += btab[item.p2].psize + 5;
				break;
			}
			case RTN:{
				si = baseR-1;
				if(item.p2 == 1){
					si++;
					stack[si].value = stack[baseR + 3].value;
				}
				p = stack[baseR +2].value;
				baseR = stack[baseR+1].value;
				level = display[--l];
				break;
			}
			case MKS:{
				stack[si].value =  0;
				stack[si+1].value = baseR;
				stack[si+2].value = p;
				stack[si+3].value = 0;
				stack[si+4].value = item.p2;
				baseR = si;
				break;
			}
			case JMP:{
				p = item.p2;
				break;
			}
			case JPC:{
				if(stack[si].value == 1){
					if(item.p1 == 0){
						p = item.p2;
					}
				}else{
					if(item.p1 == 1){
						p = item.p2;
					}
				}
				si--;
				break;
			}
			case INT:{
				si = baseR + item.p2;
				break;
			}
			case RED:{
				cin>>stack[base(item.p1)+item.p2].value;
				break;
			}
			case IDX:{
				int offset = stack[si].value;
				if (offset>=atab[item.p2].high){
					cout<<"array away"<<endl;
				}
				stack[si].value = stack[base(item.p1) + offset + atab[item.p2].address].value;
				stack[si].type = atab[item.p2].eltype;
				break;
			}
			case WRT:{
				if(item.p2 == 0){
					if(stack[si].type == chartype){
						cout<<char(stack[si].value);
					}else{
						cout<<stack[si].value<<endl;
					}
					si--;
				}
				else{
					for(int i = si;i>=si-item.p2+1;i--){
						cout<<char(stack[i].value);
					}
					cout<<endl;
					si = si-item.p2;
				}
				break;
			}
			default:{
				cout<<"no such pcode operate"<<endl;
				si--;
			}
		}
	}
}
int main(){
	display[1] = 1;
	btab[1].last = 0;
	input.open("E:\\Program\\Github\\lexicalAnalysis\\in0.txt");
	Program();
	interpret();
	system("pause");
	return 1;
}
