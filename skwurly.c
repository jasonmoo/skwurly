#include "skwurly.h"

#define MAX_PARAMS 32

char* url_sort(const char* url) {

	// building an array with twice the number of params
	// and working from middle of array
	// to remove shuffling requirement on prepending
	const char* params[MAX_PARAMS*2];
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

	// and find the others
	for (; *url != '\0'; ++url) {

		if (*url == '&') {

			if (TAIL-HEAD == MAX_PARAMS) {
				return (char*) orig_url;
			}

			// grab a pointer to the param and the first char for easy action
			const char* p = url+1;
			register int pc = *p;

			// if it sorts to before head just insert it and move HEAD to it
			int c = *params[HEAD] - pc;
			if (c > 0 || (c == 0 && strcmp(params[HEAD], p) > -1)) {
				params[--HEAD] = p;
				continue;
			}

			// if it sorts after TAIL just insert it and move TAIL to it
			c = *params[TAIL] - pc;
			if (c < 0 || (c == 0 && strcmp(params[TAIL], p) < 1)) {
				params[++TAIL] = p;
				continue;
			}

			// move tail to spot after end
			params[TAIL+1] = params[TAIL];

			// shuffle elements up starting at tail until we hit the right spot
			// and set tail to new length
			int i = TAIL++;
			for (c = *params[i-1] - pc; i > HEAD && c > 0 || (c == 0 && strcmp(params[i-1], p) > -1); c = *params[--i-1] - pc) {
				params[i] = params[i-1];
			}

			// insert the new value
			params[i] = p;

		}
	}

	// or less than 2 params found
	if (TAIL-HEAD < 1) {
		return (char*) orig_url;
	}

	// initialize our return value
	char* sorted_url = (char*) malloc(url - orig_url+1);

	// determine length of chars up to and including '?'
	int uri_len = (cursor+1) - orig_url;

	// reuse cursor now that it's no longer needed to keep a pointer to '?'
	cursor = (char*) sorted_url;

	// copy in chars up to and including '?'
	memcpy(sorted_url, orig_url, uri_len);

	// move the cursor after the '?'
	cursor += uri_len;

	// build the sorted url
	for (; HEAD <= TAIL; ++HEAD) {

		const char* end = strchr(params[HEAD], '&');

		if (end == NULL) {
			int param_len = strlen(params[HEAD]);
			memcpy(cursor, params[HEAD], param_len);
			cursor += param_len;
			*cursor++ = '&';
		}
		else {
			int param_len = end - params[HEAD]+1;
			memcpy(cursor, params[HEAD], param_len);
			cursor += param_len;
		}

	}

	// replace last char (extra &) with null terminator
	*(cursor-1) = '\0';

	return sorted_url;
}
