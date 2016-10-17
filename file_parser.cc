#include "file_parser.h"

/**
 *ctor, filename is the parameter.  A driver program will read
  the filename from the command line, and pass the filename to
  the file_parser constructor.  Filenames must not be hard-coded.

 * @param filename - string containing the name of the source file to be tokenized
 * @return - file_parser object
 */


file_parser::file_parser(string filename) {
    this->filename = filename;
}
/**
 *   reads the source file, storing the information is some
     auxiliary data structure you define in the private section.
     Throws a file_parse_exception if an error occurs.
     if the source code file fails to conform to the above
     specification, this is an error condition.
 */
void file_parser::read_file() {
    string line;
    ifstream f_input(filename.c_str());
    if (!f_input.is_open())
        throw file_parse_exception("Could not open file: " + filename);
    while (getline(f_input, line))
        file_contents.push_back(line);
    f_input.close();
    tokenize_lines(file_contents);
}
/**
 *prints the source code file to stdout.  Should not repeat
  the exact formatting of the original, but uses tabs to align
  similar tokens in a column. The fields should match the
  order of token fields given above (label/opcode/operands/comments)
 */
void file_parser::print_file() {
    unsigned int i = 1;
    for (rowVect row: line_tokens)
        printf("%-*u:L: %-11s OC: %-11s OP: %-11s C: %-11s \n", 3, i++,
               row[LABEL].c_str(),row[OPCODE].c_str(), row[OPERAND].c_str(), row[COMMENT].c_str());
}
/**
 *   returns the token found at (row, column).  Rows and columns
     are zero based.  Returns the empty string "" if there is no
     token at that location. column refers to the four fields
     identified above.
 * @param row - Row of the input source file (0 based)
 * @param col - Column from the input row. One of the 0 - 3 columns representing Label,
 * @return  - token string from row, col
 */
string file_parser::get_token(unsigned int row, unsigned int col) {
    if (row > (line_tokens.size()))
        throw file_parse_exception("Row number " + to_string(row) + " out of bounds. ");
    if (col > MAX_COLUMNS)
        throw file_parse_exception("Row, Column number " + to_string(row) + "," + to_string(col) + " out of bounds.");
    return (line_tokens[row])[col];
}
/**
 * @param file_contents - vector containing string objects each representing one line of unparsed text
 */
void file_parser::tokenize_lines(rowVect& file_contents) {
    unsigned int current_token;
    pos_index head_pos; pos_index tail_pos;

    /**Begin looping through all the source lines*/
    for (string row: file_contents) {
        current_token = LABEL;
        rowVect new_row(MAX_COLUMNS, "");
        head_pos = tail_pos = LABEL;

        find_next_token(row, head_pos, tail_pos );
        while (head_pos != NOT_FOUND  || tail_pos != NOT_FOUND) {

            string token_str = get_next_token(row, head_pos, tail_pos);

            if (tail_pos !=LABEL && current_token == LABEL)
                current_token = OPCODE;

            if IS_COMMENT(token_str.front()) {                                          //if comment,store and break out
                new_row[COMMENT] = row.substr(tail_pos, NOT_FOUND );
                break;
            }
            /**If max columns have already been read in throw an exception*/
            if (current_token == (MAX_COLUMNS-1))
                throw file_parse_exception("Too many tokens on line: " + to_string(line_tokens.size() + 1));
            /**check for invalid label*/
            else if (tail_pos == LABEL && current_token == LABEL && !is_valid_label(token_str))
                throw file_parse_exception("Invalid label on line: " + to_string(line_tokens.size()  + 1));
            else
                new_row[current_token++] = token_str;

            find_next_token(row, head_pos, tail_pos );
        }
        line_tokens.push_back(new_row);

    }
}
/**
 * Updates the two string indexes to point to the start and end of the next token on the line
 * @param row - string containing one complete source file row
 * @param head_pos - points to the last char in the next token to be processed
 * @param tail_pos - points to the first char in the next token to be processed
 */
void file_parser::find_next_token(const string& row, pos_index& head_pos, pos_index& tail_pos){
    tail_pos = row.find_first_not_of(DELIMITER, head_pos);
    head_pos = row.find_first_of(DELIMITER, tail_pos);

}
/**
 * Function finds and returns the next complete token from the input line
 * @param row - string of the entire row from source file being tokenized
 * @param head_pos -  index position of the end of token
 * @param tail_pos - index position of the start of token
 * @return  - string consisting of the fully assembled token
 */
string file_parser::get_next_token(const string& row, pos_index& head_pos, pos_index& tail_pos ) {

    string token_str = row.substr(tail_pos, head_pos - tail_pos);
    /**If token_str is the last item on the line return it*/
    if(head_pos == NOT_FOUND)
        return token_str;

    /**Check if there are single quote chars in the token_str*/
    if (!(token_str.find(SINGLE_QUOTE) == NOT_FOUND)) {
        /**Try to find the closing single quote char */
        head_pos = row.find_first_of(SINGLE_QUOTE, head_pos);
//        /**If doesn't exist throw an exception*/
//        if(head_pos == NOT_FOUND)
//            throw file_parse_exception("Unterminated quoted string on line  " + to_string(line_tokens.size() + 1));
    }

    head_pos = row.find_first_of(DELIMITER, head_pos);

    return row.substr(tail_pos, head_pos - tail_pos);
}
/**
 *  Check a string for proper LABEL format
 * @param s - string being checked for proper LABEL format
 * @return - true if string s follows proper LABEL format, false otherwise
 */
bool file_parser::is_valid_label(string s) {
    if(s.empty())return true;
    /**Check first char. Must be a letter or a '#' for the #minclude label*/
    if INVALID_LABEL_START(s)
        return false;
    /**if last char is ':' remove it and the leading char from the string*/
    if (s.back() == ':')
        s = CHOP_FRONT_AND_BACK(s);
    /**otherwise remove leading char which was already validated*/
    else
        s = CHOP_FRONT(s);
    /**check remaining characters in the string with isalnum()*/
    for (char c: s)
        if INVALID_LABEL_CHAR(c)
            return false;
    return true;
}



//    unsigned int current_token;
//    pos_index head_pos; pos_index tail_pos;
//
//    for (string row: file_contents) {
//        current_token = LABEL;
//
//        rowVect new_row(MAX_COLUMNS, "");
//        head_pos = tail_pos = LABEL;
//
////        find_next_token(row, head_pos, tail_pos );
//        /**advance token count if leading whitespace */
////        if (tail_pos !=LABEL)
////            current_token = OPCODE;
//
//        while (head_pos != NOT_FOUND  || NOT_FOUND != tail_pos) {                       //Tokenize source code line
//            string token_str = get_next_token(row, head_pos, tail_pos);
////            if (tail_pos !=LABEL)
////                current_token = OPCODE;
//            if IS_COMMENT(token_str.front()) {                                          //if comment,store and break out
//                new_row[COMMENT] = row.substr(tail_pos, NOT_FOUND /*row.length()*/);
//                break;
//            }
//            if (current_token == (MAX_COLUMNS-1))                                          //check for max tokens.
//                throw file_parse_exception("Too many tokens on line: " + to_string(line_tokens.size() + 1));
//
//            else if (tail_pos == LABEL && current_token == LABEL && !is_valid_label(token_str))   //check for invalid label
//                throw file_parse_exception("Invalid label on line: " + to_string(line_tokens.size()  + 1));
//            else
//                new_row[current_token++] = token_str;
//
//            find_next_token(row, head_pos, tail_pos );
//        }
//        line_tokens.push_back(new_row);
//        line_number++;
//    }