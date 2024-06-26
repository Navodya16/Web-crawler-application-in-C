#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <regex.h>

#include "url.c"
#include "pagedir.c"
#include "curl.h"


#include "crawler.h"

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);
int docId = 0;

unsigned int hash(char *url)
{
	unsigned int sum = 0;
	char *p = url;
	while (*p != '\0')
	{
		sum += *p;
		sum = (sum * (*p)) % HASH_TABLE_SIZE;
		p++;
	}

	return sum;
}

void init_hash_table(hashtable_t *H)
{
	//printf("done3");
	int i;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		H->url[i] = NULL;
	}
}

bool hash_table_insert(char *url, hashtable_t *H)
{
	//printf("done5");
	int index = hash(url);
	if (H->url[index] != NULL)
	{
		return false;
	}
	H->url[index] = url;
	return true;
}

bool hastable_lookup(char *url, hashtable_t *H)
{
	int index = hash(url);
	if (H->url[index] != NULL && strcmp(H->url[index], url) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool hash_table_delete(char *url, hashtable_t *H)
{
	int index = hash(url);
	if (H->url[index] != NULL && strcmp(H->url[index], url) == 0)
	{
		H->url[index] = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

void init_webpage_bag(bag_t *B)
{
	B->head = NULL;
}

void bag_insert(bag_t *B, const char *url, const char *html, size_t length, int depth)
{
	webpage_t *newpage = malloc(sizeof(webpage_t));
	if (newpage == NULL)
	{
		fprintf(stderr, "failed to allocate memory for the webpage_t object\n");
		exit(1);
	}
	
	newpage->depth = depth;
	
	newpage->html = malloc(strlen(html) + 1);
	if (newpage->html == NULL)
	{
	    fprintf(stderr, "failed to allocate memory for the html string\n");
	    exit(1);
	}
	newpage->url = malloc(strlen(url) + 1);
	if (newpage->url == NULL)
	{
	    fprintf(stderr, "failed to allocate memory for the url string\n");
	    exit(1);
	}
	
	strcpy(newpage->html, html);
	newpage->length = length;
	strcpy(newpage->url, url);
	newpage->next = NULL;
	if (B->head == NULL)
	{
		B->head = newpage;
	}
	else
	{
		webpage_t *current = B->head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newpage;
	}
}

int getNumRecords(bag_t *bag) {
    int count = 0;
    webpage_t *current = bag->head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}


static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth)
{
	if (*maxDepth < 0 || *maxDepth > 10)
	{
		printf("max depth is out of range\n");
		exit(1);
	}
	else
	{
		char *surl = *seedURL;
		normalizeURL(surl, surl);
		if (!isInternalURL(surl, surl))
		{
			printf("url is not internal\n");
			exit(1);
		}
		else
		{
			char *pdir = *pageDirectory;
			pagedir_init(pdir);
		}
	}
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth)
{
	hashtable_t H;
	init_hash_table(&H);
	char* normalizedSeedUrl = normalizeURL(seedURL, seedURL);
	hash_table_insert(normalizedSeedUrl, &H);
	
	bag_t B;
	init_webpage_bag(&B);
	size_t size;
	char* seedHtml = download(normalizedSeedUrl, &size);
	bag_insert(&B, normalizedSeedUrl, seedHtml, size, 0);
	
	webpage_t *current = B.head;
    while (current != NULL) 
	{
    	pagedir_save(current, pageDirectory, docId);
    	++docId;
    	if(current->depth < maxDepth)
    	{
    		pageScan(current, &B, &H);
		}
        // Move to the next webpage in the bag
        free(current);
        current = current->next;
    }
}

/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen)
{
	const char *html = page->html;
    const char *current = html;
    while ((current = strstr(current, "<a href=\""))) {
        current += strlen("<a href=\"");
        const char *url_start = current;
        const char *url_end = strchr(current, '\"');
        if (url_end == NULL) {
            fprintf(stderr, "Invalid URL format.\n");
            exit(1);
        }

        size_t url_length = url_end - url_start;
        char *url = malloc(url_length + 1);
        if (url == NULL) {
            fprintf(stderr, "Failed to allocate memory for URL.\n");
            exit(1);
        }
        strncpy(url, url_start, url_length);
        url[url_length] = '\0';

		char *seedUrl = page->url;
		url = normalizeURL(seedUrl, url);
		if(isInternalURL(seedUrl, url))
		{
			if(hash_table_insert(url, pagesSeen))
			{
				size_t size;
				char* secondHtml = download(url, &size);
				int seconddepth = page->depth + 1;
				bag_insert(pagesToCrawl, url, secondHtml, size, seconddepth);
			}
		}
        // Print the URL
        //printf("Found URL: %s\n", url);

        free(url); // Free the memory for the URL

        current = url_end + 1;
    }
}

int main(const int argc, char *argv[])
{
	int maxDep = atoi(argv[3]);
	parseArgs(argc, argv, &argv[1], &argv[2], &maxDep);
	crawl(argv[1], argv[2], atoi(argv[3]));
	
	return 0;
}


