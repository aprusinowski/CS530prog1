
#include "file_parser.h"

file_parser::file_parser(string f_name) {
    filename = f_name;
    number_of_lines = 0;
}

void file_parser::read_file() {
    string line;
    ifstream f_input(filename.c_str());
    if (!f_input.is_open())
        throw file_parse_exception("Could not open file: " + filename);
    while (getline(f_input, line))
        contents.push_back(line);
    number_of_lines = contents.size();
    f_input.close();
    tokenize_lines();
}

void file_parser::print_file() {
    for (rowVect row: tokens) {
        printf("%-2s\t %-2s\t %-2s\t %s \n", row[LABEL].c_str(), row[OPCODE].c_str(), row[OPERAND].c_str(),
               row[COMMENT].c_str());
    }
}

string file_parser::get_token(unsigned int row, unsigned int col) {
    if (row > (tokens.size()))
        throw file_parse_exception("Row number " + to_string(row) + " out of bounds. ");
    if (col > MAX_COLUMNS)
        throw file_parse_exception("Row, Column number " + to_string(row) + "," + to_string(col) + " out of bounds. ");
    return (tokens[row])[col];
}

void file_parser::tokenize_lines() {
    char charary[] = " \t\n\r";
    unsigned int field;
    string::size_type pos, last_pos, end_quote;
    //Iterate over the rows
    for (string row: contents) {
        field = LABEL;
        rowVect new_row(MAX_COLUMNS, "");
        last_pos = row.find_first_not_of(DELIMITER, 0);
        pos = row.find_first_of(DELIMITER, last_pos);

        if (last_pos > 0 && field == LABEL)
            field = OPCODE;
        //Tokenize row
        while (string::npos != pos || string::npos != last_pos) {
            string token_str(row.substr(last_pos, pos - last_pos));
            //if token is a comment, store and break out of loop
            if IS_COMMENT(token_str.front()) {
                new_row[COMMENT] = row.substr(last_pos, row.length());
                break;
            }
            //if read in max number of fields already throw an exception since the token is not a comment
            if (field == MAX_FIELDS)
                throw file_parse_exception("Too many tokens on line: " + to_string(tokens.size() + 1));
                //process tokens containing chars in single quotes
            else if (token_str.front() == SINGLE_QUOTE && pos < ULONG_MAX) {
                end_quote = row.find_first_of(SINGLE_QUOTE, pos);
                token_str = row.substr(last_pos, end_quote - last_pos);
                string::size_type eot = row.find_first_of(" \n\r\t", end_quote);
                token_str = token_str += row.substr(end_quote, eot-end_quote);
                new_row[field++] = token_str;
                pos = (string::npos == eot)? eot : end_quote+(eot-end_quote);
             //check for invalid label
            } else if (last_pos == 0 && field == LABEL && !is_valid_label(token_str))
                throw file_parse_exception("Invalid label on line: " + to_string(tokens.size()  + 1));
            else
                //store token in the next field position. If label, truncate to 8 chars max
                new_row[field++] = token_str;
            //find start and end of the next token on line
            last_pos = row.find_first_not_of(DELIMITER, pos);
            pos = row.find_first_of(DELIMITER, last_pos);
        }
        tokens.push_back(new_row);
    }
}
/*remove colon at the back and the first char since
      it was already checked with INVALID_LABEL_START    */
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
