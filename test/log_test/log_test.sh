g++ -c ../../src/log.cpp -I../../src/ -o ./log.o
ar rs liblog.a log.o
g++ -g log_test.cpp -I../../src/ -llog -L./ -o log_test
