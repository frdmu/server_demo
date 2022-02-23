thread_test: thread_test.cpp libthread.so
	g++ thread_test.cpp -g -I../../src/ -lthread -L./ -lpthread -o thread_test
libthread.so: ../../src/thread.cpp
	g++ ../../src/thread.cpp -c -I../../src/ -fPIC -shared -o libthread.so 
clean:
	$(RM) libthread.so thread_test
