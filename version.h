#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "13";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.11;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1619;
	static const long REVISION = 8876;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3668;
	#define RC_FILEVERSION 1,2,1619,8876
	#define RC_FILEVERSION_STRING "1, 2, 1619, 8876\0"
	static const char FULLVERSION_STRING[] = "1.2.1619.8876";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 319;
	

}
#endif //VERSION_h
