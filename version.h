#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.12";
	
	//Software Status
	static const char STATUS[] = "preAlpha";
	static const char STATUS_SHORT[] = "pre-a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 11094;
	static const long REVISION = 11094;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 15707;
	#define RC_FILEVERSION 0,0,11094,11094
	#define RC_FILEVERSION_STRING "0, 0, 11094, 11094\0"
	static const char FULLVERSION_STRING[] = "0.0.11094.11094";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
