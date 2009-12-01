#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.11;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 3;
	static const long BUILD = 1742;
	static const long REVISION = 9487;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3903;
	#define RC_FILEVERSION 1,3,1742,9487
	#define RC_FILEVERSION_STRING "1, 3, 1742, 9487\0"
	static const char FULLVERSION_STRING[] = "1.3.1742.9487";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 42;
	

}
#endif //VERSION_h
