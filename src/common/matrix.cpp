#include "matrix.h"
#include <immintrin.h>

M3x1::M3x1(){
    value = static_cast<float*>(_aligned_malloc(3 * sizeof(float), 32));
}
M3x1::~M3x1(){
    _aligned_free(value);
}
int M3x1::getM() const{ return 3; }
int M3x1::getN() const{ return 1; }

M4x1::M4x1(){
    value = static_cast<float*>(_aligned_malloc(4 * sizeof(float), 32));
}
M4x1::M4x1(
    float v00,
    float v10,
    float v20,
    float v30
){
    value = static_cast<float*>(_aligned_malloc(4 * sizeof(float), 32));
    value[0] = v00;
    value[1] = v10;
    value[2] = v20;
    value[3] = v30;
}
M4x1::~M4x1(){
    _aligned_free(value);
}
int M4x1::getM() const{ return 4; }
int M4x1::getN() const{ return 1; }

M3x3::M3x3(){
    value = static_cast<float*>(_aligned_malloc(9 * sizeof(float), 32));
}
M3x3::~M3x3(){
    _aligned_free(value);
}
int M3x3::getM() const{ return 3; }
int M3x3::getN() const{ return 3; }

M4x4::M4x4(){
    value = static_cast<float*>(_aligned_malloc(16 * sizeof(float), 32));
}
M4x4::M4x4(
    float v00, float v01, float v02, float v03,
    float v10, float v11, float v12, float v13,
    float v20, float v21, float v22, float v23,
    float v30, float v31, float v32, float v33
){
    value = static_cast<float*>(_aligned_malloc(16 * sizeof(float), 32));
    value[0] = v00;
    value[1] = v01;
    value[2] = v02;
    value[3] = v03;

    value[4] = v10;
    value[5] = v11;
    value[6] = v12;
    value[7] = v13;
    
    value[8] = v20;
    value[9] = v21;
    value[10] = v22;
    value[11] = v23;

    value[12] = v30;
    value[13] = v31;
    value[14] = v32;
    value[15] = v33;
}
M4x4::~M4x4(){
    _aligned_free(value);
}
int M4x4::getM() const{ return 4; }
int M4x4::getN() const{ return 4; }

M3x4::M3x4(){
    value = static_cast<float*>(_aligned_malloc(12 * sizeof(float), 32));
}
M3x4::M3x4(
    float v00, float v01, float v02, float v03,
    float v10, float v11, float v12, float v13,
    float v20, float v21, float v22, float v23
){
    value = static_cast<float*>(_aligned_malloc(12 * sizeof(float), 32));
    value[0] = v00;
    value[1] = v01;
    value[2] = v02;
    value[3] = v03;

    value[4] = v10;
    value[5] = v11;
    value[6] = v12;
    value[7] = v13;
    
    value[8] = v20;
    value[9] = v21;
    value[10] = v22;
    value[11] = v23;
}
M3x4::~M3x4(){
    _aligned_free(value);
}
int M3x4::getM() const{ return 3; }
int M3x4::getN() const{ return 4; }

void avx256_multi_matrix_4x4_4x4(const float *m1, const float *m2, float* result){
    __m256 row = _mm256_setr_ps(m1[0], m1[1], m1[2], m1[3], m1[0], m1[1], m1[2], m1[3]);
    __m256 col_02 = _mm256_setr_ps(m2[0], m2[4], m2[8], m2[12], m2[2], m2[6], m2[10], m2[14]);
    __m256 col_13 = _mm256_setr_ps(m2[1], m2[5], m2[9], m2[13], m2[3], m2[7], m2[11], m2[15]);
    __m256 row_x_col_02 = _mm256_mul_ps(row, col_02);
    __m256 row_x_col_13 = _mm256_mul_ps(row, col_13);
    __m256 row_result_0 = _mm256_hadd_ps(row_x_col_02, row_x_col_13);
    
    row = _mm256_setr_ps(m1[4], m1[5], m1[6], m1[7], m1[4], m1[5], m1[6], m1[7]);
    row_x_col_02 = _mm256_mul_ps(row, col_02);
    row_x_col_13 = _mm256_mul_ps(row, col_13);
    __m256 row_result_1 = _mm256_hadd_ps(row_x_col_02, row_x_col_13);

    __m256 row_result_01 = _mm256_hadd_ps(
        _mm256_permute2f128_ps(row_result_0, row_result_1, 0b00100000),
        _mm256_permute2f128_ps(row_result_0, row_result_1, 0b00110001)
    );

    row = _mm256_setr_ps(m1[8], m1[9], m1[10], m1[11], m1[8], m1[9], m1[10], m1[11]);
    row_x_col_02 = _mm256_mul_ps(row, col_02);
    row_x_col_13 = _mm256_mul_ps(row, col_13);
    row_result_0 = _mm256_hadd_ps(row_x_col_02, row_x_col_13);

    row = _mm256_setr_ps(m1[12], m1[13], m1[14], m1[15], m1[12], m1[13], m1[14], m1[15]);
    row_x_col_02 = _mm256_mul_ps(row, col_02);
    row_x_col_13 = _mm256_mul_ps(row, col_13);
    row_result_1 = _mm256_hadd_ps(row_x_col_02, row_x_col_13);

    __m256 row_result_23 = _mm256_hadd_ps(
        _mm256_permute2f128_ps(row_result_0, row_result_1, 0b00100000),
        _mm256_permute2f128_ps(row_result_0, row_result_1, 0b00110001)
    );

    _mm256_store_ps(result, row_result_01);
    _mm256_store_ps(result + 8, row_result_23); 
}

void avx256_multi_matrix_4x4_4x1(const float *m1, const float *m2, float* result){
    __m256 row = _mm256_setr_ps(m1[0], m1[1], m1[2], m1[3], m1[8], m1[9], m1[10], m1[11]);
    __m256 col = _mm256_setr_ps(m2[0], m2[1], m2[2], m2[3], m2[0], m2[1], m2[2], m2[3]);
    __m256 row_x_col_1 = _mm256_mul_ps(row, col);

    row = _mm256_setr_ps(m1[4], m1[5], m1[6], m1[7], m1[12], m1[13], m1[14], m1[15]);
    __m256 row_x_col_2 = _mm256_mul_ps(row, col);

    __m256 col_result = _mm256_hadd_ps(row_x_col_1, row_x_col_2);
    __m128 result_128 = _mm_add_ps(
        _mm_setr_ps(col_result[0], col_result[2], col_result[4], col_result[6]),
        _mm_setr_ps(col_result[1], col_result[3], col_result[5], col_result[7])
    );

    _mm_store_ps(result, result_128);
}

void avx256_multi_matrix_3x4_4x1(const float *m1,const float *m2, float* result){
    __m256 row = _mm256_setr_ps(m1[0], m1[1], m1[2], m1[3], m1[8], m1[9], m1[10], m1[11]);
    __m256 col = _mm256_setr_ps(m2[0], m2[1], m2[2], m2[3], m2[0], m2[1], m2[2], m2[3]);
    __m256 row_x_col_1 = _mm256_mul_ps(row, col);

    row = _mm256_setr_ps(m1[4], m1[5], m1[6], m1[7], 0, 0, 0, 0);
    __m256 row_x_col_2 = _mm256_mul_ps(row, col);

    __m256 col_result = _mm256_hadd_ps(row_x_col_1, row_x_col_2);
    __m128 result_128 = _mm_add_ps(
        _mm_setr_ps(col_result[0], col_result[2], col_result[4], col_result[6]),
        _mm_setr_ps(col_result[1], col_result[3], col_result[5], col_result[7])
    );

    _mm_store_ps(result, result_128);
}

M4x4 multi_matrix(const M4x4 &matrix_a, const M4x4 &matrix_b){
    M4x4 result;
    avx256_multi_matrix_4x4_4x4(matrix_a.value, matrix_b.value, result.value);
    
    return result;
}

M4x1 multi_matrix(const M4x4 &matrix_a, const M4x1 &matrix_b){
    M4x1 result;
    avx256_multi_matrix_4x4_4x1(matrix_a.value, matrix_b.value, result.value);
    
    return result;
}

M3x1 multi_matrix(const M3x4 &matrix_a, const M4x1 &matrix_b){
    M3x1 result;
    avx256_multi_matrix_3x4_4x1(matrix_a.value, matrix_b.value, result.value);
    
    return result;
}

M4x1 vec3_to_M4x1(const Vec3 &vec3){
    M4x1 result = {
        vec3.x,
        vec3.y,
        vec3.z,
        1
    };

    return result;
}

Vec3 matrix_to_vec3(const M4x1 &m4x1){
    Vec3 result = {
        m4x1.value[0],
        m4x1.value[1],
        m4x1.value[2]
    };

    return result;
}

Vec3 matrix_to_vec3(const M3x1 &m3x1){
    Vec3 result = {
        m3x1.value[0],
        m3x1.value[1],
        m3x1.value[2]
    };

    return result;
}