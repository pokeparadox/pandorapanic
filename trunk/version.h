#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.11;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1669;
	static const long REVISION = 9125;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3783;
	#define RC_FILEVERSION 1,2,1669,9125
	#define RC_FILEVERSION_STRING "1, 2, 1669, 9125\0"
	static const char FULLVERSION_STRING[] = "1.2.1669.9125";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 369;
	

}
#endif //VERSION_h
