
CFLAGS = -Os -fpic

all: aarch64 arm i686 x86_64
	@echo "\e[35mbuild completed!\e[0m"
	
.PHONY:clean
clean:
	rm -f  aarch64/* arm/* i686/* x86_64/*
	@echo  "\e[35mclean!\e[0m"


aarch64: aarch64/execinfo.o
	llvm-strip -R .note -R .comment --strip-unneeded aarch64/execinfo.o
	llvm-ar rcu aarch64/libexecinfo.a aarch64/execinfo.o
	llvm-ranlib aarch64/libexecinfo.a
	
arm: arm/execinfo.o
	llvm-strip -R .note -R .comment --strip-unneeded arm/execinfo.o
	llvm-ar rcu arm/libexecinfo.a arm/execinfo.o
	llvm-ranlib arm/libexecinfo.a
	
i686: i686/execinfo.o
	llvm-strip -R .note -R .comment --strip-unneeded i686/execinfo.o
	llvm-ar rcu i686/libexecinfo.a i686/execinfo.o
	llvm-ranlib i686/libexecinfo.a
	
x86_64: x86_64/execinfo.o
	llvm-strip -R .note -R .comment --strip-unneeded x86_64/execinfo.o
	llvm-ar rcu x86_64/libexecinfo.a x86_64/execinfo.o
	llvm-ranlib x86_64/libexecinfo.a

aarch64/%.o : %.c
	clang --target=aarch64-linux-android $(CFLAGS) -c $^ -o $@
	

arm/%.o : %.c
	clang --target=armv7a-linux-androideabi $(CFLAGS) -c $^ -o $@
	

i686/%.o : %.c
	clang --target=i686-linux-android $(CFLAGS) -c $^ -o $@
	

x86_64/%.o : %.c
	clang --target=x86_64-linux-android $(CFLAGS) -c $^ -o $@
