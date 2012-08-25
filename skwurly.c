#include "skwurly.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int param_compare(const void* a, const void* b) {

		return strcmp((*(param*)a).start, (*(param*)b).start);

}

char* url_sort(const char* url){

	const char* i = url;
	int len = 0, count = 0;

	// initial scan to see what needs to be done
	while (*i != '\0') {
		// count the string length
		++len;

		// count the number of param delimeters
		if (*i == '&' || *i == '?')
			++count;

		// next char
		++i;
	}

	// return the url if there is not enough to sort or empty string
	if (count < 2 || len == 0) {
		return (char*) url;
	}

	// initialize the params offset array
	param params[count];

	// reset pointer to beginning of string
	// reuse count to track what param we're on
	i = url;
	count = -1;
	int in = 0;

	// scan to build params set
	while (*i != '\0') {

		// note the start of the param
		if (*i == '&' || *i == '?') {
			params[++count].start = i+1;
			params[count].length = -1;
			in = 1;
		}

		// count the length of the param
		if (in == 1) {
			params[count].length += 1;
		}

		// next char
		++i;
	}

	// sort the array in place
	qsort(params, count+1, sizeof(param), param_compare);

	// initialize our return value
	char *sorted_url = (char*) malloc(len + 1);
	strcpy(sorted_url, url);

	// create a cursor and advance it to where
	// we want to start rewriting the query string
	char* cursor = strchr(sorted_url, '?');

	// build the sorted url
	for (int i = 0; i <= count; ++i) {

		// advance the cursor
		// on first iteration places it
		// on char after '?''
		++cursor;

		while (params[i].length-- > 0) {

			// write the value
			// advance the cursor
			*cursor++ = *params[i].start++;

		}

		// add the delimiter back in manually
		*cursor = '&';

	}

	// replace last char (extra &) with null terminator
	*cursor = '\0';

	return sorted_url;
}




int main(int argc, char** argv){
	char* out = NULL;
	out = url_sort("http://localhost/test?1");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?2&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ddd=&bbb=222&3&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ddd=444&bbb=222&ccc=333&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ccc=333&ccc=111&ddd=444&bbb=222");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=333&ddd=444&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=ßß∞§•øø&ddd=444&aaa=111&bbbb=1234&ccccccc=0");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&cbcc=333&ddd=444&dddf=444&aaa=111&bbbb=1234&ccccccc=0&zaaaa=89&ccc=987987");
	printf("%s\n",out );
	return 0;
}
