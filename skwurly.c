#include "skwurly.h"

/// jank to enable mempcpy on mac
#ifndef mempcpy
	static inline char* mempcpy(void* d, const char* s, unsigned long l) {
		// printf("%s : %s : %d\n", d, s, l);
		return (char*) memcpy(d,s,l)+l;
	}
#endif

#define MAX_PARAMS 32

static inline int str_compare(const char* a, const char* b) {
	for(;*a == *b; ++a, ++b)
		if (*a == '\0' || *b == '\0') return 0;
	return *a - *b;
}

char* url_sort(const char* url) {

	// building an array with twice the number of params
	// and working from middle of array
	// to remove shuffling requirement on prepending
	const char* params[MAX_PARAMS*2];
	unsigned short param_length[MAX_PARAMS*2];

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

	// keep track of if the url is already in sorted order
	int sorted = 1;

	// set initial param
	params[HEAD] = ++url;

	int LAST_PARAM = HEAD;

	// and find the others
	for (; *url; ++url) {

		if (*url == '&') {

			if (TAIL-HEAD == MAX_PARAMS) {
				return (char*) orig_url;
			}

			// set the length of the previous param
			param_length[LAST_PARAM] = url - params[LAST_PARAM];

			// grab a pointer to the param and the first char for easy action
			const char* p = url+1;

			// if it sorts to before head just insert it and move HEAD to it
			if (str_compare(params[HEAD], p) > -1) {
				sorted = 0;
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

			// if we're here we're not sorted
			sorted = 0;

			// move tail to spot after end
			int i = TAIL++;
			params[TAIL] = params[i];
			param_length[TAIL] = param_length[i];
			// shuffle elements up starting at tail until we hit the right spot
			// and set tail to new length
			int prev = i-1;
			for (; i > HEAD && str_compare(params[prev], p) > -1; --i, --prev) {
				params[i] = params[prev];
				param_length[i] = param_length[prev];
			}

			// insert the new value
			params[i] = p;
			LAST_PARAM = i;

		}
	}

	// sorted will be 1 if theres only one param or if the url is already sorted
	if (sorted == 1 || TAIL-HEAD < 1) {
		return (char*) orig_url;
	}

	// set the last length now that we're at the end of the string
	param_length[LAST_PARAM] = url - params[LAST_PARAM];

	// initialize our return value
	char* sorted_url = (char*) malloc(url - orig_url+1);

	// determine length of chars up to and including '?'
	int uri_len = (cursor+1) - orig_url;

	// copy in chars up to and including '?'
	cursor = mempcpy(sorted_url, orig_url, uri_len);

	// build the sorted url
	int iter = TAIL-HEAD;

	for (;iter--; ++HEAD) {
		cursor = mempcpy(cursor, params[HEAD], param_length[HEAD]);
		// printf("%d: %s\t%s\n", param_length[HEAD], params[HEAD], cursor);
		*cursor++ = '&';
	}
	cursor = mempcpy(cursor, params[HEAD], param_length[HEAD]);
	*cursor = '\0';

	return sorted_url;
}
