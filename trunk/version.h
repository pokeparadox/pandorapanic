#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.06;
	
	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 2367;
	static const long REVISION = 12911;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5009;
	#define RC_FILEVERSION 1,4,2367,12911
	#define RC_FILEVERSION_STRING "1, 4, 2367, 12911\0"
	static const char FULLVERSION_STRING[] = "1.4.2367.12911";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 267;
	

}
#endif //VERSION_h
