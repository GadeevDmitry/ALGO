CFLAGS = -D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192

#-----------------------------------------------------
# lib
#-----------------------------------------------------

LOG_O = ../../../lib/logs/log.o
ALG_O = ../../../lib/algorithm/algorithm.o
STK_O = ../../../lib/stack/stack.o
LST_O = ../../../lib/list/list.o

ALL_O = $(LOG_O) $(ALG_O) $(STK_O) $(LST_O)

#-----------------------------------------------------
# main
#-----------------------------------------------------

a_contest:
a_test:

b_contest:
b_test:

c_contest:
c_test:

d_contest:
d_test:

e_contest:
e_test:

f_contest:
f_test:

g_contest:
g_test:

i_contest:
i_test:

j_contest:
j_test:

%_contest: %.cpp
	g++ $(CFLAGS) $^ -o $@
%_test: %.cpp $(ALL_O)
	g++ $(CFLAGS) $^ -o $@