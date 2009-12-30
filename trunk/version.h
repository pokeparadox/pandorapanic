#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.12;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 2083;
	static const long REVISION = 11384;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4507;
	#define RC_FILEVERSION 1,3,2083,11384
	#define RC_FILEVERSION_STRING "1, 3, 2083, 11384\0"
	static const char FULLVERSION_STRING[] = "1.3.2083.11384";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 383;
	

}
#endif //VERSION_h
