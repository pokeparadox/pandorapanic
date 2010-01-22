#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.01;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 2147;
	static const long REVISION = 11722;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4629;
	#define RC_FILEVERSION 1,4,2147,11722
	#define RC_FILEVERSION_STRING "1, 4, 2147, 11722\0"
	static const char FULLVERSION_STRING[] = "1.4.2147.11722";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 47;
	

}
#endif //VERSION_h
