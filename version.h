#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.12";
	
	//Software Status
	static const char STATUS[] = "preAlpha";
	static const char STATUS_SHORT[] = "pre-a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 11071;
	static const long REVISION = 11071;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 15647;
	#define RC_FILEVERSION 0,0,11071,11071
	#define RC_FILEVERSION_STRING "0, 0, 11071, 11071\0"
	static const char FULLVERSION_STRING[] = "0.0.11071.11071";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
