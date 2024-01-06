src=$(wildcard ./*.cpp)                #获取当前目录下的所有 .cpp 文件，可以有多个格式文件；
objs=$(patsubst %.cpp, %.o, $(src))    #把 .cpp 文件替换为 .o 文件
target=cpu
$(target):$(objs)
	$(CXX) $(objs) -o $(target) -lpthread 
    
%.o:%.cpp
	$(CXX) -c $< -o $@
	
#伪目标（即不会生成文件），编译完成后删除 .o 文件
.PHONY:clean
clean:
	rm $(objs) -f
	rm cpu -f