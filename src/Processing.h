/* GPL-3 License or later, see LICENCE.txt */
/* https://www.gnu.org/licenses/gpl-3.0.en.html */
/*
 * Processing.h - header for functions to process shell arguments
 * Copyright (C) 2021 Rodrigo Amaral  <rodrigo_amaral01@outlook.com>
 */

/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _RODTOOLS_SARGS_PROCESSING_H
#define _RODTOOLS_SARGS_PROCESSING_H

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#define _RODTOOLS_SFLAGS_ERROR_REALLOCARRAY_ -1
#define _RODTOOLS_SFLAGS_ERROR_UNKNOWNFLAG_  -2
#define _RODTOOLS_SFLAGS_ERROR_NOARG_	     -3

/* Defines the initial amount of file names that can be stored in Flags struct */
#define _RODTOOLS_SFLAGS_NFILENAMES_ 10

typedef struct
{
	/* Amount of flags */
	size_t	 nflags;

	/* Array of flags that can be processed */
	char**	 flags;

	/* boolean for if a flag accepts a following argument */
	uint8_t* need_arg;

	/* Array of pointers for the given arguments */
	char**	 given_args;

	/* Array of functions to be called once a flag has been detected and optionally the argument given. Flag argument is passed to function */
	/* If a flag requires no argument, NULL is passed to function, Funcion called must return void and accept a char* as its single argument */
	void (**functions)(char* arg);

	/* Amount of given file names and maximum allocated memory for the file name pointers */
	size_t	 ngiven_files;
	size_t	 maxgiven_files;
	
	/* Array of pointers for the given file names passed as arguments */
	char**	 given_files;

} _rodtools_sflags_Flags;



/* GetError() used to return value of _RODTOOLS_SFLAGS_ERROR_ */
int _rodtools_sflags_GetError();



/* Initialization and free functions */

/* Initializes a Flags variable */
extern void _rodtools_sflags_FlagsInit(_rodtools_sflags_Flags* ShellFlags);

/* Allocates memory for amount of NumFlags */
extern void _rodtools_sflags_SetNFlags(_rodtools_sflags_Flags* ShellFlags, size_t NumFlags);

/* Frees memory for Flags struct arrays */
extern void _rodtools_sflags_FlagsFree(_rodtools_sflags_Flags* ShellFlags);



/* Flag processing functions */

/* Checks if two null terminated strings are identical, returns 0 if comparison is successful, -1 otherwise */
extern int	_rodtools_sflags_CheckString(char* string1, char* string2);

/* returns the index for a matched flag with the provided argument */
extern size_t	_rodtools_sflags_MatchFlag(_rodtools_sflags_Flags* ShellFlags, char* arg);

/* Processes the given shell arguments */
extern int	_rodtools_sflags_ProcessFlags(_rodtools_sflags_Flags* ShellFlags, int argc, char** argv);


#endif /* _RODTOOLS_SARGS_PROCESSING_H */
