#include "types.h"
#include "patchboot2.h"

#define IGNORE 0
#define NOP 0x00000000
#define PATCH_SETW(A,B) *(uint32_t *)(A) = (B)
#define PATCH_SETB(A,B) *(uint8_t *)(A) = (B)

#define IB2_ID		0
#define IB2_NBOOT	1
#define IB2_ASIC	2
#define IB2_MANUF	3
#define	IB2_RSAOS	4
#define	IB2_RSAA5	5
#define	IB2_RSABA	6
#define	IB2_RSAD3	7

#define NBOOT2 sizeof(boot2_patches)/sizeof(boot2_patches[0])
static const uint32_t boot2_patches[][IB2_RSAD3+1] = {
//	CX/CM			offsets
//	Boot2 ID		nBoot				ASIC				Manuf modelID		RSA TI-Nspire.cer	RSA A5 key			RSA BA key			RSA D3 key
	{CXB300_0,		NBOOT_CXB300_0,		ASIC_CXB300_0,		MANUF_CXB300_0,		OSRSABA_CXB300_0,	OSRSAA5_CXB300_0,	OSRSABA_CXB300_0,	IGNORE				},
	{CXB301_131,	NBOOT_CXB301_131,	ASIC_CXB301_131,	MANUF_CXB301_131,	OSRSAA5_CXB301_131,	OSRSAA5_CXB301_131,	OSRSABA_CXB301_131,	IGNORE				},
	{CXB302_141,	NBOOT_CXB302_141,	ASIC_CXB302_141,	MANUF_CXB302_141,	OSRSAA5_CXB302_141,	OSRSAA5_CXB302_141,	OSRSABA_CXB302_141,	IGNORE				},
	{CXB310_16,		NBOOT_CXB310_16,	ASIC_CXB310_16,		MANUF_CXB310_16,	OSRSAA5_CXB310_16,	OSRSAA5_CXB310_16,	OSRSABA_CXB310_16,	IGNORE				},
	{CMB310_0,		NBOOT_CMB310_0,		IGNORE,				MANUF_CMB310_0,		OSRSAD3_CMB310_0,	IGNORE,				OSRSABA_CMB310_0,	OSRSAD3_CMB310_0	},
	{CMB310_16,		NBOOT_CMB310_16,	IGNORE,				MANUF_CMB310_16,	OSRSAD3_CMB310_16,	IGNORE,				OSRSABA_CMB310_16,	OSRSAD3_CMB310_16	},
	{CXB324_7,		NBOOT_CXB324_7,		ASIC_CXB324_7,		MANUF_CXB324_7,		OSRSAA5_CXB324_7,	OSRSAA5_CXB324_7,	OSRSABA_CXB324_7,	IGNORE				},
	{CXB391_34,		NBOOT_CXB391_34,	ASIC_CXB391_34,		MANUF_CXB391_34,	OSRSAA5_CXB391_34,	OSRSAA5_CXB391_34,	OSRSABA_CXB391_34,	IGNORE				},
	{CXB402_47,		IGNORE,				ASIC_CXB402_47,		MANUF_CXB402_47,	OSRSAA5_CXB402_47,	OSRSAA5_CXB402_47,	OSRSABA_CXB402_47,	IGNORE				},
	{CXB403_49,		IGNORE,				ASIC_CXB403_49,		MANUF_CXB403_49,	OSRSAA5_CXB403_49,	OSRSAA5_CXB403_49,	OSRSABA_CXB403_49,	IGNORE				},
	{CXB430_X,		IGNORE,				ASIC_CXB430_5,		MANUF_CXB430_5,		OSRSAA5_CXB430_5,	OSRSAA5_CXB430_5,	OSRSABA_CXB430_5,	IGNORE				},
	{CXB430_X,		IGNORE,				ASIC_CXB430_3,		MANUF_CXB430_3,		OSRSAA5_CXB430_3,	OSRSAA5_CXB430_3,	OSRSABA_CXB430_3,	IGNORE				},
	{CXB440_8,		IGNORE,				ASIC_CXB440_8,		MANUF_CXB440_8,		OSRSAA5_CXB440_8,	OSRSAA5_CXB440_8,	OSRSABA_CXB440_8,	IGNORE				},
};

uint8_t getBoot2Index(uint32_t id) {
	uint8_t i=0;
	for(i=0;i<NBOOT2;i++) {
		if(boot2_patches[i][IB2_ID]==id)
			break;
	}
	return i;
}

//! Patch the Boot2.

extern uint8_t modelid;
extern uint8_t bootkeys;
extern uint8_t asicflags;

static uint32_t rsa_cx_prod[] = { RSAOS_CXPROD };
static uint32_t rsa_cm_prod[] = { RSAOS_CMPROD };
#define RSA_SIZE sizeof(rsa_cx_prod)

void patch_Boot2() {
	uint32_t id = *((uint32_t*)(0x11800020));
	uint8_t index = getBoot2Index(id);
	if(index<NBOOT2){
		if(boot2_patches[index][IB2_NBOOT])	PATCH_SETW(boot2_patches[index][IB2_NBOOT],0);
		if(boot2_patches[index][IB2_RSAOS]) {
			uint8_t* rsa=0;
			if(bootkeys) {
				if(boot2_patches[index][IB2_RSAOS]!=boot2_patches[index][IB2_RSABA])
					rsa = boot2_patches[index][IB2_RSABA];
			}
			else if(modelid<=0x10) {
				if(boot2_patches[index][IB2_RSAOS]!=boot2_patches[index][IB2_RSAA5])
					rsa = rsa_cx_prod;
			}
			else if(boot2_patches[index][IB2_RSAOS]!=boot2_patches[index][IB2_RSAD3])
				rsa = rsa_cm_prod;
			if(rsa)
				memcpy(boot2_patches[index][IB2_RSAOS],rsa,RSA_SIZE);
		}
	}
	if(	(modelid==0x0F && asicflags!=1) ||
		(modelid==0x10 && asicflags) ||
		(modelid==0x11 && asicflags!=3) ||
		(modelid==0x12 && asicflags!=2)) {
		uint32_t fakeasic1=0; // CX
		uint32_t fakeasic2=0xE3A04000; // CX
		switch(modelid) {
			case 0x0F: // CX CAS
				fakeasic1=0x00010105;
				fakeasic2=0xE3A04341;
				break;
			case 0x11: // CM CAS
				fakeasic1=0x00000002;
				fakeasic2=0xE3A04323;
				break;
			case 0x12: // CM
				fakeasic1=0x00010107;
				fakeasic2=0xE3A04362;
		}
		if(boot2_patches[index][IB2_MANUF])
			PATCH_SETW(boot2_patches[index][IB2_MANUF],0xE3A00000|modelid);
		if(boot2_patches[index][IB2_ASIC]) {
			PATCH_SETB(boot2_patches[index][IB2_ASIC],0x54);
			PATCH_SETW(boot2_patches[index][IB2_ASIC]+0x17,fakeasic2);
			PATCH_SETW(boot2_patches[index][IB2_ASIC]+0x1B,NOP);
			PATCH_SETW(boot2_patches[index][IB2_ASIC]+0x48B,fakeasic1);
		}
	}
}