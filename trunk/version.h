#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.10;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1572;
	static const long REVISION = 8579;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3585;
	#define RC_FILEVERSION 1,2,1572,8579
	#define RC_FILEVERSION_STRING "1, 2, 1572, 8579\0"
	static const char FULLVERSION_STRING[] = "1.2.1572.8579";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 272;
	

}
#endif //VERSION_h
