# compile string
gcc -o skwurly -O3 test.c skwurly.c

# jot is mac equiv of seq
if command -v jot > /dev/null; then
	seq="jot"
else
	seq="seq"
fi

time for i in `$seq 10`; do ./skwurly < 5Murls.txt > /dev/null ; done