#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "02";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.10;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1364;
	static const long REVISION = 7459;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3209;
	#define RC_FILEVERSION 1,2,1364,7459
	#define RC_FILEVERSION_STRING "1, 2, 1364, 7459\0"
	static const char FULLVERSION_STRING[] = "1.2.1364.7459";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 64;
	

}
#endif //VERSION_h
