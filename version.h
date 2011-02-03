#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char  _DATE[] = "04";
	static const char  _MONTH[] = "02";
	static const char  _YEAR[] = "2011";
	static const char  _UBUNTU_VERSION_STYLE[] = "11.02";
	
	//Software Status
	static const char  _STATUS[] = "preAlpha";
	static const char  _STATUS_SHORT[] = "pre-a";
	
	//Standard Version Type
	static const long  _MAJOR = 0;
	static const long  _MINOR = 0;
	static const long  _BUILD = 11295;
	static const long  _REVISION = 11295;
	
	//Miscellaneous Version Types
	static const long  _BUILDS_COUNT = 16176;
	#define  _RC_FILEVERSION 0,0,11295,11295
	#define  _RC_FILEVERSION_STRING "0, 0, 11295, 11295\0"
	static const char  _FULLVERSION_STRING[] = "0.0.11295.11295";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long  _BUILD_HISTORY = 0;
	

}
#endif //VERSION_H
