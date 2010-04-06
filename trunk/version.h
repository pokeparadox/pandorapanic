#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.04;
	
	//Software Status
	static const char STATUS[] = "Release Candidate";
	static const char STATUS_SHORT[] = "rc";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 2317;
	static const long REVISION = 12625;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4947;
	#define RC_FILEVERSION 1,4,2317,12625
	#define RC_FILEVERSION_STRING "1, 4, 2317, 12625\0"
	static const char FULLVERSION_STRING[] = "1.4.2317.12625";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 217;
	

}
#endif //VERSION_h
