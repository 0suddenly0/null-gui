#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   sampler2D tex_sampler;
//   float texel_width;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   texel_width  c0       1
//   tex_sampler  s0       1
//

    ps_2_0
    def c1, 0.227027029, 1.38461542, 0.31621623, 3.23076916
    def c2, 0.0702702701, 0, 0, 0
    dcl t0.xy  // uv<0,1>
    dcl_2d s0

#line 12 "D:\nullptr-sources\null-gui\null-gui\null-render\directx9\shaders\blur\hlsl\blur_x.hlsl"
    mov r0.yw, c1  // ::offsets<1,2>
    mul r0.x, r0.y, c0.x
    mov r0.x, -r0.x
    add r1.x, r0.x, t0.x
    mov r1.y, t0.y
    mul r0.x, r0.y, c0.x
    add r0.x, r0.x, t0.x
    mov r0.y, t0.y
    mul r0.z, r0.w, c0.x
    mov r0.z, -r0.z
    add r2.x, r0.z, t0.x
    mov r2.y, t0.y
    mul r0.z, r0.w, c0.x
    add r3.x, r0.z, t0.x
    mov r3.y, t0.y

#line 8
    texld r4, t0, s0  // ::color<0,1,2,3>

#line 12
    texld r1, r1, s0
    texld r0, r0, s0
    texld r2, r2, s0
    texld r3, r3, s0

#line 9
    mul r5.xyz, r4, c1.x  // ::color<0,1,2>

#line 12
    mul r1.xyz, r1, c1.z
    add r1.xyz, r1, r5  // ::color<0,1,2>
    mul r0.xyz, r0, c1.z
    add r0.xyz, r0, r1  // ::color<0,1,2>
    mul r1.xyz, r2, c2.x
    add r0.xyz, r0, r1  // ::color<0,1,2>
    mul r1.xyz, r3, c2.x
    add r4.xyz, r0, r1  // ::color<0,1,2>

#line 16
    mov r4.xyz, r4  // ::main<0,1,2>
    mov r4.w, r4.w  // ::main<3>

#line 7
    mov oC0, r4  // ::main<0,1,2,3>

// approximately 32 instruction slots used (5 texture, 27 arithmetic)
#endif

const BYTE blur_x[] =
{
      0,   2, 255, 255, 254, 255, 
    192,   0,  68,  66,  85,  71, 
     40,   0,   0,   0, 212,   2, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 128,   0, 
      0,   0,  36,   0,   0,   0, 
    132,   0,   0,   0,   4,   0, 
      0,   0, 132,   2,   0,   0, 
      4,   2,   0,   0,  68,  58, 
     92, 110, 117, 108, 108, 112, 
    116, 114,  45, 115, 111, 117, 
    114,  99, 101, 115,  92, 110, 
    117, 108, 108,  45, 103, 117, 
    105,  92, 110, 117, 108, 108, 
     45, 103, 117, 105,  92, 110, 
    117, 108, 108,  45, 114, 101, 
    110, 100, 101, 114,  92, 100, 
    105, 114, 101,  99, 116, 120, 
     57,  92, 115, 104,  97, 100, 
    101, 114, 115,  92,  98, 108, 
    117, 114,  92, 104, 108, 115, 
    108,  92,  98, 108, 117, 114, 
     95, 120,  46, 104, 108, 115, 
    108,   0,  40,   0,   0,   0, 
      0,   0, 255, 255, 188,   3, 
      0,   0,   0,   0, 255, 255, 
    212,   3,   0,   0,   0,   0, 
    255, 255, 236,   3,   0,   0, 
      0,   0, 255, 255, 248,   3, 
      0,   0,  12,   0,   0,   0, 
      4,   4,   0,   0,  12,   0, 
      0,   0,  16,   4,   0,   0, 
     12,   0,   0,   0,  32,   4, 
      0,   0,  12,   0,   0,   0, 
     44,   4,   0,   0,  12,   0, 
      0,   0,  60,   4,   0,   0, 
     13,   0,   0,   0,  72,   4, 
      0,   0,  13,   0,   0,   0, 
     88,   4,   0,   0,  13,   0, 
      0,   0, 104,   4,   0,   0, 
     12,   0,   0,   0, 116,   4, 
      0,   0,  12,   0,   0,   0, 
    132,   4,   0,   0,  12,   0, 
      0,   0, 144,   4,   0,   0, 
     12,   0,   0,   0, 160,   4, 
      0,   0,  13,   0,   0,   0, 
    172,   4,   0,   0,  13,   0, 
      0,   0, 188,   4,   0,   0, 
     13,   0,   0,   0, 204,   4, 
      0,   0,   8,   0,   0,   0, 
    216,   4,   0,   0,  12,   0, 
      0,   0, 232,   4,   0,   0, 
     13,   0,   0,   0, 248,   4, 
      0,   0,  12,   0,   0,   0, 
      8,   5,   0,   0,  13,   0, 
      0,   0,  24,   5,   0,   0, 
      9,   0,   0,   0,  40,   5, 
      0,   0,  12,   0,   0,   0, 
     56,   5,   0,   0,  12,   0, 
      0,   0,  72,   5,   0,   0, 
     13,   0,   0,   0,  88,   5, 
      0,   0,  13,   0,   0,   0, 
    104,   5,   0,   0,  12,   0, 
      0,   0, 120,   5,   0,   0, 
     12,   0,   0,   0, 136,   5, 
      0,   0,  13,   0,   0,   0, 
    152,   5,   0,   0,  13,   0, 
      0,   0, 168,   5,   0,   0, 
     16,   0,   0,   0, 184,   5, 
      0,   0,  16,   0,   0,   0, 
    196,   5,   0,   0,   7,   0, 
      0,   0, 208,   5,   0,   0, 
     99, 111, 108, 111, 114,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     19,   0,   0,   0,   0,   0, 
      1,   0,   2,   0,   3,   0, 
     24,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     26,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     28,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     30,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
    109,  97, 105, 110,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     33,   0,   0,   0,   0,   0, 
      1,   0,   2,   0, 255, 255, 
     34,   0,   0,   0, 255, 255, 
    255, 255, 255, 255,   3,   0, 
     35,   0,   0,   0,   0,   0, 
      1,   0,   2,   0,   3,   0, 
    111, 102, 102, 115, 101, 116, 
    115,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
      1,   0, 255, 255,   2,   0, 
    117, 118,   0, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   1,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    164,   1,   0,   0, 172,   1, 
      0,   0,   6,   0,   0,   0, 
    188,   1,   0,   0,   0,   0, 
      0,   0,   4,   2,   0,   0, 
     12,   2,   0,   0,   3,   0, 
      0,   0,  28,   2,   0,   0, 
      0,   0,   0,   0,  64,   2, 
      0,   0,  72,   2,   0,   0, 
      1,   0,   0,   0,  88,   2, 
      0,   0,   4,   2,   0,   0, 
    100,   2,   0,   0, 104,   2, 
      0,   0,   1,   0,   0,   0, 
    120,   2,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0, 254, 255,  44,   0, 
     67,  84,  65,  66,  28,   0, 
      0,   0, 131,   0,   0,   0, 
      0,   2, 255, 255,   2,   0, 
      0,   0,  28,   0,   0,   0, 
      5,  17,   0,   0, 124,   0, 
      0,   0,  68,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,  96,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0, 108,   0, 
      0,   0,   0,   0,   0,   0, 
    116, 101, 120,  95, 115,  97, 
    109, 112, 108, 101, 114,   0, 
      4,   0,  12,   0,   1,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 116, 101, 
    120, 101, 108,  95, 119, 105, 
    100, 116, 104,   0,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 112, 115,  95,  50, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  49,  48,  46,  49, 
      0, 171,  81,   0,   0,   5, 
      1,   0,  15, 160, 198, 121, 
    104,  62,  20,  59, 177,  63, 
     24, 231, 161,  62, 236, 196, 
     78,  64,  81,   0,   0,   5, 
      2,   0,  15, 160, 220, 233, 
    143,  61,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
      3, 176,  31,   0,   0,   2, 
      0,   0,   0, 144,   0,   8, 
     15, 160,   1,   0,   0,   2, 
      0,   0,  10, 128,   1,   0, 
    228, 160,   5,   0,   0,   3, 
      0,   0,   1, 128,   0,   0, 
     85, 128,   0,   0,   0, 160, 
      1,   0,   0,   2,   0,   0, 
      1, 128,   0,   0,   0, 129, 
      2,   0,   0,   3,   1,   0, 
      1, 128,   0,   0,   0, 128, 
      0,   0,   0, 176,   1,   0, 
      0,   2,   1,   0,   2, 128, 
      0,   0,  85, 176,   5,   0, 
      0,   3,   0,   0,   1, 128, 
      0,   0,  85, 128,   0,   0, 
      0, 160,   2,   0,   0,   3, 
      0,   0,   1, 128,   0,   0, 
      0, 128,   0,   0,   0, 176, 
      1,   0,   0,   2,   0,   0, 
      2, 128,   0,   0,  85, 176, 
      5,   0,   0,   3,   0,   0, 
      4, 128,   0,   0, 255, 128, 
      0,   0,   0, 160,   1,   0, 
      0,   2,   0,   0,   4, 128, 
      0,   0, 170, 129,   2,   0, 
      0,   3,   2,   0,   1, 128, 
      0,   0, 170, 128,   0,   0, 
      0, 176,   1,   0,   0,   2, 
      2,   0,   2, 128,   0,   0, 
     85, 176,   5,   0,   0,   3, 
      0,   0,   4, 128,   0,   0, 
    255, 128,   0,   0,   0, 160, 
      2,   0,   0,   3,   3,   0, 
      1, 128,   0,   0, 170, 128, 
      0,   0,   0, 176,   1,   0, 
      0,   2,   3,   0,   2, 128, 
      0,   0,  85, 176,  66,   0, 
      0,   3,   4,   0,  15, 128, 
      0,   0, 228, 176,   0,   8, 
    228, 160,  66,   0,   0,   3, 
      1,   0,  15, 128,   1,   0, 
    228, 128,   0,   8, 228, 160, 
     66,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      0,   8, 228, 160,  66,   0, 
      0,   3,   2,   0,  15, 128, 
      2,   0, 228, 128,   0,   8, 
    228, 160,  66,   0,   0,   3, 
      3,   0,  15, 128,   3,   0, 
    228, 128,   0,   8, 228, 160, 
      5,   0,   0,   3,   5,   0, 
      7, 128,   4,   0, 228, 128, 
      1,   0,   0, 160,   5,   0, 
      0,   3,   1,   0,   7, 128, 
      1,   0, 228, 128,   1,   0, 
    170, 160,   2,   0,   0,   3, 
      1,   0,   7, 128,   1,   0, 
    228, 128,   5,   0, 228, 128, 
      5,   0,   0,   3,   0,   0, 
      7, 128,   0,   0, 228, 128, 
      1,   0, 170, 160,   2,   0, 
      0,   3,   0,   0,   7, 128, 
      0,   0, 228, 128,   1,   0, 
    228, 128,   5,   0,   0,   3, 
      1,   0,   7, 128,   2,   0, 
    228, 128,   2,   0,   0, 160, 
      2,   0,   0,   3,   0,   0, 
      7, 128,   0,   0, 228, 128, 
      1,   0, 228, 128,   5,   0, 
      0,   3,   1,   0,   7, 128, 
      3,   0, 228, 128,   2,   0, 
      0, 160,   2,   0,   0,   3, 
      4,   0,   7, 128,   0,   0, 
    228, 128,   1,   0, 228, 128, 
      1,   0,   0,   2,   4,   0, 
      7, 128,   4,   0, 228, 128, 
      1,   0,   0,   2,   4,   0, 
      8, 128,   4,   0, 255, 128, 
      1,   0,   0,   2,   0,   8, 
     15, 128,   4,   0, 228, 128, 
    255, 255,   0,   0
};
