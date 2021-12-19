Scripts = $(subst .cpp,.o,$(wildcard *.cpp))
Dep = $(Scripts:.o=.d)


all : $(Scripts)
	@echo= 

%.o : %.cpp
	$(Compiler) $(CompileParam) -c $< -o $@

%.d : %.cpp
	@echo updating dependences of $<
	@$(Compiler) $< -MM > $@

include $(Dep)
