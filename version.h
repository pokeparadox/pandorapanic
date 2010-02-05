#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.02;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 2205;
	static const long REVISION = 12057;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4734;
	#define RC_FILEVERSION 1,4,2205,12057
	#define RC_FILEVERSION_STRING "1, 4, 2205, 12057\0"
	static const char FULLVERSION_STRING[] = "1.4.2205.12057";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 105;
	

}
#endif //VERSION_h
