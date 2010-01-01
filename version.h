#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.01;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 2097;
	static const long REVISION = 11448;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4539;
	#define RC_FILEVERSION 1,3,2097,11448
	#define RC_FILEVERSION_STRING "1, 3, 2097, 11448\0"
	static const char FULLVERSION_STRING[] = "1.3.2097.11448";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 397;
	

}
#endif //VERSION_h
