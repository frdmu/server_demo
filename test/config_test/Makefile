TARGET := config_test
LIBS := libconfig.so
SOURCE := config_test.cpp

$(TARGET): $(SOURCE) $(LIBS) 
	$(CXX) $(SOURCE) -g -I../../src/ -lconfig -L./ -lyaml-cpp -L/usr/local/lib/ -o $@ 
$(LIBS):
	$(CXX) ../../src/config.cpp -c -I../../src/ -fPIC -shared -o $@ 
clean:
	$(RM) $(LIBS) $(TARGET) 
