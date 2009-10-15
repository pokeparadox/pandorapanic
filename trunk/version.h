#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.10;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1530;
	static const long REVISION = 8346;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3510;
	#define RC_FILEVERSION 1,2,1530,8346
	#define RC_FILEVERSION_STRING "1, 2, 1530, 8346\0"
	static const char FULLVERSION_STRING[] = "1.2.1530.8346";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 230;
	

}
#endif //VERSION_h
