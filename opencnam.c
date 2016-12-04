#include "opencnam.h"

/**
 * Lookup the specified phone number on OpenCNAM
 */
struct string 
do_lookup(char *pn, const char *base_uri, const char *sid, const char *auth_token)
{
	CURL *hnd;
	CURLcode ret;

	char *uri;
	uri = malloc(strlen(base_uri) + strlen(pn));
	strcpy(uri, base_uri);
	strcat(uri, pn);

	/* Use the Authorization Header - never authenticate with the query string ! */
	char *userpwd;
	userpwd = malloc(strlen(sid) + 1 + strlen(auth_token));
	strcpy(userpwd, sid);
	strcat(userpwd, ":");
	strcat(userpwd, auth_token);

	struct string result;
	init_string(&result);

	hnd = curl_easy_init();
	if (hnd) {
		
		curl_easy_setopt(hnd, CURLOPT_URL, uri);
		curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(hnd, CURLOPT_USERPWD, userpwd);
		curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 5);
		curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

		/* Store the result */
		curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &result);

		/* Perform the GET request */
		ret = curl_easy_perform(hnd);

		/* Check for errors */
		if (ret != CURLE_OK) {
			fprintf(stderr, "lookup failed: %s\n", curl_easy_strerror(ret));
		}
		
		/* Check for known HTTP error codes */
		long http_code = 0;
		curl_easy_getinfo(hnd, CURLINFO_HTTP_CODE, &http_code);

		switch (http_code) {

		case 400:
			fprintf(stderr, "Bad Request - The phone number or an argument isn't valid [400]\n\n");
			break;

		case 401:
			fprintf(stderr, "Unathorized - missing authentication invalid credentials  [401]\n\n");
			break;

		case 402:
			fprintf(stderr, "Payment required - the account does not have enough funds [402]\n\n");
			break;

		case 404:
			fprintf(stderr, "Not found [404]\n\n");
			break;
		}

		/* Cleanup */
		curl_easy_cleanup(hnd);
		hnd = NULL;

		free(userpwd);
		free(uri);
	}
	return result;
}



int 
main(int argc, char **argv)
{
	/* Read the SID, Auth Token, etc from opencnam.ini */
	configuration config;
	if (ini_parse("opencnam.ini", handler, &config) < 0) {
		printf("Can't load 'opencnam.ini'\n");
		return 1;
	}
	
	// printf("Config loaded from 'opencnam.ini': sid=%s, auth_token=%s\n", config.sid, config.auth_token);
	
		if (strcmp(config.sid, "SID") == 0 || strcmp(config.auth_token, "AUTH_TOKEN") == 0) {
		printf("You must add your OpenCNAM account details to 'opencnam.ini'\n");
		return (4);
	}
	
	/* Get the phone number from the command line args */
	char *ph_num = NULL;
	int	c;
	while ((c = getopt(argc, argv, "p:h")) != -1)
		switch (c) {
		case 'p':
			ph_num = optarg;
			break;
		case 'h':
			usage();
			return (0);
		case '?':
			fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			break;
		default:
			usage();
			return (2);
		}

	/* Check for insufficient number of command line args */
	if (argc < 3) {
		usage();
		return (3);
	}
	
	/* Do the lookup */
	struct string response = do_lookup(ph_num, config.base_uri, config.sid, config.auth_token);
	
	printf("response=[%s]\n", response.ptr);

	free(response.ptr);
	return 0;
}
