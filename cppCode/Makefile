# ——————————————————————————————————————————————
#  Makefile for scheduling project with CPLEX on HPC
# ——————————————————————————————————————————————

# 1) CPLEX install root (change to match your cluster)
CPLEX_PATH   := /kyukon/home/apps/CO7/haswell-ib/software/CPLEX/12.6
CONCERTDIR   := $(CPLEX_PATH)/concert
CPLEXDIR     := $(CPLEX_PATH)/cplex

# 2) Compiler & flags
CXX          := g++
CXXFLAGS     := -m64 -std=c++17 -g -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -DIL_STD \
                 -Iinclude \
                 -I$(CONCERTDIR)/include \
                 -I$(CPLEXDIR)/include

# 3) Linker settings
LDFLAGS      := -L$(CONCERTDIR)/lib/x86-64_linux/static_pic \
                 -L$(CPLEXDIR)/lib/x86-64_linux/static_pic \
                 -lilocplex -lcplex -lconcert -lpthread

# 4) Project layout
SRCDIR       := src
INCDIR       := include
OBJDIR       := build/obj
BINDIR       := build/bin

# 5) Find all sources and map to object files
SRCS         := $(shell find $(SRCDIR) -name '*.cpp')
OBJS         := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# 6) Final executable
TARGET       := $(BINDIR)/scheduling

.PHONY: all clean

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .cpp into its mirror .o in output/obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure directories exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Remove everything
clean:
	rm -rf build
