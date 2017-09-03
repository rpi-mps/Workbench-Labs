//----------------------------------------------
// Lab 4 - Inline Assembly Example - Lab04_asm.c
//----------------------------------------------
//
// Assorted extended assembly examples.
//

#include "Lab04_asm.h"

asm (".global _printf_float"); // enable float printing, alternatively "-u _printf_float" linker option
// replace printf with scanf for scanf equivalent (or use both)

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	printf("GO!\r\n");
	uint32_t a = 2;
	uint32_t b = 3;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t f;

	uint32_t q;
	uint32_t r;
	uint32_t s;
	uint32_t t;

	float float1 = 12.3;
	float float2 = 13.0;
	float float3 = 16.198;

	double float4 = 1234566.200000001;
	double float5 = 2341225.055555555;
	double float6 = 1233333.1751223455;

while(1)
	{
		c = a * b;
		printf("c1: %lu\n", c);

		c += a * b;
		printf("c2: %lu\r\n", c);

		asm("mov %[some], $4" : [some] "=r" (e)); // $ means constant
		printf("e:%lx\r\n", e);
		asm("LDR r3, =0xFFFE6487\n\t"
				"LDR r2, =0x80008F71\n\t"
				"LDR r4, =0xFFFF0003"); // LDRD can do 64-bit, takes 2 registers output, 1 register and offset input
//		asm("LDR r3, =0xFFFE6487"); // Individual statements
//		asm("LDR r2, =0x80008F71");
//		asm("LDR r4, =0xFFFF0003");
		asm("SMMLAR r9, r2, r3, r4"); // r9 = r2 * r3 + r4, signed & rounded
		asm("STR r9, %[some2]" : [some2] "=m" (d));

		printf("R9:%lx\r\n", d);
		asm("mov %[some3], r2" : [some3] "=r" (f));
		printf("R2:%lx\r\n", f);
		printf("Input 3 values:\r\n");
		q = getchar();
		printf("q=%lu\r\n", q);
		r = getchar();
		printf("r=%lu\r\n", r);
		s = getchar();
		printf("s=%lu\r\n", s);
		asm("mov r2, %[regl]" : : [regl] "r" (q)); // input
		asm("mov r10,r2");
		asm("mov r2,r10");
		asm("mov %[some4], r2" : [some4] "=r" (f));
		printf("q: %lu, R2:%lx\r\n", q, f);

//		asm("STM r5,{%[x],%[y],%[z]}" : : [x] "r" (q), [y] "r" (r), [z] "r" (s)); // This doesn't work
//		getchar(); // Pause
// The compiler turns LDR into the MOVs we need.
// When using inline assembly, it's good to do just what you need as best as possible
// and then get out of assembly land so that the compiler can free up the resources
// and continue optimizing the assmebly routines it makes out of the rest of your code.
		asm("LDR r5, %[some1]\n\t"
				"LDR r6, %[some2]\n\t"
				"LDR r7, %[some3]"
				: // No outputs
				: [some1] "m" (q), [some2] "m" (r), [some3] "m" (s) // Desired input variables
				:); // No clobbers
		asm("MLA r8, r5, r6, r7\n\t" // r8 = r5 * 56 + r7
				"STR r8, %[out]" : [out] "=m" (t)
				);

		printf("Output (R8): hex=%lx, dec=%lu\r\n", t, t);

				// Can only use instructions defined in Cortex-M7 PDF
				/* Reliable operations for general purpose use:
				 * LDR, STR, MOV, MOVT
				 * Note that LDR and STR work on WORDS (32-bit), MOV and MOVT are limited to 8/16-bit
				 * Types of instructions:
				 *	MAC, SIMD, Floats,
				 * Ultimately instructions like LDR and STR just get broken down into MOVs (and MOVTs, as needed.)
				 */
		printf("MLA TIME!\r\n");
//		asm("MLA r8, %[x], %[y], %[z]" : : [x] "r" (q), [y] "r" (r), [z] "r" (s));
//		asm("STR r8, %[out]" : [out] "=m" (t));

// The one-liner we wanted.
		asm("MLA %[out], %[xerneas], %[yveltal], %[zygarde]" : [out] "=r" (t) : [xerneas] "r" (q), [yveltal] "r" (r), [zygarde] "r" (s));
		printf("Output: hex=%lx, dec=%lu\r\n", t, t);
// single
		asm("VMLA.F32 %[out], %[xerneas], %[yveltal]" : [out] "=t" (float3) : [xerneas] "t" (float1), [yveltal] "t" (float2));
		printf("Output (float): dec=%f\r\n", float3); // w is doubles (d0-d15), t is singles (s0-s31)
//double
		asm("VMLA.F64 %P[out3], %P[xerneas3], %P[yveltal3]" : [out3] "=w" (float5) : [xerneas3] "w" (float4), [yveltal3] "w" (float6)); // this is fine. printf's having issues
		printf("Output (Double): dec=%e\n", float5);
		HAL_Delay(500);
	}
}
