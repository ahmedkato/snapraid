/*
 * Copyright (C) 2011 Andrea Mazzoleni
 *
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SUPPORT_H
#define __SUPPORT_H

/****************************************************************************/
/* path */

/**
 * Copies a path limiting the size.
 * Aborts if too long.
 */
void pathcpy(char* dst, size_t size, const char* src);

/**
 * Concatenates a path limiting the size.
 * Aborts if too long.
 */
void pathcat(char* dst, size_t size, const char* src);

/**
 * Concatenates a path limiting the size.
 * Aborts if too long.
 */
void pathcatc(char* dst, size_t size, char c);

/**
 * Imports a path limiting the size.
 * In Windows all the backslash are converted to the C standard of forward slash.
 * Aborts if too long.
 */
void pathimport(char* dst, size_t size, const char* src);

/**
 * Exports a path limiting the size.
 * In Windows all the C slashes are converted to the Windows backslash.
 * Aborts if too long.
 */
void pathexport(char* dst, size_t size, const char* src);

/**
 * Prints a path.
 * Aborts if too long.
 */
void pathprint(char* dst, size_t size, const char* format, ...);

/**
 * Ensures the presence of a terminating slash, if it isn't empty.
 * Aborts if too long.
 */
void pathslash(char* dst, size_t size);

/**
 * Cuts everything after the latest slash.
 */
void pathcut(char* dst);

/**
 * Compare two paths.
 * In Windows it's case insentive and assumes \ equal at /.
 */
int pathcmp(const char* a, const char* b);

/****************************************************************************/
/* filesystem */

/**
 * Creates all the ancestor directories if missing.
 * The file name, after the last /, is ignored.
 */
int mkancestor(const char* file);

/**
 * Changes the modification time of an open file.
 */
int fmtime(int f, int64_t mtime_sec, int mtime_nsec);

/****************************************************************************/
/* memory */

/**
 * Return the size of the allocated memory.
 */
size_t malloc_counter(void);

/**
 * Safe malloc.
 * If no memory is available, it aborts.
 */
void* malloc_nofail(size_t size);

/**
 * Safe cmalloc.
 * If no memory is available, it aborts.
 */
void* calloc_nofail(size_t count, size_t size);

/**
 * Safe strdup.
 * If no memory is available, it aborts.
 */
char* strdup_nofail(const char* str);

/**
 * Helper for printing an error about a failed allocation.
 */
void malloc_fail(size_t size);

#endif
