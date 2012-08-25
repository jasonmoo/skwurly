#include <string.h>
#include <stdio.h>


void sort_params(int** params, int count, const char* url) {
	int swap[2];

	int max, i, j;

	// simple insertion sort
	for(i = 1; i != count; ++i) {
		for(j = i; j > 0; --j) {

			// determine shortest length to compare
			max = url[params[j][1]] > url[params[j-1][1]] ? url[params[j-1][1]] : url[params[j][1]];

			if (memcmp(&url[params[j][1]], &url[params[j-1][1]], max) == 1) {
				swap = params[j];
				params[j] = params[j - 1];
				params[j - 1] = swap;
			}

		}
	}

}

char* url_sort(const char* url){

	const char* i = url;
	int len = 0, count = 0;

	// iniatial scan to see what needs to be done
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
	int params[count][2];

	// reset pointer to beginning of string
	// reuse count to track what param we're on
	i = url;
	count = -1;
	int in = 0, c = 0;

	// scan to build params set
	while (*i != '\0') {

		// note the start of the param
		if (*i == '&' || *i == '?') {
			params[++count][0] = c+1;
			params[count][1] = -1;
			in = 1;
		}

		// count the length of the param
		if (in == 1) {
			params[count][1] += 1;
		}

		// next char
		++i;
		++c;
	}

	// sort the array in place
	sort_params(&params, count, url);

	// initialize our return value
	char[len] sorted_url = *url;

	// create a cursor and advance it to where
	// we want to start rewriting the query string
	char* cursor = &sorted_url;
	while (*cursor != '?' && *cursor != '\0')
		++cursor;
	++cursor;

	// build the sorted url
	for (int i = 0; i <= count; ++i) {

		while (params[i][1]-- > 0) {

			// write the value
			*cursor = url[params[i][0]++];

			// advance the cursor
			cursor++;

		}

		// add the delimiter back in manually
		*cursor = '&';

		// advance the cursor
		cursor++;

	}

	// replace last char (extra &) with null terminator
	*cursor = '\0';

	return &sorted_url;
}




int main(int argc, char** argv){
	char* out = NULL;
	out = url_sort("http://localhost/test?ddd=444&bbb=222&ccc=333&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?ccc=333&aaa=111&ddd=444&bbb=222");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=333&ddd=444&aaa=111");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&ccc=333&ddd=444&aaa=111&bbbb=1234&ccccccc=0");
	printf("%s\n",out );
	out = url_sort("http://localhost/test?bbb=222&cbcc=333&ddd=444&dddf=444&aaa=111&bbbb=1234&ccccccc=0&zaaaa=89&ccc=987987");
	printf("%s\n",out );
	return 0;
}
