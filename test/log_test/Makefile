log_test: log_test.cpp liblog.so
	g++ log_test.cpp -I../../src/ -llog -L./ -g -o log_test
liblog.so:
	g++ ../../src/log.cpp -I../../src/ -fPIC -shared -c -o liblog.so
clean:
	$(RM) log_test liblog.so
