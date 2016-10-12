
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








//#include "file_parser.h"
//
//file_parser::file_parser(string filename) {
//    file_name = filename;
//    number_of_lines = 0;
//}
//
//file_parser::~file_parser() {}
//
//void file_parser::read_file() {
//    ifstream input; //opens an ifstream to read from file
//    string line;
//    row r;
//    input.open(file_name.c_str(), ios_base::in); // opens the file to read from
//    if (!input) // if file couldn't be opened throw exception
//        throw file_parse_exception("couldn't open file, please check arguments");
//    while (!input.eof()) { // read the file while there is still data
//        getline(input, line); // get a line of data and store it in line.
//        token_lines.push_back(line); // add it to the vector of lines.
//        r = tokenize(line);     // tokenize the line into 1-4 tokens
//        number_of_lines++;      // increment the number of lines tokenized
//        file_rows.push_back(r);      // add the tokenized row to the vector of file_rows
//        if(number_of_lines == 37)
//            int a = 5;
//    }
//    input.close();              // close the file
//}
//
//void file_parser::print_file() {
//    for (row r: file_rows) {
//        printf("%-2s\t %-2s\t %-2s\t %s \n", (r.label).c_str(), r.opcode.c_str(), r.operand.c_str(),
//               r.comments.c_str());
//
//    }
//}
//
//string file_parser::get_token(unsigned int row, unsigned int column) {
//    if (row > file_rows.size() - 1)
//        throw file_parse_exception("The provided row is out of the range of the file, please try again");
//    if (column > 4)
//        throw file_parse_exception(
//                "The provided column is out of the range of the file, please choose a number between 0-4");
//    switch (column) {
//        case 0:
//            return file_rows[row].label;
//            break;
//        case 1:
//            return file_rows[row].opcode;
//            break;
//        case 2:
//            return file_rows[row].operand;
//            break;
//        case 3:
//            return file_rows[row].comments;
//            break;
//    }
//    return " ";
//}
//
//file_parser::row file_parser::tokenize(string str) {
//    const string &delimiters = "  \t";
//    row r;
//    string::size_type lastpos = str.find_first_not_of(delimiters, 0); // Skip delimiters at beginning.
//    string::size_type pos = str.find_first_of(delimiters, lastpos); // Find first "delimiter".
//    int num_tokens = 0; //number of tokens on the line;
//    while (string::npos != pos || string::npos != lastpos) {
//        if (IS_COMMENT(str[lastpos])) {
//            add_comment(r, str, lastpos, num_tokens);
//            break;
//        }
//        else if (IS_LABEL(str[pos - 1]) || IS_LABEL(*str.rbegin()) || IS_INCLUDE(str[lastpos]))  //if label
//            add_label(r, str, lastpos, pos, num_tokens, delimiters);
//        else if (IS_SPACE(str[lastpos - 1])) { // if opcode and operand
//            add_opcode(r, str, lastpos, pos, num_tokens, delimiters);
//            if (IS_COMMENT(str[lastpos])) {
//                add_comment(r, str, lastpos, num_tokens);
//                break;
//            }
//            add_operand(r, str, lastpos, pos, num_tokens, delimiters);
//        }
//        check_invalid_tokens(num_tokens, r);
//        lastpos = str.find_first_not_of(delimiters, pos);
//        pos = str.find_first_of(delimiters, lastpos);
//    }
//    return r;
//}
//
//void file_parser::check_label(string str, size_t pos) {
//    string::iterator it = str.begin();
//    if (IS_SPACE(str[0])) {
//        stringstream ss;
//        ss << "the label must to be on column 0, line " << number_of_lines + 1;
//        string message = ss.str();
//        throw file_parse_exception(message);
//    }
//    if (!isalpha(str[0])) {
//        stringstream ss;
//        ss << "The 1st letter of the label has to be a letter, on line " << number_of_lines + 1;
//        string message = ss.str();
//        throw file_parse_exception(message);
//    }
//    while (!IS_LABEL(*it)) {
//        if (!isalnum(*it)) {
//            stringstream ss;
//            ss << "The lable has to be alphanumeric, on line " << number_of_lines + 1;
//            string message = ss.str();
//            throw file_parse_exception(message);
//        }
//        it++;
//    }
//
//}
//
//void file_parser::check_invalid_tokens(int tokens, row r) {
//    if ((r.label.empty() && r.comments.empty() && tokens >= 3)) {
//        stringstream ss;
//        ss << "invalid number of tokens on line  " << number_of_lines + 1;
//        string message = ss.str();
//        throw file_parse_exception(message);
//    }
//}
//
//void file_parser::add_comment(row &r, string str, string::size_type lastpos, int &num_tokens) {
//    r.comments = str.substr(lastpos);
//    num_tokens++;
//}
//
//void file_parser::add_label(file_parser::row &r, string str, string::size_type &lastpos, string::size_type &pos, int num_tokens,
//                            const string &delimiters) {
//    if(IS_INCLUDE(str[lastpos])){
//        add_include(r,str,lastpos,pos,num_tokens,delimiters);
//    }
//    else {
//        check_label(str, pos);
//        r.label = str.substr(lastpos, pos - lastpos);
//        num_tokens++;
//    }
//}
//
//void
//file_parser::add_opcode(row &r, string str, string::size_type &lastpos, string::size_type &pos, int &num_tokens, const
//string &delimiters) {
//    r.opcode = str.substr(lastpos, pos - lastpos);
//    num_tokens++;
//    lastpos = str.find_first_not_of(delimiters, pos);
//    pos = str.find_first_of(delimiters, lastpos);
//}
//
//void
//file_parser::add_operand(row &r, string str, string::size_type &lastpos, string::size_type &pos, int &num_tokens, const
//string &delimiters) {
//    if (IS_Quotes(str[lastpos])) {
//        pos = str.find_first_of("\'", lastpos + 1);
//        r.operand = str.substr(lastpos, pos - lastpos);
//        lastpos = str.find_first_not_of(delimiters, pos);
//        pos = str.find_first_of(delimiters, lastpos);
//        r.operand += str.substr(lastpos, pos - lastpos);
//
//    } else if (lastpos <= str.length())
//        r.operand = str.substr(lastpos, pos - lastpos);
//    num_tokens++;
//    cout<<r.operand<<endl;
//}
//
//void file_parser::add_include(row &r, string str, string::size_type &lastpos, string::size_type &pos, int &num_tokens,
//                              const string &delimiters) {
//    r.label = str.substr(lastpos, pos - lastpos);
//    lastpos = str.find_first_not_of(delimiters, pos);
//    pos = str.find_first_of(delimiters, lastpos);
//    r.opcode = str.substr(lastpos, pos - lastpos);
//}
