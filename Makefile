CXXFLAGS = -Wall -std=c++14 -I . $(shell sdl2-config --cflags)
LDLIBS = -l GLESv2 $(shell sdl2-config --libs)

SRCDIRS := \
	engine \
	modules \
	utils

SRCS := $(wildcard *.cpp $(patsubst %,%/*.cpp,$(SRCDIRS)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

DEPDIR := .d
DEPS := $(patsubst %.cpp,$(DEPDIR)/%.d,$(SRCS))
$(shell for i in $(SRCDIRS); do mkdir -p $(DEPDIR)/$$i; done >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

all: opengl-es-test

opengl-es-test: $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(OBJS): %.o: %.cpp $(DEPDIR)/%.d
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPS): ;
.PRECIOUS: $(DEPS)

include $(DEPS)

.PHONY: clean
clean:
	rm -f opengl-es-test $(OBJS)
	rm -rf $(DEPDIR)

.PHONY: retab
retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
