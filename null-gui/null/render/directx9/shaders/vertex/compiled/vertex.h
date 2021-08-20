#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   float4x4 projection_matrix;
//
//
// Registers:
//
//   Name              Reg   Size
//   ----------------- ----- ----
//   projection_matrix c0       4
//

    vs_3_0
    def c4, 0, 1, 0, 0
    dcl_position v0  // input<0,1>
    dcl_color v1  // input<2,3,4,5>
    dcl_texcoord v2  // input<6,7>
    dcl_position o0
    dcl_color o1
    dcl_texcoord o2.xy

#line 17 "D:\nullptr-sources\null-gui\null-gui\null\render\directx9\shaders\vertex\hlsl\vertex.hlsl"
    mul r0, c0, v0.x
    mul r1, c1, v0.y
    add r0, r0, r1
    add r0, r0, c4.x
    mov r1.y, c4.y
    mul r1, r1.y, c3
    add r0, r0, r1  // ::output<0,1,2,3>
    mov r1, v1  // ::output<4,5,6,7>
    mov r2.xy, v2  // ::output<8,9>
    mov o0, r0  // ::main<0,1,2,3>
    mov o1, r1  // ::main<4,5,6,7>
    mov o2.xy, r2  // ::main<8,9>

// approximately 12 instruction slots used
#endif

const BYTE vertex[] =
{
      0,   3, 254, 255, 254, 255, 
    165,   0,  68,  66,  85,  71, 
     40,   0,   0,   0, 104,   2, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 132,   0, 
      0,   0,  19,   0,   0,   0, 
    136,   0,   0,   0,   3,   0, 
      0,   0,  44,   2,   0,   0, 
     32,   1,   0,   0,  68,  58, 
     92, 110, 117, 108, 108, 112, 
    116, 114,  45, 115, 111, 117, 
    114,  99, 101, 115,  92, 110, 
    117, 108, 108,  45, 103, 117, 
    105,  92, 110, 117, 108, 108, 
     45, 103, 117, 105,  92, 110, 
    117, 108, 108,  92, 114, 101, 
    110, 100, 101, 114,  92, 100, 
    105, 114, 101,  99, 116, 120, 
     57,  92, 115, 104,  97, 100, 
    101, 114, 115,  92, 118, 101, 
    114, 116, 101, 120,  92, 104, 
    108, 115, 108,  92, 118, 101, 
    114, 116, 101, 120,  46, 104, 
    108, 115, 108,   0, 171, 171, 
     40,   0,   0,   0,   0,   0, 
    255, 255,  40,   3,   0,   0, 
      0,   0, 255, 255,  64,   3, 
      0,   0,   0,   0, 255, 255, 
     76,   3,   0,   0,   0,   0, 
    255, 255,  88,   3,   0,   0, 
      0,   0, 255, 255, 100,   3, 
      0,   0,   0,   0, 255, 255, 
    112,   3,   0,   0,   0,   0, 
    255, 255, 124,   3,   0,   0, 
     17,   0,   0,   0, 136,   3, 
      0,   0,  17,   0,   0,   0, 
    152,   3,   0,   0,  17,   0, 
      0,   0, 168,   3,   0,   0, 
     17,   0,   0,   0, 184,   3, 
      0,   0,  17,   0,   0,   0, 
    200,   3,   0,   0,  17,   0, 
      0,   0, 212,   3,   0,   0, 
     17,   0,   0,   0, 228,   3, 
      0,   0,  18,   0,   0,   0, 
    244,   3,   0,   0,  19,   0, 
      0,   0,   0,   4,   0,   0, 
     20,   0,   0,   0,  12,   4, 
      0,   0,  20,   0,   0,   0, 
     24,   4,   0,   0,  20,   0, 
      0,   0,  36,   4,   0,   0, 
    109,  97, 105, 110,   0, 105, 
    110, 112, 117, 116,   0, 112, 
    111, 115,   0, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  99, 111, 108,   0, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 117, 118, 
      0, 171,  43,   1,   0,   0, 
     48,   1,   0,   0,  64,   1, 
      0,   0,  68,   1,   0,   0, 
     84,   1,   0,   0,  48,   1, 
      0,   0,   5,   0,   0,   0, 
      1,   0,   8,   0,   1,   0, 
      3,   0,  88,   1,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      1,   0, 255, 255, 255, 255, 
      2,   0,   0,   0,   2,   0, 
      3,   0,   4,   0,   5,   0, 
      3,   0,   0,   0,   6,   0, 
      7,   0, 255, 255, 255, 255, 
     43,   1,   0,   0,  68,   1, 
      0,   0,  64,   1,   0,   0, 
     68,   1,   0,   0,  84,   1, 
      0,   0,  48,   1,   0,   0, 
      5,   0,   0,   0,   1,   0, 
     10,   0,   1,   0,   3,   0, 
    164,   1,   0,   0,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      2,   0,   3,   0,  17,   0, 
      0,   0,   4,   0,   5,   0, 
      6,   0,   7,   0,  18,   0, 
      0,   0,   8,   0,   9,   0, 
    255, 255, 255, 255, 111, 117, 
    116, 112, 117, 116,   0, 171, 
      5,   0,   0,   0,   1,   0, 
     10,   0,   1,   0,   3,   0, 
    164,   1,   0,   0,  13,   0, 
      0,   0,   0,   0,   1,   0, 
      2,   0,   3,   0,  14,   0, 
      0,   0,   4,   0,   5,   0, 
      6,   0,   7,   0,  15,   0, 
      0,   0,   8,   0,   9,   0, 
    255, 255, 255, 255,  32,   1, 
      0,   0,  37,   1,   0,   0, 
    112,   1,   0,   0,   3,   0, 
      0,   0, 128,   1,   0,   0, 
      0,   0,   0,   0,  32,   1, 
      0,   0, 188,   1,   0,   0, 
      3,   0,   0,   0, 204,   1, 
      0,   0,   0,   0,   0,   0, 
    240,   1,   0,   0, 248,   1, 
      0,   0,   3,   0,   0,   0, 
      8,   2,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0, 254, 255,  34,   0, 
     67,  84,  65,  66,  28,   0, 
      0,   0,  91,   0,   0,   0, 
      0,   3, 254, 255,   1,   0, 
      0,   0,  28,   0,   0,   0, 
      5,   1,   0,   0,  84,   0, 
      0,   0,  48,   0,   0,   0, 
      2,   0,   0,   0,   4,   0, 
      0,   0,  68,   0,   0,   0, 
      0,   0,   0,   0, 112, 114, 
    111, 106, 101,  99, 116, 105, 
    111, 110,  95, 109,  97, 116, 
    114, 105, 120,   0, 171, 171, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 118, 115, 
     95,  51,  95,  48,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  49,  48, 
     46,  49,   0, 171,  81,   0, 
      0,   5,   4,   0,  15, 160, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,  15, 144,  31,   0, 
      0,   2,  10,   0,   0, 128, 
      1,   0,  15, 144,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      2,   0,  15, 144,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,  15, 224,  31,   0, 
      0,   2,  10,   0,   0, 128, 
      1,   0,  15, 224,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      2,   0,   3, 224,   5,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 160,   0,   0, 
      0, 144,   5,   0,   0,   3, 
      1,   0,  15, 128,   1,   0, 
    228, 160,   0,   0,  85, 144, 
      2,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      1,   0, 228, 128,   2,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 128,   4,   0, 
      0, 160,   1,   0,   0,   2, 
      1,   0,   2, 128,   4,   0, 
     85, 160,   5,   0,   0,   3, 
      1,   0,  15, 128,   1,   0, 
     85, 128,   3,   0, 228, 160, 
      2,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      1,   0, 228, 128,   1,   0, 
      0,   2,   1,   0,  15, 128, 
      1,   0, 228, 144,   1,   0, 
      0,   2,   2,   0,   3, 128, 
      2,   0, 228, 144,   1,   0, 
      0,   2,   0,   0,  15, 224, 
      0,   0, 228, 128,   1,   0, 
      0,   2,   1,   0,  15, 224, 
      1,   0, 228, 128,   1,   0, 
      0,   2,   2,   0,   3, 224, 
      2,   0, 228, 128, 255, 255, 
      0,   0
};