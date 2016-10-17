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
    line_count = 0;
}
/**
 *   reads the source file, storing the information is some
     auxiliary data structure you define in the private section.
     Throws a file_parse_exception if an error occurs.
     if the source code file fails to conform to the above
     specification, this is an error condition.
 */
void file_parser::read_file() {
    rowVect  file_contents;
    string line;
    ifstream f_input(filename.c_str());
    if (!f_input.is_open())
        throw file_parse_exception("Could not open file: " + filename);
    while (getline(f_input, line))
        file_contents.push_back(line);
    line_count = file_contents.size();
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
 * @return
 */
string file_parser::get_token(unsigned int row, unsigned int col) {
    if (row > (line_tokens.size()))
        throw file_parse_exception("Row number " + to_string(row) + " out of bounds. ");
    if (col > MAX_COLUMNS)
        throw file_parse_exception("Row, Column number " + to_string(row) + "," + to_string(col) + " out of bounds.");
    return (line_tokens[row])[col];
}
/**
 *
 * @param file_contents
 */
void file_parser::tokenize_lines(rowVect& file_contents) {
    unsigned int token_count;
    string::size_type pos, last_pos/*, end_quote*/;
    for (string row: file_contents) {
        token_count = LABEL;
        rowVect new_row(MAX_COLUMNS, "");
        last_pos = row.find_first_not_of(DELIMITER, 0);
        pos = row.find_first_of(DELIMITER, last_pos);

        if (last_pos !=LABEL && token_count == LABEL)                                   //advance token count if leading whitespace
            token_count = OPCODE;

        while (string::npos != pos || string::npos != last_pos) {                       //Tokenize source code line
            string token_str(row.substr(last_pos, pos - last_pos));
            if IS_COMMENT(token_str.front()) {                                          //if comment,store and break out
                new_row[COMMENT] = row.substr(last_pos, row.length());
                break;
            }
            if (token_count == (MAX_TOKENS-1))                                          //check for max tokens.
                throw file_parse_exception("Too many tokens on line: " + to_string(line_tokens.size() + 1));

            else if (pos != string::npos && token_str.find(SINGLE_QUOTE)!=string::npos){//tokens with single quotes
//                end_quote = row.find_first_of(SINGLE_QUOTE, pos);
//                end_quote = (end_quote == string::npos)?pos:end_quote;
//                token_str = row.substr(last_pos, end_quote - last_pos);                 //update token up to closing quote
//                string::size_type eot = row.find_first_of(DELIMITER, end_quote);        //find end of token
//                new_row[token_count++] = (token_str += row.substr(end_quote, eot-end_quote)); //add remaining characters
                new_row[token_count++] = process_quotes(token_str, row, pos, last_pos);
                //pos = (string::npos == eot)? string::npos: end_quote+(eot-end_quote);

            } else if (last_pos == LABEL && token_count == LABEL && !is_valid_label(token_str))   //check for invalid label
                throw file_parse_exception("Invalid label on line: " + to_string(line_tokens.size()  + 1));
            else
                new_row[token_count++] = token_str;
            last_pos = row.find_first_not_of(DELIMITER, pos);                           //find start of next token
            pos = row.find_first_of(DELIMITER, last_pos);                               //find end of next token
        }
        line_tokens.push_back(new_row);
    }
}

string file_parser::process_quotes(string& token_str, string& row, string::size_type& pos, string::size_type& last_pos ){
    string::size_type end_quote;
    end_quote = row.find_first_of(SINGLE_QUOTE, pos);
    end_quote = (end_quote == string::npos)?pos:end_quote;
    token_str = row.substr(last_pos, end_quote - last_pos);                 //update token up to closing quote
    string::size_type eot = row.find_first_of(DELIMITER, end_quote);        //find end of token
    pos = (string::npos == eot)? string::npos: end_quote+(eot-end_quote);
}

//remove colon at the back and the first char since it was already checked with INVALID_LABEL_START
/**
 *
 * @param s
 * @return
 */
bool file_parser::is_valid_label(string s) {
    if INVALID_LABEL_START(s)
        return false;
    if (s.back() == ':')
        s = CHOP_FRONT_AND_NEWLINE(s);
    else
        s = CHOP_FRONT(s);
    for (char c: s)
        if INVALID_LABEL_CHAR(c)
            return false;
    return true;
}

