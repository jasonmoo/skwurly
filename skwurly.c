#include "skwurly.h"

#define MAX_PARAMS 32

static inline int str_compare(const char* a, const char* b) {
	for(;*a == *b; ++a, ++b)
		if (*a == '\0') return 0;
	return *a - *b;
}

char* url_sort(const char* url) {

	// building an array with twice the number of params
	// and working from middle of array
	// to remove shuffling requirement on prepending
	const char* params[MAX_PARAMS*2];


	int param_length[MAX_PARAMS*2];

	const char* orig_url = url;

	// scan to first '?'
	url = strchr(url, '?');
	if (url == NULL) {
		return (char *) orig_url;
	}

	// set the cursor at '?'
	char* cursor = (char*) url;

	// building 2x array to make head/tail insertion O(1)
	// start insertion at middle
	// [NULL,NULL,NULL,<val>,<val>,NULL,NULL,NULL]
	int HEAD = MAX_PARAMS;
	int TAIL = MAX_PARAMS;

	// set initial param
	params[HEAD] = ++url;
	param_length[HEAD] = 0;

	int LAST_PARAM = HEAD;

	// and find the others
	for (; *url != '\0'; ++url) {

		if (*url == '&') {

			if (TAIL-HEAD == MAX_PARAMS) {
				return (char*) orig_url;
			}

			int length = url - params[LAST_PARAM];
			param_length[LAST_PARAM] = length;

			// grab a pointer to the param and the first char for easy action
			const char* p = url+1;

			// if it sorts to before head just insert it and move HEAD to it
			if (str_compare(params[HEAD], p) > -1) {
				params[--HEAD] = p;
				LAST_PARAM = HEAD;
				continue;
			}

			// if it sorts after TAIL just insert it and move TAIL to it
			if (str_compare(params[TAIL], p) < 1) {
				params[++TAIL] = p;
				LAST_PARAM = TAIL;
				continue;
			}

			// move tail to spot after end
			params[TAIL+1] = params[TAIL];
			param_length[TAIL+1] = param_length[TAIL];

			// shuffle elements up starting at tail until we hit the right spot
			// and set tail to new length
			int i = TAIL++;
			for (; i > HEAD && str_compare(params[i-1], p) > -1; --i) {
				params[i] = params[i-1];
				param_length[i] = param_length[i-1];
			}

			// insert the new value
			params[i] = p;
			LAST_PARAM = i;

		}
	}

	// set the last length now that we're at the end of the string
	int length = url - params[LAST_PARAM];
	param_length[LAST_PARAM] = length;

	// or less than 2 params found
	if (TAIL-HEAD < 1) {
		return (char*) orig_url;
	}

	// initialize our return value
	char* sorted_url = (char*) malloc(url - orig_url+1);

	// determine length of chars up to and including '?'
	int uri_len = (cursor+1) - orig_url;

	// reuse cursor now that it's no longer needed to keep a pointer to '?'
	cursor = (char*) sorted_url + uri_len;

	// copy in chars up to and including '?'
	memcpy(sorted_url, orig_url, uri_len);

	// build the sorted url
	int iter = TAIL-HEAD+1;
	for (;iter--; ++HEAD) {
		memcpy(cursor, params[HEAD], param_length[HEAD]);
		// printf("%d: %s\t%s\n", param_length[HEAD], params[HEAD], cursor);
		cursor += param_length[HEAD];
		*cursor++ = '&';

	}

	// replace last char (extra &) with null terminator
	*(cursor-1) = '\0';

	return sorted_url;
}
