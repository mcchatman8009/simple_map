#include <stdio.h>
#include "simple_map.h"



// @formatter:off
// Simple Unit https://github.com/mcchatman8009/simple_unit
#ifndef HEADER_ONLY_SIMPLE_UNIT_H
#define HEADER_ONLY_SIMPLE_UNIT_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define describe_suite(text,code_block){clock_t clock_before=clock();printf("%s\n",text);code_block double last_cock=(double)clock();double clock_secs=((last_cock-clock_before)/ CLOCKS_PER_SEC);clock_t clock_ms=(clock_t)(clock_secs*1000);printf("took %ldms %fsecs\n\n",clock_ms,clock_secs);} NULL
#define unit_test(text,code_block){printf("   %s ",text);code_block printf("%s","[PASSED]\n");} NULL
#define test_assert(val)if(!(val)){fprintf(stdout,"file:%s function:%s line:%d",__FILE__,__FUNCTION__,__LINE__);fflush(stdout);exit(1);} NULL
#define assert_true(val){if(!(val)){printf("[FAILED]\n");fprintf(stdout,"assert_true(%s);\n",#val);}else{printf("");} test_assert(val);} NULL
#define assert_str_not_eq(lhs,rhs){if(strcmp(lhs,rhs)==0){printf("[FAILED]\n");fprintf(stdout,"assert_str_not_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(strcmp(lhs,rhs)!=0);} NULL
#define assert_str_eq(lhs,rhs){if(strcmp(lhs,rhs)!=0){printf("[FAILED]\n");fprintf(stdout,"assert_str_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(strcmp(lhs,rhs)==0);} NULL
#define assert_float_not_eq(lhs,rhs){const double __eps=0.00001f;double __d=lhs-rhs;int __res=fabs(__d)<__eps;if((__res)){printf("[FAILED]\n");fprintf(stdout,"assert_float_not_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(!__res);} NULL
#define assert_float_eq(lhs,rhs){const double __eps=0.00001f;double __d=lhs-rhs;int __res=fabs(__d)<__eps;if(!(__res)){printf("[FAILED]\n");fprintf(stdout,"assert_float_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(__res);} NULL
#define assert_not_eq(lhs,rhs){if((lhs==rhs)){printf("[FAILED]\n");fprintf(stdout," assert_not_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(lhs!=rhs);} NULL
#define assert_eq(lhs,rhs){if((lhs!=rhs)){printf("[FAILED]\n");fprintf(stdout," assert_eq(%s,%s);\n",#lhs,#rhs);}else{printf("");} test_assert(lhs==rhs);} NULL
#endif
// @formatter:on


int main() {
    describe_suite("Hashmap creation tests", {
        unit_test("Test alloc & free functions", {
            smap * m = sm_alloc(1);
            sm_free(m);
        });

        unit_test("Test increasing the map capacity", {
            smap * m = sm_alloc(1);
            sm_put_str(m, "Key 1", "Value");
            sm_put_str(m, "Key 2", "Value");
            sm_put_str(m, "Key 3", "Value");
            sm_put_str(m, "Key 4", "Value");
            sm_put_str(m, "Key 5", "Value");
            sm_put_str(m, "Key 6", "Value");
            sm_put_str(m, "Key 7", "Value");
            sm_put_str(m, "Key 8", "Value");
            sm_put_str(m, "Key 9", "Value");
            sm_put_str(m, "Key 10", "Value");
            sm_put_str(m, "Key 11", "Value");
            sm_put_str(m, "Key 12", "Value");

            assert_true(m->cap >= 10);
            assert_eq(m->size, 12);
            sm_free(m);
        });
    });

    describe_suite("Test Hashmap functions", {
        unit_test("Test put string function", {
            smap * m = sm_new();
            sm_put_str(m, "Key", "Value");
            assert_str_eq(sm_get(m, "Key"), "Value");
            sm_free(m);
        });

        unit_test("Test put int function", {
            smap * m = sm_new();
            sm_put_int(m, "Key", 998);

            int* i_ptr = sm_get(m, "Key");

            assert_eq(*i_ptr, 998);
            sm_free(m);
        });

        unit_test("Test put long function", {
            smap * m = sm_new();
            sm_put_long(m, "Key", 998L);

            long* i_ptr = sm_get(m, "Key");

            assert_eq(*i_ptr, 998L);
            sm_free(m);
        });

        unit_test("Test put long function", {
            smap * m = sm_new();
            sm_put_long(m, "Key", 998L);

            long* l_ptr = sm_get(m, "Key");

            assert_eq(*l_ptr, 998L);
            sm_free(m);
        });

        unit_test("Test put float function", {
            smap * m = sm_new();
            sm_put_float(m, "Key", 110.8901f);

            float* ptr = sm_get(m, "Key");

            assert_float_eq(*ptr, 110.8901f);
            sm_free(m);
        });

        unit_test("Test put double function", {
            smap * m = sm_new();
            sm_put_double(m, "Key", 110000000.8901);

            double* ptr = sm_get(m, "Key");

            assert_float_eq(*ptr, 110000000.8901);
            sm_free(m);
        });

        unit_test("Test put char function", {
            smap * m = sm_new();
            sm_put_ch(m, "Key", 'A');

            char* ptr = sm_get(m, "Key");

            assert_eq(*ptr, 'A');
            sm_free(m);
        });

        unit_test("Test put pointer function", {
            smap * m = sm_new();
            char str[] = "Hello, Test";
            sm_put(m, "Key", str);

            const char* ptr = sm_get(m, "Key");

            assert_str_eq(str, ptr);
            sm_free(m);
        });

        unit_test("Test that map contains key", {
            smap * m = sm_new();
            char str[] = "Hello, Test";
            sm_put(m, "Key", str);

            assert_true(sm_contains(m, "Key"));

            sm_free(m);
        });

        unit_test("Test map contains missing key", {
            smap * m = sm_new();
            char str[] = "Hello, Test";
            sm_put(m, "Key", str);

            assert_true(!sm_contains(m, "Key1"));

            sm_free(m);
        });

        unit_test("Test missing key", {
            smap * m = sm_new();
            char str[] = "Hello, Test";
            sm_put(m, "Key", str);

            const char* ptr = sm_get(m, "Key1");

            assert_eq(ptr, NULL);

            sm_free(m);
        });

        unit_test("Test empty function", {
            smap * m = sm_new();
            assert_true(sm_empty(m));
            sm_free(m);
        });

        unit_test("Test empty function with key->value", {
            smap * m = sm_new();
            char str[] = "Hello, Test";
            sm_put(m, "Key", str);

            assert_not_eq(true, sm_empty(m));
            assert_eq(1, sm_size(m));

            sm_free(m);
        });

        unit_test("Create 10,000 duplicate keys", {
            const int num_of_elements  = 10000;
            smap * m = sm_new();

            for (int i = 0; i < num_of_elements; i++) {
                sm_put_int(m, "Key 1", i);
            }
            assert_eq(sm_size(m), 1);

            sm_free(m);
        });

        unit_test("Create 100,000 elements, and clear them all", {
            const int num_of_elements  = 100000;
            smap * m = sm_new();

            for (int i = 0; i < num_of_elements; i++) {
                int size = snprintf(NULL, 0, "Key %d", i) + 1;
                char buffer[size];
                snprintf(buffer, size, "Key %d", i);

                sm_put_int(m, buffer, i);
            }

            assert_eq(sm_size(m), num_of_elements);

            sm_clear(m);

            assert_eq(sm_size(m), 0);
            assert_true(sm_empty(m));

            sm_free(m);
        });

        unit_test("Create 100 elements, and the last remove 5 of them", {
            const int num_of_elements  = 100;
            smap * m = sm_new();

            for (int i = 0; i < num_of_elements; i++) {
                int size = snprintf(NULL, 0, "Key %d", i + 1) + 1;
                char buffer[size];
                snprintf(buffer, size, "Key %d", i + 1);

                sm_put_int(m, buffer, i);
            }

            sm_remove(m, "Key 100");
            sm_remove(m, "Key 100");
            sm_remove(m, "Key 99");
            sm_remove(m, "Key 98");
            sm_remove(m, "Key 97");
            sm_remove(m, "Key 96");

            assert_eq(sm_size(m), 95);

            for (int i = 0; i < num_of_elements; i++) {
                int size = snprintf(NULL, 0, "Key %d", i + 1) + 1;
                char buffer[size];
                snprintf(buffer, size, "Key %d", i + 1);

                if (i < (num_of_elements - 5)) {
                    assert_true(sm_contains(m, buffer));
                } else {
                    assert_true(!sm_contains(m, buffer));
                }
            }

            sm_free(m);
        });


        unit_test("Test get or default of a map", {
            smap * m = sm_new();
            int data = 100;
            int* i = &data;

            int* res = sm_get_or_default(m, "Key", i);

            assert_eq(*res, *i);

            sm_free(m);
        });

        unit_test("Read all the keys of a given map", {
            const int num_of_elements  = 100;
            smap * m = sm_new();

            for (int i = 0; i < num_of_elements; i++) {
                int size = snprintf(NULL, 0, "Key %d", i + 1) + 1;
                char buffer[size];
                snprintf(buffer, size, "Key %d", i + 1);

                sm_put_int(m, buffer, i);
            }

            const char* keys[num_of_elements];
            sm_read_keys(m, keys, num_of_elements);

            for (int i = 0; i < num_of_elements; i++) {
                assert_true(sm_get(m, keys[i]) != NULL);
            }

            sm_free(m);
        });

        unit_test("Read all the values of a given map", {
            const int num_of_elements  = 100;
            smap * m = sm_new();

            for (int i = 0; i < num_of_elements; i++) {
                int size = snprintf(NULL, 0, "Key %d", i + 1) + 1;
                char buffer[size];
                snprintf(buffer, size, "Key %d", i + 1);

                sm_put_int(m, buffer, i);
            }

            int * vals[num_of_elements];
            sm_read_vals(m, (void**) vals, num_of_elements);

            for (int i = 0; i < num_of_elements; i++) {
                assert_true(*vals[i] >= 0 && *vals[i] < num_of_elements);
            }

            sm_free(m);
        });

    });

    return 0;
}
