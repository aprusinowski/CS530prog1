#include "file_parser.h"

#define DELIMITER " "
#define MAX_FIELDS 3
#define IS_COMMENT(C) (C == '*' || C == ';')
#define NOT_NEWLINE(S)  (S->compare("\r") != 0)

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
    std::vector<row>::iterator r_it;
    row::iterator t_it;
    unsigned int i = 1;

    for (r_it = tokens.begin(); r_it != tokens.end(); ++r_it) {
        cout << left << i++ << " ";
        for (t_it = r_it->begin(); t_it != r_it->end(); ++t_it)
            cout << left << (*t_it) << "\t\t";
        cout << endl;
    }
    cout << endl;
}

string file_parser::get_token(unsigned int col, unsigned int row){

    if(row > (tokens.size()-1))
        throw file_parse_exception("Row number "+ to_string(row)+" out of bounds.\n");
    if(col > (tokens[row].size()-1))
        throw file_parse_exception("Row, Column number "+ to_string(row)+","+to_string(col)+" out of bounds.\n");

    return (tokens[row-1])[col];
}


void file_parser::tokenize_lines()
{
    unsigned int field;
    string::size_type pos, last_pos, end_quote;

    for (row::iterator it = contents.begin() ; it != contents.end(); ++it) {
        field = 0;
        row new_row(4,"");
        last_pos = it->find_first_not_of(DELIMITER, 0);
        pos = it->find_first_of(DELIMITER, last_pos);

        if(last_pos > 0 && field == 0)
            field++;

        if NOT_NEWLINE(it){

            while (string::npos != pos || string::npos != last_pos) {

                string temp_string(it->substr(last_pos, pos - last_pos));
                if(temp_string.compare("\r")==0) break;

                else if IS_COMMENT(temp_string.front()) {
                    new_row[3] = it->substr(last_pos, it->length());
                    break;
                }
                else if(field == MAX_FIELDS)
                    throw file_parse_exception("Too many tokens on line: "+ to_string(number_of_lines + 1));

                else if (temp_string.at(0) == '\'' && pos < ULONG_MAX) {
                    end_quote = it->find_first_of('\'', pos);
                    temp_string = it->substr(last_pos, end_quote - 1);
                    new_row[field++] = temp_string;
                    pos = last_pos + end_quote;

                }else if (last_pos == 0 && field == 0 && !is_valid_label(temp_string))
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

    if (isalpha(s.front()) || s.front() == '#') {

        if (s.back() == '\r')
            s = s.substr(0, s.size() - 2);
        if (s.back() == ':')
            s = s.substr(1, s.size() - 2);
        else
            s = s.substr(1);
        for (char c: s)
            if (!isalnum(c) && c != '\r')
                return false;

        return true;
    } else
        return false;


}
