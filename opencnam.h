#include <ctype.h>
#include <stdio.h>
#include <curl/curl.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "include/ini.h"

struct string {
	char *ptr;
	size_t len;
};

typedef struct {
	const char *sid;
	const char *auth_token;
	const char *base_uri;
} configuration;


struct string do_lookup(char *pn, const char *base_uri, const char *sid, const char *auth_token);


/**
 * Initialize the string struct
 */
void 
init_string(struct string *s)
{
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

/**
 * Write the HTTP output to the string struct s
 */
size_t 
writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size * nmemb;
}


/*
 * The handler reads the INI file fills out the configuration struct
 */
static int 
handler(void *user, const char *section, const char *name, const char *value)
{
	configuration  *pconfig = (configuration *) user;

	if (strcmp(name, "sid") == 0) {
		pconfig->sid = strdup(value);
	} else if (strcmp(name, "auth_token") == 0) {
		pconfig->auth_token = strdup(value);
	} else if (strcmp(name, "base_uri") == 0) {
		pconfig->base_uri = strdup(value);
	} else {
		return 2;	/* unknown section/name, error */
	}
	return 1;
}

/*
 * Print usage information
 */
void 
usage(void)
{
	puts("Usage:\n"
	     " -p PH_NUM      The phone number to search for in E164 format\n"
	     " -h             Show this help screen\n"
	);
}
