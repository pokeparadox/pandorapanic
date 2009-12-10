#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.12;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 1904;
	static const long REVISION = 10396;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4175;
	#define RC_FILEVERSION 1,3,1904,10396
	#define RC_FILEVERSION_STRING "1, 3, 1904, 10396\0"
	static const char FULLVERSION_STRING[] = "1.3.1904.10396";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 204;
	

}
#endif //VERSION_h
