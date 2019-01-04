#pragma once

#if defined(_MSC_VER)
    //  Microsoft
	#ifndef EXPORT
		#define EXPORT __declspec(dllexport)
	#endif
	#ifndef IMPORT
		#define IMPORT __declspec(dllimport)
	#endif
#elif defined(__GNUC__)
	//  GCC
	#ifndef EXPORT
		#define EXPORT __attribute__((visibility("default")))
	#endif
	#ifndef IMPORT
		#define IMPORT 
	#endif
#else
	#ifndef EXPORT
		#define EXPORT
		#pragma warning Unknown dynamic link import/export semantics.
	#endif
	#ifndef IMPORT
		#define IMPORT 
		#pragma warning Unknown dynamic link import/export semantics.
	#endif
#endif