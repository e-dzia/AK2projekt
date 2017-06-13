CXX = /usr/bin/g++
CXXFLAGS = -Wall -Wextra -pedantic -fopenmp -std=c++11
OBJDIR = ./obj
OUTDIR = ./out

$(OUTDIR)/test: $(OBJDIR)/main.o $(OBJDIR)/TimerLinux.o $(OBJDIR)/Utils.o | directories
	$(CXX) $(CXXFLAGS) $(OBJDIR)/main.o $(OBJDIR)/TimerLinux.o $(OBJDIR)/Utils.o -o $(OUTDIR)/test

$(OBJDIR)/main.o: main.cpp | directories
	$(CXX) $(CXXFLAGS) main.cpp -c -o $(OBJDIR)/main.o

$(OBJDIR)/TimerLinux.o: TimerLinux.cpp | directories
	$(CXX) $(CXXFLAGS) TimerLinux.cpp -c -o $(OBJDIR)/TimerLinux.o

$(OBJDIR)/Utils.o: Utils.cpp | directories
	$(CXX) $(CXXFLAGS) Utils.cpp -c -o $(OBJDIR)/Utils.o

.PHONY: directories
directories: $(OBJDIR) $(OUTDIR)

$(OBJDIR):
	mkdir -p "$(OBJDIR)"

$(OUTDIR):
	mkdir -p "$(OUTDIR)"

.PHONY: install
install: $(OUTDIR)/test
	mkdir -p ./build/{test,results}
	cp start_test.sh ./build/start_test.sh
	cp $(OUTDIR)/test ./build/test/test
	chmod 755 ./build/{test/test,start_test.sh}
