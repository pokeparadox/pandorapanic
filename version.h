#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.05;
	
	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 4;
	static const long BUILD = 2348;
	static const long REVISION = 12804;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4977;
	#define RC_FILEVERSION 1,4,2348,12804
	#define RC_FILEVERSION_STRING "1, 4, 2348, 12804\0"
	static const char FULLVERSION_STRING[] = "1.4.2348.12804";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 248;
	

}
#endif //VERSION_h
