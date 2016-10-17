#include "file_parser.h"
#include <iomanip>

#define CONTINUE(R) (R=='Y' || R=='y')

int main(int argc, char **argv) {
//    unsigned int c, r;
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

//            cout << left << setw(14) << "    LABELS"
//                 << left << setw(10) << "OPCODES"
//                 << left << setw(10) << "OPERANDS"
//                 << left << setw(10) << "COMMENTS"
//                 << endl;
//            cout << "========================================================================================"
//                 << endl;
            parser->print_file();
            cout << endl;


//            do {
//                cout << "Check row: ";
//                cin >> r;
//                cout << "Column: ";
//                cin >> c;
//
//                try {
//                    cout << "Token at location is: " << parser->get_token(r,c) << endl << endl;
//                }
//                catch (file_parse_exception &e) {
//                    cout << e.getMessage() << endl;
//                }
//                cout << "Check another location? (y/n): ";
//                cin >> resp;
//            } while CONTINUE(resp);
        }
        cout << "Lines processed: "<< parser->size()<<endl;
        cout << "Run Again (Y/N): ";
        cin >> resp;
        delete parser;
//        errors = false;
    } while CONTINUE(resp);
    return 0;
}
