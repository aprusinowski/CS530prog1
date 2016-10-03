//
//
//

#include <cstring>
#include "file_parser.h"

int main(int argc, char **argv)
{
    unsigned int c,r;

    if(argc != 2){
        cout << "Put the filename as the only argument" << endl;
        return 1;
    }

    file_parser parser(argv[1]);


    parser.read_file();
    parser.print_file();

    for(int i=0;i<10;i++){
        cout<<"Row:";
        cin >> r;
        cout<<"Col:";
        cin >> c;
        cout<<parser.get_token(c,r)<<endl;
    }


    return 0;
}
