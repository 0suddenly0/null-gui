#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   sampler2D tex_sampler;
//   float texel_height;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   texel_height c0       1
//   tex_sampler  s0       1
//

    ps_3_0
    def c1, 0.227027029, 1, 2, 0
    def c2, 0, 1.38461542, 3.23076916, 0.31621623
    def c3, 0.0702702701, 0, 0, 0
    defi i0, 2, 0, 0, 0
    dcl_texcoord v0.xy
    dcl_2d s0
    texld r0, v0, s0
    mul r0.xyz, r0, c1.x
    mov r1.x, c1.y
    mov r1.yzw, r0.xxyz
    mov r2.x, r1.x
    rep i0
      add r2.yz, r2.x, -c1
      abs r2.yz, r2
      mov r3.xy, -r2.yzzw
      mov r2.yz, -r2
      add r2.yz, r2, r3.xxyw
      cmp r2.yz, r2, c1.y, c1.w
      mov r2.y, -r2.y
      cmp r2.y, r2.y, c2.x, c2.y
      mov r2.z, -r2.z
      cmp r2.y, r2.z, r2.y, c2.z
      mul r2.y, r2.y, c0.x
      mov r2.y, -r2.y
      add r3.y, r2.y, v0.y
      mov r3.x, v0.x
      texld r3, r3, s0
      add r2.yz, r2.x, -c1
      abs r2.yz, r2
      mov r4.xy, -r2.yzzw
      mov r2.yz, -r2
      add r2.yz, r2, r4.xxyw
      cmp r2.yz, r2, c1.y, c1.w
      mov r2.y, -r2.y
      cmp r2.y, r2.y, c2.x, c2.w
      mov r2.z, -r2.z
      cmp r2.y, r2.z, r2.y, c3.x
      mul r2.yzw, r2.y, r3.xxyz
      add r2.yzw, r1, r2
      add r3.xy, r2.x, -c1.yzzw
      abs r3.xy, r3
      mov r3.zw, -r3.xyxy
      mov r3.xy, -r3
      add r3.xy, r3, r3.zwzw
      cmp r3.xy, r3, c1.y, c1.w
      mov r3.x, -r3.x
      cmp r3.x, r3.x, c2.x, c2.y
      mov r3.y, -r3.y
      cmp r3.x, r3.y, r3.x, c2.z
      mul r3.x, r3.x, c0.x
      add r3.y, r3.x, v0.y
      mov r3.x, v0.x
      texld r3, r3, s0
      add r4.xy, r2.x, -c1.yzzw
      abs r4.xy, r4
      mov r4.zw, -r4.xyxy
      mov r4.xy, -r4
      add r4.xy, r4, r4.zwzw
      cmp r4.xy, r4, c1.y, c1.w
      mov r3.w, -r4.x
      cmp r3.w, r3.w, c2.x, c2.w
      mov r4.x, -r4.y
      cmp r3.w, r4.x, r3.w, c3.x
      mul r3.xyz, r3.w, r3
      add r1.yzw, r2, r3.xxyz
      add r2.x, r2.x, c1.y
    endrep
    mov oC0.xyz, r1.yzww
    mov oC0.w, r0.w

// approximately 66 instruction slots used (3 texture, 63 arithmetic)
#endif

const BYTE blur_y[] =
{
      0,   3, 255, 255, 254, 255, 
     45,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 135,   0, 
      0,   0,   0,   3, 255, 255, 
      2,   0,   0,   0,  28,   0, 
      0,   0,   4,   1,   0,   0, 
    128,   0,   0,   0,  68,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
     96,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
    112,   0,   0,   0,   0,   0, 
      0,   0, 116, 101, 120,  95, 
    115,  97, 109, 112, 108, 101, 
    114,   0,   4,   0,  12,   0, 
      1,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    116, 101, 120, 101, 108,  95, 
    104, 101, 105, 103, 104, 116, 
      0, 171, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 112, 115,  95,  51, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  49,  48,  46,  49, 
      0, 171,  81,   0,   0,   5, 
      1,   0,  15, 160, 198, 121, 
    104,  62,   0,   0, 128,  63, 
      0,   0,   0,  64,   0,   0, 
      0,   0,  81,   0,   0,   5, 
      2,   0,  15, 160,   0,   0, 
      0,   0,  20,  59, 177,  63, 
    236, 196,  78,  64,  24, 231, 
    161,  62,  81,   0,   0,   5, 
      3,   0,  15, 160, 220, 233, 
    143,  61,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  48,   0,   0,   5, 
      0,   0,  15, 240,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  31,   0,   0,   2, 
      5,   0,   0, 128,   0,   0, 
      3, 144,  31,   0,   0,   2, 
      0,   0,   0, 144,   0,   8, 
     15, 160,  66,   0,   0,   3, 
      0,   0,  15, 128,   0,   0, 
    228, 144,   0,   8, 228, 160, 
      5,   0,   0,   3,   0,   0, 
      7, 128,   0,   0, 228, 128, 
      1,   0,   0, 160,   1,   0, 
      0,   2,   1,   0,   1, 128, 
      1,   0,  85, 160,   1,   0, 
      0,   2,   1,   0,  14, 128, 
      0,   0, 144, 128,   1,   0, 
      0,   2,   2,   0,   1, 128, 
      1,   0,   0, 128,  38,   0, 
      0,   1,   0,   0, 228, 240, 
      2,   0,   0,   3,   2,   0, 
      6, 128,   2,   0,   0, 128, 
      1,   0, 228, 161,  35,   0, 
      0,   2,   2,   0,   6, 128, 
      2,   0, 228, 128,   1,   0, 
      0,   2,   3,   0,   3, 128, 
      2,   0, 233, 129,   1,   0, 
      0,   2,   2,   0,   6, 128, 
      2,   0, 228, 129,   2,   0, 
      0,   3,   2,   0,   6, 128, 
      2,   0, 228, 128,   3,   0, 
    208, 128,  88,   0,   0,   4, 
      2,   0,   6, 128,   2,   0, 
    228, 128,   1,   0,  85, 160, 
      1,   0, 255, 160,   1,   0, 
      0,   2,   2,   0,   2, 128, 
      2,   0,  85, 129,  88,   0, 
      0,   4,   2,   0,   2, 128, 
      2,   0,  85, 128,   2,   0, 
      0, 160,   2,   0,  85, 160, 
      1,   0,   0,   2,   2,   0, 
      4, 128,   2,   0, 170, 129, 
     88,   0,   0,   4,   2,   0, 
      2, 128,   2,   0, 170, 128, 
      2,   0,  85, 128,   2,   0, 
    170, 160,   5,   0,   0,   3, 
      2,   0,   2, 128,   2,   0, 
     85, 128,   0,   0,   0, 160, 
      1,   0,   0,   2,   2,   0, 
      2, 128,   2,   0,  85, 129, 
      2,   0,   0,   3,   3,   0, 
      2, 128,   2,   0,  85, 128, 
      0,   0,  85, 144,   1,   0, 
      0,   2,   3,   0,   1, 128, 
      0,   0,   0, 144,  66,   0, 
      0,   3,   3,   0,  15, 128, 
      3,   0, 228, 128,   0,   8, 
    228, 160,   2,   0,   0,   3, 
      2,   0,   6, 128,   2,   0, 
      0, 128,   1,   0, 228, 161, 
     35,   0,   0,   2,   2,   0, 
      6, 128,   2,   0, 228, 128, 
      1,   0,   0,   2,   4,   0, 
      3, 128,   2,   0, 233, 129, 
      1,   0,   0,   2,   2,   0, 
      6, 128,   2,   0, 228, 129, 
      2,   0,   0,   3,   2,   0, 
      6, 128,   2,   0, 228, 128, 
      4,   0, 208, 128,  88,   0, 
      0,   4,   2,   0,   6, 128, 
      2,   0, 228, 128,   1,   0, 
     85, 160,   1,   0, 255, 160, 
      1,   0,   0,   2,   2,   0, 
      2, 128,   2,   0,  85, 129, 
     88,   0,   0,   4,   2,   0, 
      2, 128,   2,   0,  85, 128, 
      2,   0,   0, 160,   2,   0, 
    255, 160,   1,   0,   0,   2, 
      2,   0,   4, 128,   2,   0, 
    170, 129,  88,   0,   0,   4, 
      2,   0,   2, 128,   2,   0, 
    170, 128,   2,   0,  85, 128, 
      3,   0,   0, 160,   5,   0, 
      0,   3,   2,   0,  14, 128, 
      2,   0,  85, 128,   3,   0, 
    144, 128,   2,   0,   0,   3, 
      2,   0,  14, 128,   1,   0, 
    228, 128,   2,   0, 228, 128, 
      2,   0,   0,   3,   3,   0, 
      3, 128,   2,   0,   0, 128, 
      1,   0, 233, 161,  35,   0, 
      0,   2,   3,   0,   3, 128, 
      3,   0, 228, 128,   1,   0, 
      0,   2,   3,   0,  12, 128, 
      3,   0,  68, 129,   1,   0, 
      0,   2,   3,   0,   3, 128, 
      3,   0, 228, 129,   2,   0, 
      0,   3,   3,   0,   3, 128, 
      3,   0, 228, 128,   3,   0, 
    238, 128,  88,   0,   0,   4, 
      3,   0,   3, 128,   3,   0, 
    228, 128,   1,   0,  85, 160, 
      1,   0, 255, 160,   1,   0, 
      0,   2,   3,   0,   1, 128, 
      3,   0,   0, 129,  88,   0, 
      0,   4,   3,   0,   1, 128, 
      3,   0,   0, 128,   2,   0, 
      0, 160,   2,   0,  85, 160, 
      1,   0,   0,   2,   3,   0, 
      2, 128,   3,   0,  85, 129, 
     88,   0,   0,   4,   3,   0, 
      1, 128,   3,   0,  85, 128, 
      3,   0,   0, 128,   2,   0, 
    170, 160,   5,   0,   0,   3, 
      3,   0,   1, 128,   3,   0, 
      0, 128,   0,   0,   0, 160, 
      2,   0,   0,   3,   3,   0, 
      2, 128,   3,   0,   0, 128, 
      0,   0,  85, 144,   1,   0, 
      0,   2,   3,   0,   1, 128, 
      0,   0,   0, 144,  66,   0, 
      0,   3,   3,   0,  15, 128, 
      3,   0, 228, 128,   0,   8, 
    228, 160,   2,   0,   0,   3, 
      4,   0,   3, 128,   2,   0, 
      0, 128,   1,   0, 233, 161, 
     35,   0,   0,   2,   4,   0, 
      3, 128,   4,   0, 228, 128, 
      1,   0,   0,   2,   4,   0, 
     12, 128,   4,   0,  68, 129, 
      1,   0,   0,   2,   4,   0, 
      3, 128,   4,   0, 228, 129, 
      2,   0,   0,   3,   4,   0, 
      3, 128,   4,   0, 228, 128, 
      4,   0, 238, 128,  88,   0, 
      0,   4,   4,   0,   3, 128, 
      4,   0, 228, 128,   1,   0, 
     85, 160,   1,   0, 255, 160, 
      1,   0,   0,   2,   3,   0, 
      8, 128,   4,   0,   0, 129, 
     88,   0,   0,   4,   3,   0, 
      8, 128,   3,   0, 255, 128, 
      2,   0,   0, 160,   2,   0, 
    255, 160,   1,   0,   0,   2, 
      4,   0,   1, 128,   4,   0, 
     85, 129,  88,   0,   0,   4, 
      3,   0,   8, 128,   4,   0, 
      0, 128,   3,   0, 255, 128, 
      3,   0,   0, 160,   5,   0, 
      0,   3,   3,   0,   7, 128, 
      3,   0, 255, 128,   3,   0, 
    228, 128,   2,   0,   0,   3, 
      1,   0,  14, 128,   2,   0, 
    228, 128,   3,   0, 144, 128, 
      2,   0,   0,   3,   2,   0, 
      1, 128,   2,   0,   0, 128, 
      1,   0,  85, 160,  39,   0, 
      0,   0,   1,   0,   0,   2, 
      0,   8,   7, 128,   1,   0, 
    249, 128,   1,   0,   0,   2, 
      0,   8,   8, 128,   0,   0, 
    255, 128, 255, 255,   0,   0
};
