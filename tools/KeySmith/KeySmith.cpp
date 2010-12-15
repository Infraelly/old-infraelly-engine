#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>



int random(int lowerBound, int upperBound){
    if(lowerBound == upperBound){ return lowerBound; }
    //switch values if needed
    if( upperBound < lowerBound ){
        int tmp = upperBound;
        upperBound = lowerBound;
        lowerBound = tmp;
    }

    //increase upper bound to make it posible to get upperbound itselfs
    ++upperBound;

    //get the total negative offset (wrt 0)
    int deltaZ = 0;
    if( lowerBound < 0 ) {
        deltaZ += lowerBound;
        lowerBound = 0;
    }
    if( upperBound < 0 ){
        deltaZ += upperBound;
        upperBound = 0;
    }

    if( deltaZ < 0 ){ deltaZ *= -1; };

    //generate a new number (positive)
    int num = rand() % (upperBound+deltaZ);

    // rand() makes numbers btween 0-RAND_MAX
    //  we cant use zeros... so we will roll again
    while( num < lowerBound ){
        num = rand() % (upperBound+deltaZ);
    }

    //make number negative if nesecary
    if( deltaZ > 0 ){
        num -= deltaZ;
    }

    return num;
}
std::string upperCase(std::string input){
	if (!input.empty() ){
		for (unsigned int i = 0; i < input.length(); ++i){
			input.at(i) = toupper(input.at(i));
		}
	}
	return input;
}
bool isalpha(const std::string& text){
    for( unsigned int i = 0; i < text.length(); ++i){
        if( !isalpha( int(text.at(i)) ) ){ return 0; };
    }
    return 1;
}






using namespace std;


//  isk = infraelly security key
//#define TEST_CHECK


#ifdef TEST_CHECK
#include "checkTest.hpp"
#endif

int main(int argc, char *argv[]){
#ifdef TEST_CHECK
    if( isk::checkTest() ) { cout << "test success\n"; };
    return 0;
#endif

    //if command line options
    if( argc <= 1 ){
        cout << "Usage: keySmith.exe [keyFunctionName]...[keyFunctionName]...etc" << endl;
        return 1;
    }

    //size of key
    const int keySize = 512;
    //seed with version
    srand( time(NULL) );



    // produce keys for all args
    for(int i = 1; i < argc; ++i){
        //if ar if not all alpha characters
        if( !isalpha( argv[i] ) ){
            cout << "Argument " << i << " \"" << argv[i] << "\" must be comprised of alphabet Characters" << endl;
            continue;
        }
        //make the check function file
        string filename = argv[i];
        filename.at(0) = toupper(filename.at(0));
        filename += ".hpp";
        filename.insert(filename.begin(), 'k');
        filename.insert(filename.begin(), 'c');
        filename.insert(filename.begin(), 'e');
        filename.insert(filename.begin(), 'h');
        filename.insert(filename.begin(), 'c');
        fstream file( filename.c_str(), fstream::out );
        if( !file.good() ){
            cout << "Unable to make file: " << filename << endl;
            return 1;
        }
        //make the key file
        string keyFilename = argv[i];
        keyFilename += ".isk";
        fstream keyFile( keyFilename.c_str(), fstream::out );
        if( !keyFile.good() ){
            cout << "Unable to make file: " << keyFilename << endl;
            file.close();
            return 1;
        }
        //make the key
        string key;
        key.reserve( keySize );
        for(int j = 0; j < keySize; ++j){
            char ch = random(40,126);
            if(ch!='\\')
            key.push_back( char(ch) );
        }
        //store the keyfile
        keyFile << key;
        //close key file
        keyFile.close();


        string functionName(argv[i]);
        functionName.at(0) = toupper(functionName.at(0));
        //store he comparer function header
        file << "#ifndef HPP_ISK_" << upperCase(argv[i]) << "\n";
        file << "#define HPP_ISK_" << upperCase(argv[i]) << "\n";
        file << "namespace isk{" << "\n";
        file << "#include <string>" << "\n";
        file << "#include <fstream>" << "\n";
        file << "using namespace std;" << "\n";
        file << "static string " << upperCase(argv[i]) << "_KEY(\"" << key << "\");\n";
        file << "bool check" << functionName << "(){";
        file << "ifstream kf(\"" << keyFilename << "\",fstream::in);";
        file << "if(!(kf.good()&&kf.is_open())){";
        file << "cout<<\"Invalid key file\"<<endl;";
        file << "kf.close();";
        file << "return 0;";
        file << "}";
        file << "string ik;ik.reserve(" << keySize << ");";
        file << "char ch = kf.get();while(ch!=EOF){";
        file << "ik.push_back(ch);ch=kf.get();}";
        file << "if(ik!=" << upperCase(argv[i]) << "_KEY){";
        file << "cout<<\"Invalid key file\"<<endl;";
        file << "kf.close();";
        file << "return 0;";
        file << "}else{return 1;}";
        file << "}\n";
        file << "}\n";
        file << "#endif" << "\n";
        file.close();

        cout << "Success: " << argv[i] << endl;
    }
    cout << "Finished" << endl;
    return 0;
}
