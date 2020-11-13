/*
 * @Author: your name
 * @Date: 2020-11-11 14:37:11
 * @LastEditTime: 2020-11-12 16:04:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_coo_matrix.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "matrix/CooMatrix.h"
#include "test_data.h"

#define PR_COOMATRIX(matrix_ptr, row, col) do { \
    for (int i=0; i<row; ++i) { \
        for (int j=0; j<col; ++j) { \
            printf("%02f ", Matrix_get(matrix_ptr, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

static int  suite_success_init (void) 
{
    printf("\nCooMatrix suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nCooMatrix suite success clean\n");
}

static void test_coo_matrix_set_get (void)
{
    extern float test_data_float[TEST_DATA_SIZE];
    CooMatrix* matrix = CooMatrix_load(20, 5, test_data_float);
    printf("\n\n");
    PR_COOMATRIX(matrix, Matrix_rows(matrix), Matrix_cols(matrix));
    CooMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

int do_coo_matrix_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test CooMatrix Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test CooMatrix set get", test_coo_matrix_set_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}