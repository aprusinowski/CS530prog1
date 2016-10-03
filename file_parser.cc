//
//
//

#include "file_parser.h"

file_parser::file_parser(string f_name)
{
    filename = f_name;
    number_of_lines = 0;
}

void file_parser::read_file(){

    ifstream input;
    string line;
    input.open(filename.c_str(), ios::in);
    if(!input) throw file_parse_exception("Could not open file");

    while(!input.eof()){
        getline(input, line);
        contents.push_back(line);
        number_of_lines++;
    }
    input.close();
    tokenize_lines();

}

void file_parser::print_file(){
    std::vector<row>::iterator r_it;
    std::vector<string>::iterator t_it;

    for (r_it = tokens.begin() ; r_it != tokens.end(); ++r_it)
    {
        for (t_it = (*r_it).begin() ; t_it != (*r_it).end(); ++t_it)
        {
            cout << (*t_it)<<"\t";
        }
        cout<<endl;

    }

    std::cout << '\n';

}

// returns the token found at (row, column).  Rows and columns
// are zero based.  Returns the empty string "" if there is no
// token at that location. column refers to the four fields
// identified above.
string file_parser::get_token(unsigned int col, unsigned int row)
{
    string token ="";

    if(row > (tokens.size()-1))     return token;
    if(col > (tokens[row].size()-1))return token;

    token = (tokens[row])[col];

    return token;

}

void file_parser::tokenize_lines()
{
    const string delim = " ";
    string::size_type pos, last_pos;

    row *new_row;// = new row();

    for (std::vector<string>::iterator it = contents.begin() ; it != contents.end(); ++it) {
        new_row = new row();

        last_pos = (*it).find_first_not_of(delim, 0);
        pos = (*it).find_first_of(delim, last_pos);

        while(string::npos != pos || string::npos!=last_pos){
            new_row->push_back((*it).substr(last_pos, pos-last_pos));

            last_pos = (*it).find_first_not_of(delim, pos);
            pos = (*it).find_first_of(delim, last_pos);


        }
        if((*new_row->begin()).compare("\r"))
            tokens.push_back(*new_row);
    }

}