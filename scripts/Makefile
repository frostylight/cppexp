Scripts = $(subst .cpp,.o,$(wildcard *.cpp))
Dep = $(Scripts:.o=.d)


all : $(Scripts:%=$(CacheDir)/%)
	@echo=finish compile

$(CacheDir)/%.o : %.cpp
	$(Compiler) $(CompileParam) -c $< -o $@

$(CacheDir)/%.d : %.cpp
	@echo updating dependences of $<
	@$(Compiler) $< -MM > $@
#	python dep.py $@.t
#	del $@.t

#include $(Dep:%=$(CacheDir)/%)
