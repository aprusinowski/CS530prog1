//
//
//

#include "file_parser.h"
typedef vector<string> row;

file_parser::file_parser(string f_name)
{
    filename = f_name;
    number_of_lines = 0;
}

void file_parser::read_file(){

    ifstream input;
    string line;
    input.open(filename.c_str(), ios::in);
    if(!input) throw file_parse_exception("Could not open file"); //throw exceptions in here...catch in driver

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

//put comments into index 0,3 no matter if they are only item


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
    unsigned int position = 0;
    string delim = " ";
    string::size_type pos, last_pos, end_quote;

    row *new_row;



    for (std::vector<string>::iterator it = contents.begin() ; it != contents.end(); ++it) {
        new_row = new row(4,"");
        position = 0;

        last_pos = (*it).find_first_not_of(delim, 0);
        pos = (*it).find_first_of(delim, last_pos);

        if(last_pos > 0 && position == 0)
            position++;

        if((*it).compare("\r")!= 0)
        while(string::npos != pos || string::npos!=last_pos){

            string *temp_string = new string((*it).substr(last_pos, pos-last_pos));

            if(temp_string->at(0)=='\'' && pos < ULONG_MAX) {
                end_quote = (*it).find_first_of("\'", pos);
                *temp_string = (*it).substr(last_pos, end_quote-1);
                new_row->at(position) = *temp_string;
                pos = last_pos+end_quote;
                position++;
            }

            else if(is_comment(*temp_string)){
                new_row->at(3)=(*it).substr(last_pos, (*it).length());
                break;
            }else if(last_pos == 0 && position == 0 && !is_valid_label(*temp_string)){
                //throw exception
                return;
            }
            else{
                new_row->at(position) = *temp_string;
                position++;
            }

            last_pos = (*it).find_first_not_of(delim, pos);
            pos = (*it).find_first_of(delim, last_pos);


            //To Do handle includes

        }

        tokens.push_back(*new_row);
    }
}


bool file_parser::is_comment(string s)
{
    return (s.front()=='*' || s.front()==';');
}

bool file_parser::is_valid_label(string s)
{
    if(!isalpha(s.front()))
        return false;
    if(s.back() == ':')
        s.resize(s.size()-1);

    for(char c: s){
        if(!isalnum(c))
            return false;
    }
    return true;

}
