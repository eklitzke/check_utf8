check_utf8: check_utf8.cc
	g++ $< -std=c++11 -o $@

.PHONY: clean
clean:
	rm -f check_utf8
