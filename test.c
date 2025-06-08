#include "greatest/greatest.h"
#include "cstring_array.h"
#include "cstring_array_aligned.h"

TEST test_cstring_array_new(void) {
    cstring_array *array = cstring_array_new();
    ASSERT(array != NULL);
    ASSERT_EQ(array->indices->n, 0);
    ASSERT_EQ(array->str->n, 0);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_new(void) {
    cstring_array_aligned *array = cstring_array_aligned_new();
    ASSERT(array != NULL);
    ASSERT_EQ(array->indices->n, 0);
    ASSERT_EQ(array->str->n, 0);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_new_size(void) {
    cstring_array *array = cstring_array_new_size(10);
    ASSERT(array != NULL);
    ASSERT_EQ(array->indices->n, 0);
    ASSERT_EQ(array->str->m, 10);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_new_size(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    ASSERT(array != NULL);
    ASSERT_EQ(array->indices->n, 0);
    ASSERT_EQ(array->str->m, 10);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_capacity(void) {
    cstring_array *array = cstring_array_new_size(10);
    ASSERT_EQ(cstring_array_capacity(array), 10);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_capacity(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    ASSERT_EQ(cstring_array_aligned_capacity(array), 10);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_used(void) {
    cstring_array *array = cstring_array_new_size(10);
    ASSERT_EQ(cstring_array_used(array), 0);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_used(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    ASSERT_EQ(cstring_array_aligned_used(array), 0);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_num_strings(void) {
    cstring_array *array = cstring_array_new_size(10);
    ASSERT_EQ(cstring_array_num_strings(array), 0);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_num_strings(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    ASSERT_EQ(cstring_array_aligned_num_strings(array), 0);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_resize(void) {
    cstring_array *array = cstring_array_new_size(10);
    cstring_array_resize(array, 20);
    ASSERT_EQ(array->str->m, 20);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_resize(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    cstring_array_aligned_resize(array, 20);
    ASSERT_EQ(array->str->m, 20);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_clear(void) {
    cstring_array *array = cstring_array_new_size(10);
    cstring_array_clear(array);
    ASSERT_EQ(array->str->n, 0);
    ASSERT_EQ(array->indices->n, 0);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_clear(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    cstring_array_aligned_clear(array);
    ASSERT_EQ(array->str->n, 0);
    ASSERT_EQ(array->indices->n, 0);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_start_token(void) {
    cstring_array *array = cstring_array_new_size(10);
    uint32_t index = cstring_array_start_token(array);
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_start_token(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    uint32_t index = cstring_array_aligned_start_token(array);
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_terminate(void) {
    cstring_array *array = cstring_array_new_size(10);
    cstring_array_start_token(array);
    cstring_array_terminate(array);
    ASSERT_EQ(array->str->a[0], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_terminate(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    cstring_array_aligned_start_token(array);
    cstring_array_aligned_terminate(array);
    ASSERT_EQ(array->str->a[0], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_add_string(void) {
    cstring_array *array = cstring_array_new_size(10);
    uint32_t index = cstring_array_add_string(array, "hello");
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_add_string(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    uint32_t index = cstring_array_aligned_add_string(array, "hello");
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_add_string_len(void) {
    cstring_array *array = cstring_array_new_size(10);
    uint32_t index = cstring_array_add_string_len(array, "hello", 5);
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_add_string_len(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    uint32_t index = cstring_array_aligned_add_string_len(array, "hello", 5);
    ASSERT_EQ(index, 0);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_cat_string(void) {
    cstring_array *array = cstring_array_new_size(10);
    cstring_array_start_token(array);
    cstring_array_cat_string(array, "hello");
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_cat_string(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    cstring_array_aligned_start_token(array);
    cstring_array_aligned_cat_string(array, "hello");
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_cat_string_len(void) {
    cstring_array *array = cstring_array_new_size(10);
    cstring_array_start_token(array);
    cstring_array_cat_string_len(array, "hello", 5);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_cat_string_len(void) {
    cstring_array_aligned *array = cstring_array_aligned_new_size(10);
    cstring_array_aligned_start_token(array);
    cstring_array_aligned_cat_string_len(array, "hello", 5);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_from_char_array(void) {
    char_array *str = char_array_new();
    char_array_push(str, 'h');
    char_array_push(str, 'e');
    char_array_push(str, 'l');
    char_array_push(str, 'l');
    char_array_push(str, 'o');
    char_array_push(str, '\0');
    cstring_array *array = cstring_array_from_char_array(str);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_from_char_array(void) {
    char_array_aligned *str = char_array_aligned_new();
    char_array_aligned_push(str, 'h');
    char_array_aligned_push(str, 'e');
    char_array_aligned_push(str, 'l');
    char_array_aligned_push(str, 'l');
    char_array_aligned_push(str, 'o');
    char_array_aligned_push(str, '\0');
    cstring_array_aligned *array = cstring_array_aligned_from_char_array(str);
    ASSERT_EQ(array->indices->n, 1);
    ASSERT_EQ(array->str->n, 6);
    ASSERT_EQ(array->str->a[5], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

TEST test_cstring_array_from_strings(void) {
    cstring_array *array = cstring_array_from_strings((char *[]){"hello", "world"}, 2);
    ASSERT_EQ(array->indices->n, 2);
    ASSERT_EQ(array->str->n, 12);
    ASSERT_EQ(array->str->a[11], '\0');
    cstring_array_destroy(array);
    PASS();
}

TEST test_cstring_array_aligned_from_strings(void) {
    cstring_array_aligned *array = cstring_array_aligned_from_strings((char *[]){"hello", "world"}, 2);
    ASSERT_EQ(array->indices->n, 2);
    ASSERT_EQ(array->str->n, 12);
    ASSERT_EQ(array->str->a[11], '\0');
    cstring_array_aligned_destroy(array);
    PASS();
}

SUITE(cstring_array_suite) {
    RUN_TEST(test_cstring_array_new);
    RUN_TEST(test_cstring_array_aligned_new);
    RUN_TEST(test_cstring_array_new_size);
    RUN_TEST(test_cstring_array_aligned_new_size);
    RUN_TEST(test_cstring_array_capacity);
    RUN_TEST(test_cstring_array_aligned_capacity);
    RUN_TEST(test_cstring_array_used);
    RUN_TEST(test_cstring_array_aligned_used);
    RUN_TEST(test_cstring_array_num_strings);
    RUN_TEST(test_cstring_array_aligned_num_strings);
    RUN_TEST(test_cstring_array_resize);
    RUN_TEST(test_cstring_array_aligned_resize);
    RUN_TEST(test_cstring_array_clear);
    RUN_TEST(test_cstring_array_aligned_clear);
    RUN_TEST(test_cstring_array_start_token);
    RUN_TEST(test_cstring_array_aligned_start_token);
    RUN_TEST(test_cstring_array_terminate);
    RUN_TEST(test_cstring_array_aligned_terminate);
    RUN_TEST(test_cstring_array_add_string);
    RUN_TEST(test_cstring_array_aligned_add_string);
    RUN_TEST(test_cstring_array_add_string_len);
    RUN_TEST(test_cstring_array_aligned_add_string_len);
    RUN_TEST(test_cstring_array_cat_string);
    RUN_TEST(test_cstring_array_aligned_cat_string);
    RUN_TEST(test_cstring_array_cat_string_len);
    RUN_TEST(test_cstring_array_aligned_cat_string_len);
    RUN_TEST(test_cstring_array_from_char_array);
    RUN_TEST(test_cstring_array_aligned_from_char_array);
    RUN_TEST(test_cstring_array_from_strings);
    RUN_TEST(test_cstring_array_aligned_from_strings);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(cstring_array_suite);
    GREATEST_MAIN_END();
}

