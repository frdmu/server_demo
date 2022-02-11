TARGET := config_test
LIBS := libconfig.so
SOURCE := config_test.cpp

$(TARGET): $(SOURCE) $(LIBS) 
	$(CXX) $(SOURCE) -I../../src/ -lconfig -L./ -o $@ 
$(LIBS):
	$(CXX) ../../src/config.cpp -c -I../../src/ -fPIC -shared -o $@ 
clean:
	$(RM) $(LIBS) $(TARGET) 
