#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.09;
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 1357;
	static const long REVISION = 7405;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3199;
	#define RC_FILEVERSION 1,2,1357,7405
	#define RC_FILEVERSION_STRING "1, 2, 1357, 7405\0"
	static const char FULLVERSION_STRING[] = "1.2.1357.7405";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 57;
	

}
#endif //VERSION_h
