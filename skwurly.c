#include "skwurly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int param_compare(const void* a, const void* b) {

	char aa = *(*(param*)a).start;
	char bb = *(*(param*)b).start;

	if (aa == bb) {
		return strcmp((*(param*)a).start+1, (*(param*)b).start+1);
	}
	return aa - bb;

}

char* url_sort(const char* url){

	char* cursor = NULL;
	int len = 0, count = 0;

	// scan to first '?', mark it and break
	while (*url != '\0') {
		// count the string length
		// can use this to get back to first char in url
		++len;

		if (*url == '?') {
			cursor = (char*) url++;
			count = 1;
			break;
		}

		// next char
		++url;
	}

	// scan the rest of the string for '&' tokens
	while (*url != '\0') {
		// count the string length
		// can use this to get back to first char in url
		++len;

		// count the number of param delimeters
		if (*url == '&') {
			++count;
		}

		// next char
		++url;
	}

	// return the url if there is not enough to sort or empty string or never found a '?'
	if (count < 2 || len == 0 || cursor == NULL) {
		return strdup(url-len);
		// return (char*) url-len;
	}

	// initialize the params offset array
	param params[count];

	// reuse count to track what param we're on
	count = 0;

	// set first value beginning after cursor position
	params[count].start = ++cursor;
	params[count].length = 0;

	// scan the rest of the string to build params set
	while (*cursor != '\0') {

		// note the start of the param
		// cursor advances each time
		if (*cursor++ == '&') {
			params[++count].start = cursor;
			params[count].length = 0;
		}
		// count the length of the param
		else {
			++params[count].length;
		}

	}

	// sort the array in place in reverse order for
	// easier iteration later on
	qsort(params, count+1, sizeof(param), param_compare);

	// initialize our return value
	char* sorted_url = (char*) malloc(len + 1);

	// restart cursor at beginning of new string
	cursor = sorted_url;

	// write the url up to the first '?'
	// reset pointer to beginning of string
	url -= len;
	while (*url != '?' && *url != '\0') {
		*cursor++ = *url++;
	}
	*cursor++ = '?';

	// build the sorted url
	for (int i = 0; i <= count; ++i) {

		while (params[i].length-- > 0) {

			// write the value
			// advance the cursor
			*cursor++ = *params[i].start++;

		}

		// add the delimiter in manually
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
		buf[strcspn(buf, "\n")] = '\0';
		if (buf[0] == '\0')
		    continue;
		out = url_sort(buf);
		free(out);
		printf("Orig: %s\nSort: %s\n\n",buf, out );
	}
	return 0;
}
