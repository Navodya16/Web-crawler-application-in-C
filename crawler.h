#ifndef CRAWLER_H
#define CRAWLER_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "curl.c"
//#include "url.h"
//struct webpage_t;
//#include "pagedir.h"

#define HASH_TABLE_SIZE 100

typedef struct webpage_t
{
	char *url;
	char *html;
	size_t length;
	int depth;
	struct webpage_t *next;
} webpage_t;

typedef struct
{
	webpage_t *head;
} bag_t;

typedef struct
{
	char *url[HASH_TABLE_SIZE];
} hashtable_t;

#endif
