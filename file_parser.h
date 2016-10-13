/*
  file_parser.h
    CS530, Fall 2016
*/
#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include <string>
#include <vector>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "file_parse_exception.h"

#define DELIMITER      " \n\r\t"
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
#define INVALID_LABEL_CHAR(C)      (!isalnum(C))


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
    int size() {return line_count;};

private:
    string filename;
    unsigned int line_count;
    allRowsVect line_tokens;

    void tokenize_lines(rowVect&);
    bool is_valid_label(string);
};

#endif

