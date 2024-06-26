#ifndef PAGEDIR_H
#define PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <errno.h>
#include <string.h>

#include <stdbool.h>
//struct webpage_t;

#include "crawler.h"

/**
 * Initializes the page directory so that webpages can be saved in it.
 * Returns true if initialization succeeded, false otherwise.
 */
bool pagedir_init(const char *pageDirectory);

/**
 * Saves a given webpage to a given page directory with a given document ID.
 */
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID);

#endif
