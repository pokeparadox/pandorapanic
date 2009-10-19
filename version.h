#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.10;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1534;
	static const long REVISION = 8367;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3516;
	#define RC_FILEVERSION 1,2,1534,8367
	#define RC_FILEVERSION_STRING "1, 2, 1534, 8367\0"
	static const char FULLVERSION_STRING[] = "1.2.1534.8367";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 234;
	

}
#endif //VERSION_h
