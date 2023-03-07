ifeq ($(CC), gcc)
	CXX=g++

	aarch64_cc=aarch64-linux-android-$(CC)
	aarch64_cxx=aarch64-linux-android-$(CXX)
	arm_cc=arm-linux-androideabi-$(CC)
	arm_cxx=arm-linux-androideabi-$(CXX)
	i686_cc=i686-linux-android-$(CC)
	i686_cxx=i686-linux-android-$(CXX)
	x86_64_cc=x86_64-linux-android-$(CC)
	x86_64_cxx=x86_64-linux-android-$(CXX)


	cflags=-O2 -fPIC --sysroot=./sysroot
	cxxflags=-std=c++23 -O2 -fPIC --sysroot=./sysroot
else
	aarch64_cc=clang --target=aarch64-linux-android
	aarch64_cxx=clang++ --target=aarch64-linux-android
	arm_cc=clang --target=armv7a-linux-androideabi
	arm_cxx=clang++ --target=armv7a-linux-androideabi
	i686_cc=clang --target=i686-linux-android
	i686_cxx=clang++ --target=i686-linux-android
	x86_64_cc=clang --target=x86_64-linux-android
	x86_64_cxx=clang++ --target=x86_64-linux-android

	cflags=-O2 -fPIC --sysroot=./sysroot
	cxxflags=-std=c++2b -O2 -fPIC --sysroot=./sysroot
endif

CFLAGS := -std=c99 $(cflags)
CXXFLAGS := -fno-exceptions -fno-rtti $(cxxflags)

c_src = $(wildcard *.c)
cxx_src = $(wildcard *.cpp)

aarch64_c_obj = $(patsubst %.c,aarch64/%.o, $(c_src))
aarch64_cxx_obj = $(patsubst %.cpp,aarch64/%.o, $(cxx_src))
aarch64_obj = $(aarch64_c_obj) $(aarch64_cxx_obj)
aarch64_static = aarch64/libandroid-extra.a


arm_c_obj = $(patsubst %.c,arm/%.o, $(c_src))
arm_cxx_obj = $(patsubst %.cpp,arm/%.o, $(cxx_src))
arm_obj = $(arm_c_obj) $(arm_cxx_obj)
arm_static = arm/libandroid-extra.a


i686_c_obj = $(patsubst %.c,i686/%.o, $(c_src))
i686_cxx_obj = $(patsubst %.cpp,i686/%.o, $(cxx_src))
i686_obj = $(i686_c_obj) $(i686_cxx_obj)
i686_static = i686/libandroid-extra.a


x86_64_c_obj = $(patsubst %.c,x86_64/%.o, $(c_src))
x86_64_cxx_obj = $(patsubst %.cpp,x86_64/%.o, $(cxx_src))
x86_64_obj = $(x86_64_c_obj) $(x86_64_cxx_obj)
x86_64_static = x86_64/libandroid-extra.a


all: $(aarch64_static) $(arm_static) \
# $(i686_static) $(x86_64_static)
	@echo "\e[35mbuild completed!\e[0m"

.PHONY:clean
clean:
	rm -f  aarch64/* arm/* i686/* x86_64/*
	@echo  "\e[35mclean!\e[0m"



$(aarch64_static) : $(aarch64_obj)
	llvm-strip -R .comment --strip-unneeded $(aarch64_obj)
	llvm-ar cru $(aarch64_static) $(aarch64_obj)
	llvm-ranlib $(aarch64_static)

$(arm_static) : $(arm_obj)
	llvm-strip -R .comment --strip-unneeded $(arm_obj)
	llvm-ar cru $(arm_static) $(arm_obj)
	llvm-ranlib $(arm_static)

$(i686_static) : $(i686_obj)
	llvm-strip -R .comment --strip-unneeded $(i686_obj)
	llvm-ar cru $(i686_static) $(i686_obj)
	llvm-ranlib $(i686_static)

$(x86_64_static) : $(x86_64_obj)
	llvm-strip -R .comment --strip-unneeded $(x86_64_obj)
	llvm-ar cru $(x86_64_static) $(x86_64_obj)
	llvm-ranlib $(x86_64_static)


aarch64/%.o : %.c
	$(aarch64_cc) $(CFLAGS) -c $^ -o $@

aarch64/%.o : %.cpp
	$(aarch64_cxx) $(CXXFLAGS) -c $^ -o $@
	
	
arm/%.o : %.c
	$(arm_cc) $(CFLAGS) -c $^ -o $@

arm/%.o : %.cpp
	$(arm_cxx) $(CXXFLAGS) -c $^ -o $@
	
	
i686/%.o : %.c
	$(i686_cc) $(CFLAGS) -c $^ -o $@

i686/%.o : %.cpp
	$(i686_cxx) $(CXXFLAGS) -c $^ -o $@


	
x86_64/%.o : %.c
	$(x86_64_cc) $(CFLAGS) -c $^ -o $@

x86_64/%.o : %.cpp
	$(x86_64_cxx) $(CXXFLAGS) -c $^ -o $@
