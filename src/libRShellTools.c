/* GPL-3 License or later, see LICENCE.txt */
/* https://www.gnu.org/licenses/gpl-3.0.en.html */
/*
 * libRShellArgs.c - functions to process shell arguments
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

#include "Processing.h"

static int _RODTOOLS_SFLAGS_ERROR_;

int _rodtools_sflags_GetError()
{
	return _RODTOOLS_SFLAGS_ERROR_;
}

void _rodtools_sflags_FlagsInit(_rodtools_sflags_Flags* ShellFlags)
{
	memset(ShellFlags, 0, sizeof(*ShellFlags));
	ShellFlags->maxgiven_files = _RODTOOLS_SFLAGS_NFILENAMES_;
}

void _rodtools_sflags_SetNFlags(_rodtools_sflags_Flags* ShellFlags, size_t NumFlags)
{
	ShellFlags->nflags = NumFlags;
	ShellFlags->flags = calloc(NumFlags, sizeof(*ShellFlags->flags));
	ShellFlags->need_arg = calloc(NumFlags, sizeof(*ShellFlags->need_arg));
	ShellFlags->given_args = calloc(NumFlags, sizeof(*ShellFlags->given_args));
	ShellFlags->functions = calloc(NumFlags, sizeof(*ShellFlags->functions));
	ShellFlags->given_files = calloc(_RODTOOLS_SFLAGS_NFILENAMES_ , sizeof(*ShellFlags->given_files));
}

void _rodtools_sflags_FlagsFree(_rodtools_sflags_Flags* ShellFlags)
{
	free(ShellFlags->flags);
	free(ShellFlags->need_arg);
	free(ShellFlags->given_args);
	free(ShellFlags->functions);
	free(ShellFlags->given_files); 
}

int _rodtools_sflags_CheckString(char* string1, char* string2)
{
	size_t j = 0;
	while(1)
	{
		if( string1[j] != string2[j] )
			break;
		else if ( (!string1[j]) && (!string2[j]) )
			return 0;
		j++;
	}
	return -1;
}

size_t _rodtools_sflags_MatchFlag(_rodtools_sflags_Flags* ShellFlags, char* arg)
{
	for(size_t i = 0; i < ShellFlags->nflags; i++)
	{
		if(! _rodtools_sflags_CheckString(ShellFlags->flags[i], arg))
			return i;
	}
	return -1;
}

int _rodtools_sflags_ProcessFlags(_rodtools_sflags_Flags* ShellFlags, int argc, char** argv)
{
	/*	Processing flags	*/
	size_t args_count = 1;
	while(argc > args_count)
	{
		/* Check if argument is a file name, if so store it */
		if(argv[args_count][0] != '-')
		{
			/* Allocate more space for file name pointers, if need be, increments of 10 */
			if(ShellFlags->ngiven_files == ShellFlags->maxgiven_files)
			{
				if(reallocarray(ShellFlags->given_files, ShellFlags->maxgiven_files + 10, sizeof(*ShellFlags->given_files)) == NULL)
				{
					printf("reallocarray failed! Can't allocate more space for given file names ptrs\n");
					_RODTOOLS_SFLAGS_ERROR_ = _RODTOOLS_SFLAGS_ERROR_REALLOCARRAY_;
					return _RODTOOLS_SFLAGS_ERROR_REALLOCARRAY_;
				}
				ShellFlags->maxgiven_files += 10;
			}

			ShellFlags->given_files[ShellFlags->ngiven_files] = argv[args_count];
			ShellFlags->ngiven_files++;
			goto PROCESS_FLAGS_END;
		}

		/* Check if argument is an allowed flag */
		size_t fmatch_index = _rodtools_sflags_MatchFlag(ShellFlags, argv[args_count]);
		if(fmatch_index == -1)
		{
			printf("Unknown flag: \"%s\"\n",argv[args_count]);
			_RODTOOLS_SFLAGS_ERROR_ = _RODTOOLS_SFLAGS_ERROR_UNKNOWNFLAG_;
			return _RODTOOLS_SFLAGS_ERROR_UNKNOWNFLAG_;
		}

		/* Check if flag requires an argument, if so, store argument pointer */
		if(ShellFlags->need_arg[fmatch_index])
		{
			args_count++;

			/* Check if provided argument is a flag, argument to a a flag being a flag?? Don't think so chief */
			if(argv[args_count][0] == '-')
			{
				printf("%s flag requires an argument!\n",argv[args_count-1]);
				_RODTOOLS_SFLAGS_ERROR_ = _RODTOOLS_SFLAGS_ERROR_NOARG_;
				return _RODTOOLS_SFLAGS_ERROR_NOARG_;
			}

			ShellFlags->given_args[fmatch_index] = argv[args_count];
		}

		/* Call upon flag function */
		ShellFlags->functions[fmatch_index](ShellFlags->given_args[fmatch_index]);

		PROCESS_FLAGS_END:
		args_count++;
	}

	return 0;
}
