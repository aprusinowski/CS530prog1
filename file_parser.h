/*
  file_parser.h
    CS530, Fall 2016
*/
#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include <iterator>
#include <sstream>
#include <cctype>
#include <climits>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "file_parse_exception.h"

#define DELIMITER      " "
#define NEWLINE_C      '\n'
#define SINGLE_QUOTE   '\''
#define MAX_FIELDS      3
#define MAX_COLUMNS     4
#define LABEL           0
#define OPCODE          1
#define OPERAND         2
#define COMMENT         3
#define IS_COMMENT(C)              (C == '*' || C == ';')
#define INVALID_LABEL_START(S)     (!isalpha(S.front()) && s.front() != '#')
#define CHOP_FRONT_AND_NEWLINE(S)  (S = S.substr(1, S.size() - 2))
#define CHOP_FRONT(S)              (S.substr(1))
#define INVALID_LABEL_CHAR(C)      (!isalnum(C) && C != NEWLINE_C)


typedef vector<string>  rowVect;
typedef vector<rowVect> allRowsVect;

using namespace std;

class file_parser {
public:
    // ctor, filename is the parameter.  A driver program will read
    // the filename from the command line, and pass the filename to
    // the file_parser constructor.  Filenames must not be hard-coded.
    file_parser(string);

    // dtor
    ~file_parser(){};

    // reads the source file, storing the information is some
    // auxiliary data structure you define in the private section.
    // Throws a file_parse_exception if an error occurs.
    // if the source code file fails to conform to the above
    // specification, this is an error condition.
    void read_file();

    // returns the token found at (row, column).  Rows and columns
    // are zero based.  Returns the empty string "" if there is no
    // token at that location. column refers to the four fields
    // identified above.
    string get_token(unsigned int, unsigned int);

    // prints the source code file to stdout.  Should not repeat
    // the exact formatting of the original, but uses tabs to align
    // similar tokens in a column. The fields should match the
    // order of token fields given above (label/opcode/operands/comments)
    void print_file();

    // returns the number of lines in the source code file
    int size() {return number_of_lines;};

private:
    string filename;
    unsigned int number_of_lines;
    rowVect  contents;
    allRowsVect tokens;
    void tokenize_lines();
    bool is_valid_label(string);
};

#endif



































//#ifndef FILE_PARSER_H
//#define FILE_PARSER_H
//#define  IS_COMMENT(C)      (C == '*' || C == ';')
#define  IS_SPACE(C)        (C == ' ' || C == '\t')
#define  IS_LABEL(C)        (C == ':')
#define  IS_INCLUDE(C)      (C == '#')
#define  IS_Quotes(C)       (C == '\'')

//#include <string>
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <iomanip>
//#include <cstdlib>
//#include <cstring>
//#include "file_parse_exception.h"
//#include <sstream>
//
//using namespace std;
//
//class file_parser {
//
//public:
//    // ctor, filename is the parameter.  A driver program will read
//    // the filename from the command line, and pass the filename to
//    // the file_parser constructor.  Filenames must not be hard-coded.
//    file_parser(string);
//
//    // dtor
//    ~file_parser();
//
//    // reads the source file, storing the information is some
//    // auxiliary data structure you define in the private section.
//    // Throws a file_parse_exception if an error occurs.
//    // if the source code file fails to conform to the above
//    // specification, this is an error condition.
//    void read_file();
//
//    // returns the token found at (row, column).  Rows and columns
//    // are zero based.  Returns the empty string "" if there is no
//    // token at that location. column refers to the four fields
//    // identified above.
//    string get_token(unsigned int, unsigned int);
//
//    // prints the source code file to stdout.  Should not repeat
//    // the exact formatting of the original, but uses tabs to align
//    // similar tokens in a column. The fields should match the
//    // order of token fields given above (label/opcode/operands/comments)
//    void print_file();
//
//    // returns the number of lines in the source code file
//    int size();
//
//private:
//    //returns a vector of file_rows from a vector of strings
//    struct row{
//        string label;
//        string opcode;
//        string operand;
//        string comments;
//    };
//
//    file_parser::row tokenize(string);
//    void check_label(string, string::size_type);
//    void check_invalid_tokens(int,row);
//    void add_comment(row &,string,string::size_type, int &);
//    void add_label(file_parser::row &, string, string::size_type& , string::size_type &, int, const string &basicString);
//    void add_opcode(row &,string,string::size_type &,string::size_type &,int &,const string&);
//    void add_operand(row &,string,string::size_type &,string::size_type &,int &,const string&);
//    void add_include(row &,string,string::size_type &,string::size_type &,int &,const string&);
//
//
//    string file_name;
//    vector<file_parser::row> file_rows;
//    vector<string> token_lines;
//
//    int number_of_lines;
//
//
//
//};
//
//#endif

