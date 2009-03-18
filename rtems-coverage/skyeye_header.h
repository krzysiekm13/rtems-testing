/*
 *  $Id$
 */

/*! @file skyeye_header.h
 *  @brief skyeye_header Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */

#ifndef __Skyeye_Header_h
#define __Skyeye_Header_h

#define MAX_DESC_STR 32
typedef struct prof_header_s{
	/* the version of header file */
	int ver;
	/* The length of header */
	int header_length;
	int prof_start;
	int prof_end;
	/* The description info for profiling file */
	char desc[MAX_DESC_STR];
}prof_header_t;

#endif
