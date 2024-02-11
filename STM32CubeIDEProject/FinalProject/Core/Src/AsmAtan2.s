.section .data
PI_HALF:
    .float 1.57079632679    @ Define constant for PI/2
NEG_PI_HALF:
    .float -1.57079632679
M_PI:
    .float 3.14159265359       @ Define constant for PI
LIMIT:
	.float 100.0
NEG_HUNDRED: .float -100.0

.section .text
.global asm_atan2

asm_atan2:
    @ Input: s0 (x_cor), s1 (y_cor), r2 (pointer to result)
    @ Output: result in memory pointed by r2
    PUSH {r4-r7}

    vmov s2, s0
    vmov s3, s1
    ldr r6, =LIMIT
    VLDR.F32 S6, [r6]
    vmul.f32 s4, s2, s6
    vdiv.f32 s5, s4, s3

    @ Cap the ratio at 100 or -100
    vmov s7, s6

	ldr r6, =NEG_HUNDRED
    VLDR.F32 S8, [r6]

    vcmp.f32 s5, s7
    vmrs    APSR_nzcv, FPSCR
    bgt cap_positive
    vcmp.f32 s5, s8
    vmrs    APSR_nzcv, FPSCR
    blt cap_negative
    b continue

cap_positive:
    vmov s5, s7
    b continue

cap_negative:
    vmov s5, s8

continue:
	vcvt.s32.f32 s13, s5
	vmov r0, s13
    @ Call arctan_lookup with capped ratio
    bl arctan_lookup
    @ Adjust result based on y_cor sign
    VLDR.F32 s9, [R1]
    vcmp.f32 s9, #0.0
    vmrs    APSR_nzcv, FPSCR
    bgt positive_y
    blt negative_y

positive_y:
    @ y_cor > 0, result already in r3 from arctan_lookup
    b store_result

negative_y:
    @ y_cor < 0, adjust result based on x_cor sign
    vcmp.f32 s0, #0
    vmrs    APSR_nzcv, FPSCR
    blt y_neg_x_neg
    bgt y_neg_x_pos

y_neg_x_neg:
    ldr r6, =M_PI
    VLDR.F32 S10, [r6]
    vsub.f32 s5, s5, s10
    b store_result

y_neg_x_pos:
    ldr r6, =M_PI
    VLDR.F32 s11, [r6]
    vadd.f32 s5, s5, s11

store_result:
    @ Store the result in memory pointed by r2
    vstr.f32 s5, [r2]
    POP {r4-r7}
    BX LR
