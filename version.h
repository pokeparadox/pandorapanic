#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.09;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 1291;
	static const long REVISION = 7078;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3085;
	#define RC_FILEVERSION 1,1,1291,7078
	#define RC_FILEVERSION_STRING "1, 1, 1291, 7078\0"
	static const char FULLVERSION_STRING[] = "1.1.1291.7078";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 391;
	

}
#endif //VERSION_h
