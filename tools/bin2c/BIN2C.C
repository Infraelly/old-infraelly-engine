#include <cctype>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string upperCase(std::string input){
	if (!input.empty() ){
		for (unsigned int i = 0; i < input.length(); ++i){
			input.at(i) = toupper(input.at(i));
		}
	}
	return input;
}


const unsigned int line = 12;
const unsigned int bufferSize = 1;

int main( int argc, char *argv[]){
    //  timer
    time_t startTime = time( NULL );
    //  whether to verbose output or not
    bool verbose = 0;

    switch( argc ){
        default:
        case 1:
        case 2:
            cout << " - <<< BIN2C v1.0 >>> by Christian Padovano - \n";
            cout << "Tampered with and ported to C++ by Tony Huynh\n" << endl;
            cout << "USAGE: bin2C  <INPUT filename> <OUTPUT base-name> [-v]\n" << endl;
            cout << "  <INPUT> = File to be opened and converted" << endl;
            cout << "  <OUTPUT > = without extension '.h' it will be added by program." << endl;
            cout << "  [-v] = Optional. Verbose output" << endl;
            cout << "  eg. bin2c image.png image_png" << endl;
            cout << "    This will produce two files. image_png.h and image_png.c" << endl;
            cout << "    The data structure will be image_png[]" << endl;
            cout << "    The size of image_png[] will be stored in IMAGE_PNG_LEN" << endl;
            exit(EXIT_SUCCESS);
        case 3:
            break;
        case 4:
            if( upperCase(argv[3]) == "-V" ){
                verbose = 1;
                cout << "Verbose output enabled" << endl;
            }
            break;
    }

    //  The input file
    ifstream source;
    //  The output file (header)
    ofstream destHeader;
    //  The output file (source)
    ofstream destSource;
    //  Used when reading in
    char buffer;
    //  The name of the header output file
    string outputHeaderFilename = argv[2];
    outputHeaderFilename += ".h";
    //  The name of the source output file
    string outputSourceFilename = argv[2];
    outputSourceFilename += ".c";
    //  Length of structure
    long int length = 0;


    if(verbose){ cout << "Opening " << argv[1] << endl; }
    //  Try to open the input file
    source.open( argv[1], fstream::binary);
    //  Check to see if it has opened
    if( source.bad() || !source.is_open() ){
      cout << "ERROR: Can't find source file \"" << argv[1] << "\"" << endl;
      source.close();
      exit(EXIT_FAILURE);
    }

    if(verbose){ cout << "Opening " << outputHeaderFilename << endl; }
    //  Try to open the onput file (header)
    destHeader.open( outputHeaderFilename.c_str() );
    //  Check to see if it has opened
    if( destHeader.bad() || !destHeader.is_open() ){
      cout << "ERROR: Can't open destination file \"" << outputHeaderFilename << "\"" << endl;
      source.close();
      destHeader.close();
      exit(EXIT_FAILURE);
    }

    if(verbose){ cout << "Opening " << outputSourceFilename << endl; }
    //  Try to open the onput file (source)
    destSource.open( outputSourceFilename.c_str() );
    //  Check to see if it has opened
    if( destSource.bad() || !destSource.is_open() ){
      cout << "ERROR: Can't open destination file \"" << outputHeaderFilename << "\"" << endl;
      source.close();
      destHeader.close();
      destSource.close();
      exit(EXIT_FAILURE);
    }


    //  All files open succesfuly


    if(verbose){ cout << "Writing source file" << endl; }
    //  Write the header of source file
    destSource << "unsigned char " << argv[2] << "[] = \n{";
    //set to output to hex

    //  Write the contents of Source file
    while( !source.eof() ){
        //put newline and tab 4 spaces
        destSource << "\n   ";
        for (unsigned int i = 0; ((i <= line) && (!destSource.eof()) ); ++i){
            source.read( &buffer, bufferSize );
            if ( !destSource.eof() ){
                destSource << "0x" << hex << int(buffer) << ", ";
                length += bufferSize;
            } else {
                break;
            }
        }
    }
    //  write file footer
    destSource << "0x0\n};\n" << endl;
    //  check if file is good
    if( destSource.good() ){
        if(verbose){ cout << "Sucessfully written source file" << endl; }
    } else {
        cout << "Failed to write source file" << endl;
        return EXIT_FAILURE;
    }


    if(verbose){ cout << "Writing header file" << endl; }
    //  Write contents of header file
    destHeader << "#ifndef H_RESOURCE_" << upperCase( argv[2] ) << "\n";
    destHeader << "#define H_RESOURCE_" << upperCase( argv[2] ) << "\n";
    destHeader << "\n";
    destHeader << "const unsigned int " << upperCase( argv[2] ) << "_LEN = " << length << ";\n";
    destHeader << "extern unsigned char " << argv[2] << "[];\n";
    destHeader << "\n";
    destHeader << "#endif" << endl;
    if( destHeader.good() ){
        if(verbose){ cout << "Sucessfully written header" << endl; }
    } else {
        cout << "Failed to write header" << endl;
        return EXIT_FAILURE;
    }



    //report
    cout << "Source file: " << argv[1] << endl;
    cout << "Size of file: " << dec << length << endl;
    cout << "Include file: " << outputHeaderFilename << endl;
    cout << "Source file: " << outputSourceFilename << endl;
    cout << "Data structure name: " << argv[2] << "[]" << endl;
    cout << "Size of structure via: " << upperCase( argv[2] ) << "_LEN" << endl;

    cout << "Total Time: " << time(NULL) - startTime << "s" << endl;



    //  Close files
    source.close();
    destSource.close();
    destHeader.close();
    return EXIT_SUCCESS;
}
