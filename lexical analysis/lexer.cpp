#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

#define TAG_ADD 256				// +
#define TAG_SUB 257				// -
#define TAG_MUL 258				// *
#define TAG_DIV 259				// /
#define TAG_SMALLER 260			// <
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
#define TAG_PRODURE 296
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
#define TAG_ASSIGN 309			// [
#define TAG_TYPESTRING 310

struct syms
{
	int ints;
	string content;
	char chars;
	string strings;
	double reals;
};
struct lexer
{
	int cate;
	syms sym;
	int lineno;
};

string symName[]={"const","integer","real","char","var","array","of",
	"if","then","else","do","while","for","to","by","procedure",
	"function","read","write","begin","end"};
int categoryName[]={TAG_CONST,TAG_INT,TAG_REAL,TAG_CHAR,TAG_VAR,TAG_ARRAY,TAG_OF,
	TAG_IF,TAG_THEN,TAG_ELSE,TAG_DO,TAG_WHILE,TAG_FOR,TAG_TO,TAG_BY,TAG_PRODURE,
	TAG_FUNC,TAG_READ,TAG_WRITE,TAG_BEGIN,TAG_END};
int typeNumber = 23;
ifstream input;
ofstream output;
string w = "";
char wc;
double number = 0;
char ch = ' ';
string pLine;
string filename;
int isEnd = 0;
lexer lex[5000];
int cate;
int sym;
int lineno;
int ints;
double reals;
int lineNo = 1;
string id;
char chars;
string strings;
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
void GetSym(){
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
			sym = TAG_SMALLER;
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
int main(){
	cout << "Please enter the filename of sourcecode" << endl;
	cin >> filename;
	input.open(filename);
	output.open("11061017_token.txt");
	streambuf* pOld = cout.rdbuf(output.rdbuf());
	while(isEnd == 0){
		GetSym();
	}
	input.close();
	output.close();
	cout.rdbuf(pOld);
	system("pause");
}
