/*
 * ControlX
 *
 * Copyright (C) 2016 Excale
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 */

/* Warning: this code is quite horrible.
   We recommend that you do not try to understand it */

extern char __ndless_loader_cx_cas_start__;
extern char __ndless_loader_cx_cas_end__;

extern char __ndless_boot2_prestage__;

extern char __ndless_loader_cx_num_start__;
extern char __ndless_loader_cx_num_end__;

extern char __ndless_loader_cx_cas_dvt_start__;
extern char __ndless_loader_cx_cas_dvt_end__;

extern char __ndless_loader_cx_num_dvt_start__;
extern char __ndless_loader_cx_num_dvt_end__;

extern char __ndless_loader_cm_cas_start__;
extern char __ndless_loader_cm_cas_end__;

extern char __ndless_loader_cm_num_start__;
extern char __ndless_loader_cm_num_end__;

asm(R"controlx(
.globl __ndless_loader_cx_cas_start__
.globl __ndless_loader_cx_cas_end__
.globl __ndless_boot2_prestage__
.globl __ndless_loader_cx_num_start__
.globl __ndless_loader_cx_num_end__
.globl __ndless_loader_cx_cas_dvt_start__
.globl __ndless_loader_cx_cas_dvt_end__
.globl __ndless_loader_cx_num_dvt_start__
.globl __ndless_loader_cx_num_dvt_end__
.globl __ndless_loader_cm_cas_start__
.globl __ndless_loader_cm_cas_end__
.globl __ndless_loader_cm_num_start__
.globl __ndless_loader_cm_num_end__
__ndless_loader_cx_cas_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cx_cas.bin"
__ndless_loader_cx_cas_end__:
__ndless_loader_cx_num_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cx_num.bin"
__ndless_loader_cx_num_end__:
__ndless_loader_cx_cas_dvt_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cx_cas_dvt.bin"
__ndless_loader_cx_cas_dvt_end__:
__ndless_loader_cx_num_dvt_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cx_num_dvt.bin"
__ndless_loader_cx_num_dvt_end__:
__ndless_loader_cm_cas_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cm_cas.bin"
__ndless_loader_cm_cas_end__:
__ndless_loader_cm_num_start__:
LDR    R0, [PC, #8]
LDRD   R2, R3, [PC, #12]
STRD   R2, R3, [R0]
LDR    PC, [PC]
.word 0x11AE1C48
.word 0x1187E99C
LDR    PC, [PC, #-4]
.word 0x11908090
.incbin "ndless_loader_cm_num.bin"
__ndless_loader_cm_num_end__:
__ndless_boot2_prestage__:
LDR    PC, [PC, #-4]
.word 0x11908070
)controlx");

int ndless_loader(image_info * flashimage) {
    const char boot2_str[] = "BOOT2";
    if(!memcmp(flashimage->type, boot2_str, sizeof(boot2_str))) {
		const char boot243_str[] = "4.3.49";
		if(!memcmp(flashimage->version, boot243_str, sizeof(boot243_str))) {
			memcpy((void *)0x1187E994, &__ndless_boot2_prestage__, 2*sizeof(uint32_t));
			uint8_t *ndless_loader_start=0, *ndless_loader_end=0;
			switch(modelid) {
				case 0x0F:
					ndless_loader_start=bootkeys?&__ndless_loader_cx_cas_dvt_start__:&__ndless_loader_cx_cas_start__;
					ndless_loader_end=bootkeys?&__ndless_loader_cx_cas_dvt_end__:&__ndless_loader_cx_cas_end__;
					break;
				case 0x10:
					ndless_loader_start=bootkeys?&__ndless_loader_cx_num_dvt_start__:&__ndless_loader_cx_num_start__;
					ndless_loader_end=bootkeys?&__ndless_loader_cx_num_dvt_end__:&__ndless_loader_cx_num_end__;
					break;
				case 0x11:
					ndless_loader_start=&__ndless_loader_cm_cas_start__;
					ndless_loader_end=&__ndless_loader_cm_cas_end__;
					break;
				case 0x12:
					ndless_loader_start=&__ndless_loader_cm_num_start__;
					ndless_loader_end=&__ndless_loader_cm_num_end__;
					break;
				default:
					return 0;
			}
			if(ndless_loader_end-ndless_loader_start<=0x3D4) {
				memcpy((void *)0x11908070, ndless_loader_start , ndless_loader_end-ndless_loader_start);
				return 1;
			}
		}
	}
	return 0;
}