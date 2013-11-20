#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;
using std::vector;

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
#define TAG_SEMICOLON 276		// ;
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

int symNumber = 1;
int typeNumber = 23;
ifstream input;
ofstream output;
string w = "";
double number = 0;
int cate = 0;
int isEnd = 0;
int lexcate[5000];
string lexsym[5000];
vector<Item> symbolTable;
lexer lex[5000];
struct lexer
{
	int cate;
	string sym;
};
class Item{
	public:
		string name;
		int type;
		value value;
		int level;
		int lineNo;
		int offset;

};
class value{
	public:
		string stringValue;
		int intValue;
		char charValue;
};
void error(string errorInfo){
	cout << errorInfo << endl;
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

void getCate(){
	cate = lex[symNumber].cate;
	symNumber++;
}
void Exper(){
}
void Statement(){
}
void CompoundStatement(){
	VarDeclar();
	ConstDeclar();
}
void Function(){
}
void FormalParamList(){
	if(cate == TAG_VAR){
		getCate();
		if(cate == TAG_ID){
			getCate();
			while(cate == TAG_COMMA){
				getCate();
				if(cate == TAG_ID){

				} else {
					error("need ID");
				}
				getCate();
			}
			if(cate == TAG_COLON){
				getCate();
				if (cate == TAG_INT){

				}else if(cate == TAG_REAL){

				}else if(cate == TAG_CHAR){

				}else{
					error("");
				}
			} else {
				error("need COLON");
			}
		} else {
			error("need ID")
		}
		if(cate == TAG_SEMICOLON){
			FormalParamList();
		}else{
			error("need SEMICOLON");
		}
	} else {
		error("need VAR");
	}
}
void ProdureHead(){
	while(cate == TAG_PRODURE){
		getCate();
		if(cate == TAG_ID){
			getCate();
			if(cate == TAG_LPARENT){
				getCate();
				FormalParamList();
			} else {
				error("not (");
			}
			if(cate != TAG_RPARENT){
				error("");
			}
			getCate();
			if(cate != TAG_SEMICOLON){
				error("");
			}
		} else {
			error("not ID");
		}
	}
}
void Produre(){
	ProdureHead();
	CompoundStatement();
	while(cate != TAG_SEMICOLON){
		ProdureHead();
		CompoundStatement();
	}
}
void ConstDeclar(){
}
void VarDeclar(){
}
void Block(){
	ConstDeclar();
	VarDeclar();
	if(cate == TAG_PRODURE){
		Produre();
	} else if(cate == TAG_FUNC){
		Function();
	}
	CompoundStatement();
}
void Program(){
	getCate();
	Block();
	if(cate != TAG_PERIOD){
		error("");
	}else{
	}
}
void main(){

}
