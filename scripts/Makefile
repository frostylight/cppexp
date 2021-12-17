Scripts = $(subst .cpp,.o,$(wildcard *.cpp))
Dep = $(Scripts:.o=.d)


all : $(Scripts:%=$(CacheDir)/%)
	@echo= 

$(CacheDir)/%.o : %.cpp
	$(Compiler) $(CompileParam) -c $< -o $@

$(CacheDir)/%.d : %.cpp
	@$(Compiler) $< -MM > $@

include $(Dep:%=$(CacheDir)/%)
