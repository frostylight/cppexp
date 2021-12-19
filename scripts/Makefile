Scripts = $(subst .cpp,.o,$(wildcard *.cpp))
Dep = $(Scripts:.o=.d)


all : $(Scripts)
	@echo= 

#编译生成.o文件
%.o : %.cpp
	$(Compiler) $(CompileParam) -c $< -o $@

#文件依赖关系不存在或源文件更改时重新生成
%.d : %.cpp
	@echo updating dependences of $<
	@$(Compiler) $< -MM > $@

#导入文件依赖关系
include $(Dep)
