#include "file_parser.h"

#define DELIMITER      " "
#define NEWLINE_C      '\r'
#define SINGLE_QUOTE   '\''
#define MAX_FIELDS     3

#define IS_COMMENT(C)              (C == '*' || C == ';')
#define NOT_NEWLINE(S)             (S->compare("\r") != 0)
#define VALID_LABEL_BEGIN(S)       (isalpha(S.front()) || s.front() == '#')
#define CHOP_NEWLINE_CHAR(S)       (S.substr(0, S.size() - 2))
#define CHOP_FRONT_AND_NEWLINE(S)  (S = S.substr(1, S.size() - 2))
#define CHOP_FRONT(S)              (S.substr(1))
#define INVALID_LABEL_CHAR(C)      (!isalnum(C) && C != NEWLINE_C)

file_parser::file_parser(string f_name) {
    filename = f_name;
    number_of_lines = 0;
}

void file_parser::read_file() {
    ifstream input;
    string line;
    input.open(filename.c_str(), ios::in);

    if (!input)
        throw file_parse_exception("Could not open file: " + filename);

    while (getline(input, line))
        contents.push_back(line);
    input.close();
    tokenize_lines();
}


void file_parser::print_file() {
    unsigned int line_num = 0;
    allRowsVect::iterator r_it;
    rowVect::iterator t_it;
    cout <<left;

    for (r_it = tokens.begin(); r_it != tokens.end(); ++r_it) {
        cout <<setw(4)<<to_string(++line_num);
        for (t_it = r_it->begin(); t_it != r_it->end(); ++t_it) {

            std::string::size_type found = (*t_it).find_last_of('\r');

            if(found !=  string::npos)
                (*t_it) = (*t_it).substr(0, found);

            if((*t_it).length() < 10)
                  cout <<setw(10)<<(*t_it);
            else
                cout<<(*t_it);
        }
        cout << endl;
    }

}

string file_parser::get_token(unsigned int col, unsigned int row) {
    if (row > (tokens.size() - 1))
        throw file_parse_exception("Row number "+to_string(row)+" out of bounds. \r");
    if (col > (tokens[row].size() - 1))
        throw file_parse_exception("Row, Column number "+to_string(row)+","+to_string(col)+" out of bounds. \r");
    return (tokens[row - 1])[col];
}


void file_parser::tokenize_lines() {
    unsigned int field;
    string::size_type pos, last_pos, end_quote;

    for (rowVect::iterator it = contents.begin(); it != contents.end(); ++it) {
        field = 0;
        rowVect new_row(4, "");
        last_pos = it->find_first_not_of(DELIMITER, 0);
        pos = it->find_first_of(DELIMITER, last_pos);

        if (last_pos > 0 && field == 0)
            field++;

        if NOT_NEWLINE(it) {

            while (string::npos != pos || string::npos != last_pos) {

                string temp_string(it->substr(last_pos, pos - last_pos));
                if (temp_string.compare("\r") == 0) break;

                else if IS_COMMENT(temp_string.front()) {
                    new_row[3] = it->substr(last_pos, it->length());
                    break;
                } else if (field == MAX_FIELDS)
                    throw file_parse_exception("Too many tokens on line: " + to_string(number_of_lines + 1));

                else if (temp_string.at(0) == SINGLE_QUOTE && pos < ULONG_MAX) {
                    end_quote = it->find_first_of(SINGLE_QUOTE, pos);
                    temp_string = it->substr(last_pos, end_quote - 1);
                    new_row[field++] = temp_string;
                    pos = last_pos + end_quote;

                } else if (last_pos == 0 && field == 0 && !is_valid_label(temp_string))
                    throw file_parse_exception("Invalid label on line: " + to_string(number_of_lines + 1));
                else
                    new_row[field++] = temp_string;

                last_pos = it->find_first_not_of(DELIMITER, pos);
                pos = it->find_first_of(DELIMITER, last_pos);
            }
        }
        tokens.push_back(new_row);
        number_of_lines++;
    }
}


bool file_parser::is_valid_label(string s) {

    if VALID_LABEL_BEGIN(s){
        if (s.back() == NEWLINE_C)
            s = CHOP_NEWLINE_CHAR(s);
        if (s.back() == ':')
            s = CHOP_FRONT_AND_NEWLINE(s);
        else
            s = CHOP_FRONT(s);
        for (char c: s)
            if INVALID_LABEL_CHAR(c) //(!isalnum(c) && c != NEWLINE_C)
                return false;

        return true;
    } else
        return false;


}

