#include "skwurly.h"

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
