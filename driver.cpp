#include "file_parser.h"
#include <iomanip>

#define CONTINUE(R) (R=='Y' || R=='y')

int main(int argc, char **argv) {

    char resp;
    bool errors = false;

    if (argc != 2) {
        cout << "Put the filename as the only argument" << endl;
        return 1;
    }

    do {

        file_parser *parser = new file_parser(argv[1]);

        try {
            parser->read_file();
        }
        catch (file_parse_exception &e) {
            errors = true;
            cout << e.getMessage() << endl;
        }
        if (!errors) {


        }
        parser->print_file();
        cout << "Lines processed: "<< parser->size()<<endl;

        cout << "Run Again (Y/N): ";
        cin >> resp;
        delete parser;

    } while CONTINUE(resp);
    return 0;
}
