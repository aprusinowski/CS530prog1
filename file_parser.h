/*
 * Artur Prusinowski, Anas Khafagi, Justin Waymire
 * Edoras account: cssc0631
 * Assignment 1
 *
  file_parser.h
  CS530, Fall 2016
*/

#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include <string>
#include <vector>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <cstddef>      //size_t
#include "file_parse_exception.h"

#define NOT_FOUND      string::npos
#define DELIMITER      " \n\r\t"
#define SINGLE_QUOTE   "\'"
#define MAX_COLUMNS     4
#define LABEL           0
#define OPCODE          1
#define OPERAND         2
#define COMMENT         3
#define IS_COMMENT(C)              (C == '*' || C == ';')
#define INVALID_LABEL_START(S)     (!isalpha(S.front()) && s.front() != '#')
#define CHOP_FRONT_AND_BACK(S)  (S = S.substr(1, S.size() - 2))
#define CHOP_FRONT(S)              (S.substr(1))
#define INVALID_LABEL_CHAR(C)      (!isalnum(C))

using namespace std;

typedef vector<string>  rowVect;
typedef vector<rowVect> allRowsVect;
typedef string::size_type pos_index;

class file_parser {
public:
    /**
    * ctor, filename is the parameter.  A driver program will read
    * the filename from the command line, and pass the filename to
    * the file_parser constructor.  Filenames must not be hard-coded.
    * @param filename - string containing the name of the source file to be tokenized
    * @return - file_parser object
    */
    file_parser(string);

    /**
     * Destructor
     * */
    ~file_parser(){};

    /**
    * reads the source file, storing the information is some
    * auxiliary data structure you define in the private section.
    * Throws a file_parse_exception if an error occurs.
    * if the source code file fails to conform to the above
    * specification, this is an error condition.
    */
    void read_file();

    /**
    * returns the token found at (row, column).  Rows and columns
    * are zero based.  Returns the empty string "" if there is no
    * token at that location. column refers to the four fields
    * identified above.
    * @param row - Row of the input source file (0 based)
    * @param col - Column from the input row. One of the 0 - 3 columns representing Label,
    * @return  - token string from row, col
    */
    string get_token(unsigned int, unsigned int);

    /**
    * prints the source code file to stdout.  Should not repeat
    * the exact formatting of the original, but uses tabs to align
    * similar tokens in a column. The fields should match the
    * order of token fields given above (label/opcode/operands/comments)
    */
    void print_file();

    /***
     * @return  returns the number of lines in the source code file
     */
    int size() {return line_tokens.size();};

private:
    /**source file file name*/
    string filename;

    /**Vector stores all tokens after call to tokenize_lines().
     * Size = number of lines in filename */
    allRowsVect line_tokens;

    /**Vector of string objects. Stores one*/
    rowVect  file_contents;

    /**
    * Function tokenizes and stores all original source code lines
    * @param file_contents - vector containing string objects each representing one line of unparsed text
     */
    void tokenize_lines(rowVect&);

    /**
    * Updates the two string indexes to point to the start and end of the next token on the line
    * @param row - string containing one complete source file row
    * @param head_pos - points to the last char in the next token to be processed
    * @param tail_pos - points to the first char in the next token to be processed
    */
    void find_next_token(const string&, pos_index& , pos_index& );

    /**
    * Function finds and returns the next complete token from the input line
    * @param row - string of the entire row from source file being tokenized
    * @param head_pos -  index position of the end of token
    * @param tail_pos - index position of the start of token
    * @return  - string consisting of the fully assembled token
    */
    string get_next_token(const string&,  pos_index& , pos_index& ) ;

    /**
    *  Check a string for proper LABEL format
    * @param s - string being checked for proper LABEL format
    * @return - true if string s follows proper LABEL format, false otherwise
    */
    bool is_valid_label(string);
};
#endif


