#include "skwurly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARAMS 32

char* url_sort(const char* url) {

	// building an array with twice the number of params
	// and working from middle of array
	// to remove shuffling requirement on prepending
	const char* params[MAX_PARAMS*2];
	const char* orig_url = url;

	// scan to first '?', mark it and break
	for (;*url != '?' && *url != '\0';++url);

	// set the cursor at '?' or '\0'
	char* cursor = (char*) url++;

	// param count
	int count = 0;

	// building 2x array to make head/tail sorted insertion O(N)
	// start insertion at middle
	// [NULL,NULL,NULL,<val>,<val>,NULL,NULL,NULL]
	int HEAD = MAX_PARAMS;
	int TAIL = MAX_PARAMS;

	// set initial param
	params[HEAD] = url;

	// and find the others
	for (; *url != '\0'; ++url) {

		if (*url == '&') {

			if (count++ == MAX_PARAMS) {
				return (char*) orig_url;
			}

			const char* p = url+1;

			// if it sorts to before head just insert it and move HEAD to it
			if (*params[HEAD] - *p > -1 || strcmp(params[HEAD], p) > -1) {
				params[--HEAD] = p;
				continue;
			}

			// if it sorts after TAIL just insert it and move TAIL to it
			if (*params[TAIL] - *p < -1 || strcmp(params[TAIL], p) < -1) {
				params[++TAIL] = p;
				continue;
			}

			// // move tail on place after end
			params[TAIL+1] = params[TAIL];

			// shuffle elements up starting at tail until we hit the right spot
			int i = TAIL;
			for (; i > HEAD && (*params[i-1] - *p > -1 || strcmp(params[i-1], p) > -1); --i) {
				params[i] = params[i-1];
			}

			// insert the new value
			params[i] = p;

			// set tail to new length
			++TAIL;

		}
	}

	// if there's no '?'
	// or less than 2 params found
	if (cursor == '\0' || count < 1) {
		return (char*) orig_url;
	}

	// initialize our return value
	char* sorted_url = (char*) malloc(url - orig_url);

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
		while (*params[HEAD] != '\0' && *params[HEAD] != '&') {
			*cursor++ = *params[HEAD]++;
		}
		*cursor++ = '&';
	}

	// replace last char (extra &) with null terminator
	*(cursor-1) = '\0';

	return sorted_url;
}


int main(int argc, char** argv){
	char buf[2048];
	char* out;
	while(fgets(buf,2048,stdin) != NULL){
		//chopping off newlines
		buf[strlen(buf)-1] = '\0';
		if (buf[0] == '\0')
		    continue;
		out = url_sort(buf);
		// printf("Orig: %s\nSort: %s\n\n",buf, out );

		if (buf != out) {
			free(out);
		}
	}
	return 0;
}
