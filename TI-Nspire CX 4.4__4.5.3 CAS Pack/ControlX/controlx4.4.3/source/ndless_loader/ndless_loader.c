/*
 * ControlX
 *
 * Forked from nLaunchy https://github.com/Excale/nLaunchy
 *
 * Copyright (C) 2012-2013 nLaunch team
 * Copyright (C) 2013      nLaunch CX guy
 * Copyright (C) 2013-2016 Excale
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

// see Makefile for included Ndless installers

#include "../types.h"
#include "../patchos.h"

#ifdef DVT // development -> disable production-only OSes
	#undef OS_301
	#undef OS_302_1791
	#undef OS_302_1793
	#undef OS_310_0
	#undef OS_310_157
	#undef OS_310_236
	#undef OS_310_392
	#undef OS_320_1212
	#undef OS_320_1219
	#undef OS_323
	#undef OS_324
	#undef OS_330_538
	#undef OS_360_546_550
	#undef OS_390_461
	#undef OS_390_463
	#undef OS_391
	#undef OS_400
	#undef OS_402
	#undef OS_403
	#undef OS_42
	#undef OS_430_547
	#undef OS_430_702
	#undef OS_440_531
	#undef OS_440_532
#else // production -> disable development-only OSes
	#undef OS_300_0
	#undef OS_300_1045
	#undef OS_300_1503
	#undef OS_320_776
	#undef OS_320_1030
	#undef OS_320_1180
	#undef OS_330_218
	#undef OS_360_427
	#undef OS_360_521
#endif

#ifndef CAS // non-CAS -> disable CAS-only OSes
	#undef OS_310_157
	#undef OS_330_218
	#undef OS_330_538
	#undef OS_360_427
	#undef OS_360_521
	#undef OS_430_547
	#undef OS_440_531
#else // CAS -> disable numeric-only OSes
	#undef OS_300_0
	#undef OS_300_1045
	#undef OS_300_1503
	#undef OS_310_0
	#undef OS_310_236
#endif

#ifdef CM // CM -> disable CX-only OSes
	#undef OS_440_532
	#undef OS_440_531
	#undef OS_430_702
	#undef OS_430_547
	#undef OS_42
	#undef OS_403
	#undef OS_402
	#undef OS_400
	#undef OS_391
	#undef OS_390_463
	#undef OS_390_461
	#undef OS_360_546_550
	#undef OS_360_521
	#undef OS_360_427
	#undef OS_330_538
	#undef OS_330_218
	#undef OS_324
	#undef OS_323
	#undef OS_302_1793
	#undef OS_302_1791
	#undef OS_301
	#undef OS_300_0
	#undef OS_300_1045
	#undef OS_300_1503
#else // CX -> disable CM-only OSes
	#undef OS_310_0
	#undef OS_310_157
	#undef OS_310_236
#endif

#ifdef OS_310_392
	#define NDLESS_31
#endif
#ifdef OS_360_546_550
	#define NDLESS_36
#endif
#if defined(OS_390_461) || defined(OS_390_463)
	#undef NDLESS_390 // non working (incomplete ndless_resources file)
#endif
#ifdef OS_391
	#define NDLESS_391
#endif
#ifdef OS_400
	#define NDLESS_400
#endif
#ifdef OS_403
	#define NDLESS_403
#endif
#ifdef OS_42
	#define NDLESS_42
#endif
#ifdef OS_430_702
	#define NDLESS_43
#endif
#ifdef OS_440_532
	#define NDLESS_44
#endif

#if	defined(NDLESS_31) || defined(NDLESS_36) || defined(NDLESS_390) || defined(NDLESS_391) || defined(NDLESS_400) || defined(NDLESS_403) || defined(NDLESS_42) || defined(NDLESS_43) || defined(NDLESS_44)
	#define PATCH_NDLESS
#endif
#if defined(OS_300_0) || defined(OS_300_1045) || defined(OS_300_1503) || defined(OS_301) || defined(OS_310_0) || defined(OS_310_157) || defined(OS_310_236) || defined(OS_310_392) || defined(OS_320_776) || defined(OS_320_1030) || defined(OS_320_1180) || defined(OS_320_1212) || defined(OS_320_1219) || defined(OS_323) || defined(OS_324) || defined(OS_330_218) || defined(OS_330_538) || defined(OS_360_427) || defined(OS_360_521) || defined(OS_360_546_550) || defined(OS_390_461)|| defined(OS_390_463) || defined(OS_391) || defined(OS_400)
	#define PATCH_LT_400
#endif
#if defined(OS_402) || (defined(OS_403) && !defined(NDLESS_403)) || (defined(OS_42) && !defined(NDLESS_42)) || defined(OS_430_547) || (defined(OS_430_702) && !defined(NDLESS_43)) || defined(OS_440_531) || (defined(OS_440_532) && !defined(NDLESS_44))
	#define PATCH_GT_402_NON_NDLESS
#endif
#if defined(OS_390_461) || defined(OS_390_463) || defined(OS_391) || defined(OS_400) || defined(OS_402) || defined(OS_403) || defined(OS_42) || defined(OS_430_547) || defined(OS_430_702) || defined(OS_440_531) || defined(OS_440_532)
	#define PATCH_GT_39
#endif
#if defined(OS_360_427) || defined(OS_360_521) || defined(OS_360_546_550) || defined(PATCH_GT_39)
	#define PATCH_GT_36
#endif

//	os:	num					CAS
#ifdef PATCH_GT_36
	static const uint32_t selfd_patch_addr[] = { // patch discrepency for OS 3.6+
		#ifdef OS_360_427
				SELFD_CXC360_427,
		#endif
		#ifdef OS_360_521
				SELFD_CXC360_521,
		#endif
		#ifdef OS_360_546_550
			#ifndef CAS
				SELFD_CX360_5XX,
			#else
				SELFD_CXC360_546_550,
			#endif
		#endif
		#ifdef OS_390_461
			#ifndef CAS
				SELFD_CX390_461,
			#else
				SELFD_CXC390_461,
			#endif
		#endif
		#ifdef OS_390_463
			#ifndef CAS
				SELFD_CX390_463,
			#else
				SELFD_CXC390_463,
			#endif
		#endif
		#ifdef OS_391
			#ifndef CAS
				SELFD_CX391_38,
			#else
				SELFD_CXC391_38,
			#endif
		#endif
		#ifdef OS_400
			#ifndef CAS
				SELFD_CX400_235,
			#else
				SELFD_CXC400_235,
			#endif
		#endif
		#ifdef OS_402
			#ifndef CAS
				SELFD_CX402_27,
			#else
				SELFD_CXC402_27,
			#endif
		#endif
		#ifdef OS_403
			#ifndef CAS
				SELFD_CX403_29,
			#else
				SELFD_CXC403_29,
			#endif
		#endif
		#ifdef OS_42
			#ifndef CAS
				SELFD_CX420_532,
			#else
				SELFD_CXC420_532,
			#endif
		#endif
		#ifdef OS_430_547
			#ifdef CAS
				SELFD_CXC430_547,
			#endif
		#endif
		#ifdef OS_430_702
			#ifndef CAS
				SELFD_CX430_702,
			#else
				SELFD_CXC430_702,
			#endif
		#endif
		#ifdef OS_440_531
			#ifdef CAS
				SELFD_CXC440_531,
			#endif
		#endif
		#ifdef OS_440_532
			#ifndef CAS
				SELFD_CX440_532,
			#else
				SELFD_CXC440_532,
			#endif
		#endif
	};
	static const uint32_t asic1_patch_addr[] = {
		#ifdef OS_360_427
				ASIC1_CXC360_427,
		#endif
		#ifdef OS_360_521
				ASIC1_CXC360_521,
		#endif
		#ifdef OS_360_546_550
			#ifndef CAS
				ASIC1_CX360_5XX,
			#else
				ASIC1_CXC360_546_550,
			#endif
		#endif
		#if defined(OS_390_461) || defined(OS_390_463)
			#ifndef CAS
				ASIC1_CX390_46X,
			#else
				ASIC1_CXC390_46X,
			#endif
		#endif
		#ifdef OS_391
			#ifndef CAS
				ASIC1_CX391_38,
			#else
				ASIC1_CXC391_38,
			#endif
		#endif
		#ifdef OS_400
			#ifndef CAS
				ASIC1_CX400_235,
			#else
				ASIC1_CXC400_235,
			#endif
		#endif
		#ifdef OS_402
			#ifndef CAS
				ASIC1_CX402_27,
			#else
				ASIC1_CXC402_27,
			#endif
		#endif
		#ifdef OS_403
			#ifndef CAS
				ASIC1_CX403_29,
			#else
				ASIC1_CXC403_29,
			#endif
		#endif
		#ifdef OS_42
			#ifndef CAS
				ASIC1_CX420_532,
			#else
				ASIC1_CXC420_532,
			#endif
		#endif
		#ifdef OS_430_547
			#ifdef CAS
				ASIC1_CXC430_547,
			#endif
		#endif
		#ifdef OS_430_702
			#ifndef CAS
				ASIC1_CX430_702,
			#else
				ASIC1_CXC430_702,
			#endif
		#endif
		#ifdef OS_440_531
			#ifdef CAS
				ASIC1_CXC440_53X,
			#endif
		#endif
		#if defined(OS_440_532) || defined(OS_440_531)
			#ifndef CAS
				ASIC1_CX440_532,
			#else
				ASIC1_CXC440_53X,
			#endif
		#endif
	};
#endif
#ifdef PATCH_GT_39
	static const uint32_t asic2_patch_addr[] = {
		#ifdef OS_390_461
			#ifndef CAS
				ASIC2_CX390_461,
			#else
				ASIC2_CXC390_461,
			#endif
		#endif
		#ifdef OS_390_463
			#ifndef CAS
				ASIC2_CX390_463,
			#else
				ASIC2_CXC390_463,
			#endif
		#endif
		#ifdef OS_391
			#ifndef CAS
				ASIC2_CX391_38,
			#else
				ASIC2_CXC391_38,
			#endif
		#endif
		#ifdef OS_400
			#ifndef CAS
				ASIC2_CX400_235,
			#else
				ASIC2_CXC400_235,
			#endif
		#endif
		#ifdef OS_402
			#ifndef CAS
				ASIC2_CX402_27,
			#else
				ASIC2_CXC402_27,
			#endif
		#endif
		#ifdef OS_403
			#ifndef CAS
				ASIC2_CX403_29,
			#else
				ASIC2_CXC403_29,
			#endif
		#endif
		#ifdef OS_42
			#ifndef CAS
				ASIC2_CX420_532,
			#else
				ASIC2_CXC420_532,
			#endif
		#endif
		#ifdef OS_430_547
			#ifdef CAS
				ASIC2_CXC430_547,
			#endif
		#endif
		#ifdef OS_430_702
			#ifndef CAS
				ASIC2_CX430_702,
			#else
				ASIC2_CXC430_702,
			#endif
		#endif
		#ifdef OS_440_531
			#ifdef CAS
				ASIC2_CXC440_531,
			#endif
		#endif
		#ifdef OS_440_532
			#ifndef CAS
				ASIC2_CX440_532,
			#else
				ASIC2_CXC440_532,
			#endif
		#endif
	};
#endif
#ifdef PATCH_GT_402_NON_NDLESS
	static const uint32_t boot2upd_patch_addr[] = { // patch Boot2upd for OS 3.6+ when no Ndless
	#ifdef OS_402
		#ifndef CAS
			BOOT2UPD_CX402_27,
		#else
			BOOT2UPD_CXC402_27,
		#endif
	#endif
	#if defined(OS_403) && !defined(NDLESS_403)
		#ifndef CAS
			BOOT2UPD_CX403_29,
		#else
			BOOT2UPD_CXC403_29,
		#endif
	#endif
	#if defined(OS_42) && !defined(NDLESS_42)
		#ifndef CAS
			BOOT2UPD_CX420_532,
		#else
			BOOT2UPD_CXC420_532,
		#endif
	#endif
	#ifdef OS_430_547
			BOOT2UPD_CXC430_547,
	#endif
	#if defined(OS_430_702) && !defined(NDLESS_43)
		#ifndef CAS
			BOOT2UPD_CX430_702,
		#else
			BOOT2UPD_CXC430_702,
		#endif
	#endif
	#ifdef OS_440_531
			BOOT2UPD_CXC440_53X,
	#endif
	#if defined(OS_440_532) && !defined(NDLESS_44)
		#ifndef CAS
			BOOT2UPD_CX440_532,
		#else
			BOOT2UPD_CXC440_53X,
		#endif
	#endif
	};
#endif
#ifdef PATCH_LT_400
	static const uint32_t nboot_patch_addr[] = { // nBoot patch for OS 4.0.0-
		#ifdef OS_300_0
				NBOOT_CX300_0,
		#endif
		#ifdef OS_300_1045
				NBOOT_CX300_1045,
		#endif
		#ifdef OS_300_1503
				NBOOT_CX300_1503,
		#endif
		#ifdef OS_301
			#ifndef CAS
				NBOOT_CX301_1753,
			#else
				NBOOT_CXC301_1753,
			#endif
		#endif
		#ifndef CAS
			#ifdef OS_302_1791
				NBOOT_CX302_1791,
			#endif
			#ifdef OS_302_1793
				NBOOT_CX302_1793,
			#endif
		#else
			#ifdef OS_302_1791
				NBOOT_CXC302_1791,
			#endif
			#ifdef OS_302_1793
				NBOOT_CXC302_1793,
			#endif
		#endif
		#ifdef OS_310_0
				NBOOT_CM310_0,
		#endif
		#ifdef OS_310_157
				NBOOT_CMC310_157,
		#endif
		#ifdef OS_310_236
				NBOOT_CM310_236,
		#endif
		#ifdef OS_310_392
			#ifndef CM
				#ifndef CAS
				NBOOT_CX310_392,
				#else
				NBOOT_CXC310_392,
				#endif
			#else
				#ifndef CAS
				NBOOT_CM310_392,
				#else
				NBOOT_CMC310_392,
				#endif
			#endif
		#endif
		#if defined(OS_320_776)
			#ifndef CAS
				NBOOT_CX320_776,
			#else
				NBOOT_CXC320_776,
			#endif
		#endif
		#if defined(OS_320_1030)
			#ifndef CAS
				NBOOT_CX320_1030,
			#else
				NBOOT_CXC320_1030,
			#endif
		#endif
		#if defined(OS_320_1180)
			#ifndef CAS
				NBOOT_CX320_1180,
			#else
				NBOOT_CXC320_1180,
			#endif
		#endif
		#if defined(OS_320_1212) || defined(OS_320_1219)
			#ifndef CM
				#ifndef CAS
				NBOOT_CX320_121X,
				#else
				NBOOT_CXC320_121X,
				#endif
			#else
				#ifndef CAS
				NBOOT_CM320_121X,
				#else
				NBOOT_CMC320_121X,
				#endif
			#endif
		#endif
		#if defined(OS_323) || defined(OS_324)
			#ifndef CAS
				NBOOT_CX32X_123X,
			#else
				NBOOT_CXC32X_123X,
			#endif
		#endif
		#if defined(OS_330_218)
				NBOOT_CXC330_218,
		#endif
		#if defined(OS_330_538)
				NBOOT_CXC330_538,
		#endif
		#ifdef OS_360_427
				NBOOT_CXC360_427,
		#endif
		#ifdef OS_360_521
				NBOOT_CXC360_521,
		#endif
		#ifdef OS_360_546_550
			#ifndef CAS
				NBOOT_CX360_5XX,
			#else
				NBOOT_CXC360_546_550,
			#endif
		#endif
		#if defined(OS_390_461) || defined(OS_390_463)
			#ifndef CAS
				NBOOT_CX390_46X,
			#else
				NBOOT_CXC390_46X,
			#endif
		#endif
		#ifdef OS_391
			#ifndef CAS
				NBOOT_CX391_38,	
			#else
				NBOOT_CXC391_38,
			#endif
		#endif
		#ifdef OS_400
			#ifndef CAS
				NBOOT_CX400_235,
			#else
				NBOOT_CXC400_235,
			#endif
		#endif
    };
#endif
#ifdef PATCH_NDLESS
    static const uint32_t ndless_load_addr[] = {
		#ifdef NDLESS_31
			#ifndef CM
				#ifndef CAS
				BOOT2UPD_CX310_392,
				#else
				BOOT2UPD_CXC310_392,
				#endif
			#else
				#ifndef CAS
				BOOT2UPD_CM310_392,
				#else
				BOOT2UPD_CMC310_392,
				#endif
			#endif
		#endif
		#ifdef NDLESS_36
			#ifndef CAS
				BOOT2UPD_CX360_5XX,
			#else
				BOOT2UPD_CXC360_546_550,
			#endif
		#endif
		#ifdef NDLESS_390
			#ifndef CAS
				BOOT2UPD_CX390_46X,
			#else
				BOOT2UPD_CXC390_46X,
			#endif
		#endif
		#ifdef NDLESS_391
			#ifndef CAS
				BOOT2UPD_CX391_38,
			#else
				BOOT2UPD_CXC391_38,
			#endif
		#endif
		#ifdef NDLESS_400
			#ifndef CAS
				BOOT2UPD_CX400_235,
			#else
				BOOT2UPD_CXC400_235,
			#endif
		#endif
		#ifdef NDLESS_403
			#ifndef CAS
				BOOT2UPD_CX403_29,
			#else
				BOOT2UPD_CXC403_29,
			#endif
		#endif
		#ifdef NDLESS_42
			#ifndef CAS
				BOOT2UPD_CX420_532,
			#else
				BOOT2UPD_CXC420_532,
			#endif
		#endif
		#ifdef NDLESS_43
			#ifndef CAS
				BOOT2UPD_CX430_702,
			#else
				BOOT2UPD_CXC430_702,
			#endif
		#endif
		#ifdef NDLESS_44
			#ifndef CAS
				BOOT2UPD_CX440_532,
			#else
				BOOT2UPD_CXC440_53X,
			#endif
		#endif
    };
    static const uint32_t ndless_loader_model_specific[][5] = {
        /* fopen,				stat,		 			malloc,					fread,				fclose */
		#ifdef NDLESS_31
			#ifndef CM
				#ifndef CAS
				{FOPEN_CX310_392,		STAT_CX310_392,			MALLOC_CX310_392,		FREAD_CX310_392,		FCLOSE_CX310_392},
				#else
				{FOPEN_CXC310_392,		STAT_CXC310_392,		MALLOC_CXC310_392,		FREAD_CXC310_392,		FCLOSE_CXC310_392},
				#endif
			#else
				#ifndef CAS
				{FOPEN_CM310_392,		STAT_CM310_392,			MALLOC_CM310_392,		FREAD_CM310_392,		FCLOSE_CM310_392},
				#else
				{FOPEN_CMC310_392,		STAT_CMC310_392,		MALLOC_CMC310_392,		FREAD_CMC310_392,		FCLOSE_CMC310_392},
				#endif
			#endif
		#endif
		#ifdef NDLESS_36
			#ifndef CAS
				{FOPEN_CX360_5XX, 		STAT_CX360_5XX,			MALLOC_CX360_5XX,		FREAD_CX360_5XX,		FCLOSE_CX360_5XX},
			#else
				{FOPEN_CXC360_546_550, 	STAT_CXC360_546_550,	MALLOC_CXC360_546_550,	FREAD_CXC360_546_550,	FCLOSE_CXC360_546_550},
			#endif
		#endif
		#ifdef NDLESS_390
			#ifndef CAS
				{FOPEN_CX390_46X,		STAT_CX390_46X,			MALLOC_CX390_46X,		FREAD_CX390_46X,		FCLOSE_CX390_46X},
			#else
				{FOPEN_CXC390_46X,		STAT_CXC390_46X,		MALLOC_CXC390_46X,		FREAD_CXC390_46X,		FCLOSE_CXC390_46X},
			#endif
		#endif
		#ifdef NDLESS_391
			#ifndef CAS
				{FOPEN_CX391_38,		STAT_CX391_38,			MALLOC_CX391_38,		FREAD_CX391_38,			FCLOSE_CX391_38},
			#else
				{FOPEN_CXC391_38,		STAT_CXC391_38,			MALLOC_CXC391_38,		FREAD_CXC391_38,		FCLOSE_CXC391_38},
			#endif
		#endif
		#ifdef NDLESS_400
			#ifndef CAS
				{FOPEN_CX400_235,		STAT_CX400_235,			MALLOC_CX400_235,		FREAD_CX400_235,		FCLOSE_CX400_235},
			#else
				{FOPEN_CXC400_235,		STAT_CXC400_235,		MALLOC_CXC400_235,		FREAD_CXC400_235,		FCLOSE_CXC400_235},
			#endif
		#endif
		#ifdef NDLESS_403
			#ifndef CAS
				{FOPEN_CX403_29,		STAT_CX403_29,			MALLOC_CX403_29,		FREAD_CX403_29,			FCLOSE_CX403_29},
			#else
				{FOPEN_CXC403_29,		STAT_CXC403_29,			MALLOC_CXC403_29,		FREAD_CXC403_29,		FCLOSE_CXC403_29},
			#endif
		#endif
		#ifdef NDLESS_42
			#ifndef CAS
				{FOPEN_CX420_532,		STAT_CX420_532,			MALLOC_CX420_532,		FREAD_CX420_532,		FCLOSE_CX420_532},
			#else
				{FOPEN_CXC420_532,		STAT_CXC420_532,		MALLOC_CXC420_532,		FREAD_CXC420_532,		FCLOSE_CXC420_532},
			#endif
		#endif
		#ifdef NDLESS_43
			#ifndef CAS
				{FOPEN_CX430_702,		STAT_CX430_702,			MALLOC_CX430_702,		FREAD_CX430_702,		FCLOSE_CX430_702},
			#else
				{FOPEN_CXC430_702,		STAT_CXC430_702,		MALLOC_CXC430_702,		FREAD_CXC430_702,		FCLOSE_CXC430_702},
			#endif
		#endif
		#ifdef NDLESS_44
			#ifndef CAS
				{FOPEN_CX440_532,		STAT_CX440_532,			MALLOC_CX440_532,		FREAD_CX440_532,		FCLOSE_CX440_532},
			#else
				{FOPEN_CXC440_532,		STAT_CXC440_532,		MALLOC_CXC440_532,		FREAD_CXC440_532,		FCLOSE_CXC440_532},
			#endif
		#endif
	};
#endif
void __attribute__((naked)) ndless_loader(void) {
	uint32_t id=*((volatile uint32_t *)0x10000020);
#if defined(PATCH_GT_402_NON_NDLESS) || defined(PATCH_NDLESS)
	int32_t os_b2upd=-1;
#endif
#if defined(PATCH_LT_400) || defined(PATCH_GT_402_NON_NDLESS) || defined(PATCH_GT_36)
	#ifdef PATCH_LT_400
		int32_t os_nboot=-1;
	#endif
	#ifdef PATCH_GT_36
		int32_t os_selfd=-1;
		int32_t os_asic1=-1;
	#endif
	#ifdef PATCH_GT_39
		int32_t os_asic2=-1;
	#endif
		switch (id) {
			#ifdef OS_440_532
				#ifdef CAS
					case CXC440_532:
				#else
					case CX440_532:
				#endif
				#ifndef NDLESS_44
						os_b2upd++;
				#endif
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_440_531
					case CXC440_531:
						os_selfd++;
						os_b2upd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_430_702
				#ifdef CAS
					case CXC430_702:
				#else
					case CX430_702:
				#endif
				#ifndef NDLESS_43
						os_b2upd++;
				#endif
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_430_547
					case CXC430_547:
						os_selfd++;
						os_b2upd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_42
				#ifdef CAS
					case CXC420_532:
				#else
					case CX420_532:
				#endif
				#ifndef NDLESS_42
						os_b2upd++;
				#endif
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_403
				#ifdef CAS
					case CXC403_29:
				#else
					case CX403_29:
				#endif
				#ifndef NDLESS_403
						os_b2upd++;
				#endif
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_402
				#ifdef CAS
					case CXC402_27:
				#else
					case CX402_27:
				#endif
						os_b2upd++;
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef PATCH_GT_402_NON_NDLESS
				*(uint32_t*)boot2upd_patch_addr[os_b2upd]=0xE12FFF1E;
			#endif
			#if defined(PATCH_GT_402_NON_NDLESS) || defined(PATCH_GT_36) || defined(PATCH_GT_39)
				#ifdef PATCH_LT_400
					os_nboot=-127;
				#endif
			#endif
			#ifdef OS_400
				#ifdef CAS
					case CXC400_235:
				#else
					case CX400_235:
				#endif
						os_nboot++;
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef OS_391
				#ifdef CAS
					case CXC391_38:
				#else
					case CX391_38:
				#endif
						os_nboot++;
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#if defined(OS_390_461) || defined(OS_390_463)
				#ifdef CAS
					case CXC390_46X:
						#if defined(OS_390_461) && defined(OS_390_463)
							if(*((volatile uint32_t *)(0x10000020)) != CXC390_46X || *((volatile uint32_t *)(0x10A0A084)) == 0x6F68702F) { // not 3.9.0 or 463
								os_selfd++;
								os_asic2++;
							}
						#endif
				#else
					case CX390_46X:
						#if defined(OS_390_461) && defined(OS_390_463)
							if(*((volatile uint32_t *)(0x10000020)) != CX390_46X || *((volatile uint32_t *)(0x109A9604)) == 0x6F68702F) { // not 3.9.0 or 463
								os_selfd++;
								os_asic2++;
							}
						#endif
				#endif
						os_nboot++;
						os_selfd++;
						os_asic1++;
						os_asic2++;
			#endif
			#ifdef PATCH_GT_39
				#ifdef CAS
						*(uint32_t*)(asic2_patch_addr[os_asic2]+0x118)=0x04000001; // CX CAS ASIC2
				#else
						*(uint32_t*)(asic2_patch_addr[os_asic2]+0x118)=0; // CX ASIC2
				#endif
						*(uint16_t*)(asic2_patch_addr[os_asic2])=0x2110;
						*(uint32_t*)(asic2_patch_addr[os_asic2]+8)=0;
			#endif
			#ifdef OS_360_546_550
				#ifdef CAS
					case CXC360_546_550:
				#else
					case CX360_5XX:
								
				#endif
						os_nboot++;
						os_selfd++;
						os_asic1++;
			#endif
			#ifdef OS_360_521
					case CXC360_521:
						os_nboot++;
						os_selfd++;
						os_asic1++;
			#endif
			#ifdef OS_360_427
					case CXC360_427:
						os_nboot++;
						os_selfd++;
						os_asic1++;
			#endif
			#ifdef PATCH_GT_36
				#ifdef CAS
						*(uint32_t*)(asic1_patch_addr[os_asic1]+0x17)=0xE3A04341; // CX CAS ASIC2
						*(uint32_t*)(asic1_patch_addr[os_asic1]+0x48B)=0x00010105; // CX CAS ASIC1
				#else
						*(uint32_t*)(asic1_patch_addr[os_asic1]+0x17)=0xE3A04000; // CX ASIC2
						*(uint32_t*)(asic1_patch_addr[os_asic1]+0x48B)=0; // CX ASIC1
				#endif
						*(uint32_t*)selfd_patch_addr[os_selfd]=0xE12FFF1E;
						*(uint8_t*)asic1_patch_addr[os_asic1]=0x54;
						*(uint32_t*)(asic1_patch_addr[os_asic1]+0x1B)=0;
			#endif
			#if defined(OS_330_538)
					case CXC330_538:		os_nboot++;
			#endif
			#if defined(OS_330_218)
					case CXC330_218:		os_nboot++;
			#endif
			#if defined(OS_323) || defined(OS_324)
				#ifdef CAS
					#ifdef OS_323
						case CXC323_1233:
					#endif
					#ifdef OS_324
						case CXC324_1237:
					#endif
				#else
						case CX32X_123X:
				#endif
							os_nboot++;
			#endif
			#if defined(OS_320_1212) || defined(OS_320_1219)
				#ifdef CM
				#ifdef CAS
					#ifdef OS_320_1212
						case CMC320_1212:
					#endif
					#ifdef OS_320_1219
						case CMC320_1219:
					#endif
				#else
					#ifdef OS_320_1212
						case CM320_1212:
					#endif
					#ifdef OS_320_1219
						case CM320_1219:
					#endif
				#endif
				#else
				#ifdef CAS
					#ifdef OS_320_1212
						case CXC320_1212:
					#endif
					#ifdef OS_320_1219
						case CXC320_1219:
					#endif
				#else
					#ifdef OS_320_1212
						case CX320_1212:
					#endif
					#ifdef OS_320_1219
						case CX320_1219:
					#endif
				#endif
				#endif
							os_nboot++;
			#endif
			#ifdef OS_320_1180
				#ifdef CAS
					case CXC320_1180:
				#else
					case CX320_1180:
				#endif
							os_nboot++;
			#endif
			#ifdef OS_320_1030
				#ifdef CAS
					case CXC320_1030:
				#else
					case CX320_1030:
				#endif
							os_nboot++;
			#endif
			#ifdef OS_320_776
				#ifdef CAS
					case CXC320_776:
				#else
					case CX320_776:
				#endif
							os_nboot++;
			#endif
			#ifdef OS_310_392
				#ifdef CM
					#ifdef CAS
					case CMC310_392:
					#else
					case CM310_392:
					#endif
				#else
					#ifdef CAS
					case CXC310_392:
					#else
					case CX310_392:
					#endif
				#endif
							os_nboot++;
			#endif
			#ifdef OS_310_236
					case CM310_236:
							os_nboot++;
			#endif
			#ifdef OS_310_157
					case CMC310_157:
							os_nboot++;
			#endif
			#ifdef OS_310_0
					case CM310_0:
							os_nboot++;
			#endif
			#if defined(OS_302_1791) || defined(OS_302_1793)
				#ifdef CAS
					case CXC302_179X:
						#if defined(OS_302_1791) && defined(OS_302_1793)
								if(*((volatile uint32_t *)(0x10000020)) != CXC302_179X || *((volatile uint32_t *)(0x10831D18)) == 0x6F68702F) // not 3.0.2 or 1793
													os_nboot++;
						#endif
				#else
					case CX302_179X:
						#if defined(OS_302_1791) && defined(OS_302_1793)
							if(*((volatile uint32_t *)(0x10000020)) != CX302_179X || *((volatile uint32_t *)(0x107D60E0)) == 0x6F68702F) // not 3.0.2 or 1793
											os_nboot++;
						#endif
				#endif
							os_nboot++;
			#endif
			#ifdef OS_301
				#ifdef CAS
					case CXC301_1753:
				#else
					case CX301_1753:
				#endif
							os_nboot++;
			#endif
			#ifdef OS_300_1503
					case CX300_1503:
							os_nboot++;
			#endif
			#ifdef OS_300_1045
					case CX300_1045:
							os_nboot++;
			#endif
			#ifdef OS_300_0
					case CX300_0:
							os_nboot++;
			#endif
			#ifdef PATCH_LT_400
					if(os_nboot>=0)
						*(uint32_t*)nboot_patch_addr[os_nboot]=0;
			#endif
		}
	#endif
	#ifdef PATCH_NDLESS
		static uint32_t ndless_loader[] = { NDLESS_LOADER };
	#if defined(PATCH_GT_402_NON_NDLESS)
		os_b2upd=-1;
	#endif
		switch (id) {
			#if defined(NDLESS_44)
				#ifdef CAS
					case CXC440_532:
				#else
					case CX440_532:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_43)
				#ifdef CAS
					case CXC430_702:
				#else
					case CX430_702:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_42)
				#ifdef CAS
					case CXC420_532:
				#else
					case CX420_532:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_403)
				#ifdef CAS
					case CXC403_29:
				#else
					case CX403_29:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_400)
				#ifdef CAS
					case CXC400_235:
				#else
					case CX400_235:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_391)
				#ifdef CAS
					case CXC391_38:
				#else
					case CX391_38:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_390)
				#ifdef CAS
					case CXC390_46X:
				#else
					case CX390_46X:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_36)
				#ifdef CAS
					case CXC360_546_550:
				#else
					case CX360_5XX:
				#endif
						os_b2upd++;
			#endif
			#if defined(NDLESS_31)
				#ifdef CM
					#ifdef CAS
					case CMC310_392:
					#else
					case CM310_392:
					#endif
				#else
					#ifdef CAS
					case CXC310_392:
					#else
					case CX310_392:
					#endif
				#endif
						os_b2upd++;
			#endif
			__builtin_memcpy(ndless_loader+OFFSET_MODEL, ndless_loader_model_specific[os_b2upd], sizeof(ndless_loader_model_specific[0]));
			uint32_t * dest = (uint32_t *)ndless_load_addr[os_b2upd];
			for (uint32_t i = 0; i < sizeof(ndless_loader)/sizeof(ndless_loader[0]); i++)
				*dest++ = ndless_loader[i];
		}
	#endif
	__asm volatile(
        "LDR    PC, =0x10000000 \n"
    );
    __builtin_unreachable();
}
