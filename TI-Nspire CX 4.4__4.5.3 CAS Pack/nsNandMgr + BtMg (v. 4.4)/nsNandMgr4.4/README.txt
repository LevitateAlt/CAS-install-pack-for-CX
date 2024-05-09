+------------------------+
| nsNandMgr + BtMg 4.3.1 |
+------------------------+
for Ndless 3.1+
by Critor

includes code developped by:
- Bsl
- Goplat
- ExtendeD

WARNING: nsNandMgr/BtMg may permanently brick your TI-Nspire if not used correctly.
You should use it only:
- if you know what you're doing
- if a tutorial told you exactly what to do

In all cases, changes aren't effective untill you reboot your calculator.
So you can still check your changes by running the tool again, and fix them if necessary.


A) Common nsNandMgr/BtMg features :
---------------------------------

1) "Manuf" menu lets you :
- switch the Nspire type : CAS/non-CAS (*)
- change the default language of a TI-Nspire CX/CM
- turn an Nspire CM into an Nspire CX and back (**)
- remove the boot graphics
- clean the Manuf partition to uninstall nBoot
(*) only enabled on prototypes where this modification
doesn't create a mismatch with the ASIC chip flags
(**) in CX mode, the CM will need Nlaunch in order to work properly

4) "Manuf area" menu lets you :
- dump your manuf partition (default "/ndless/manuf.img")
- flash the image (default "/ndless/manuf.img") to your manuf partition

5) "Boot Data" menu lets you :
- dump your BootD partition (default "/ndless/bootd.img")
- remove the minimal installable OS downgrade protection
- switch the Boot2/Diags boot order

B) Features differing in nsNandMgr/BtMg tools :
---------------------------------------------
2/3/6) "Boot2", "Diags" and "ExtnD" menus let you :
- dump from your Boot2/Diags/ExtnD partition
- flash to your Boot2/Diags/ExtnD partition
- trim the dumped image
- run a dumped/flashed image

nsNandMgr is kept for compatibility with older tutorials
and works with the following files :
- /ndless/manuf.img
- /ndless/boot2.img
- /ndless/bootd.img
- /ndless/diags.img
- /ndless/extnd.img

The newer BtMg tool :
- works by default with the /ndless/dump.img file
- once launched a 1st time, will be automatically
launched for every .img file you try to open
- will automatically detect if the .img file is a Manuf, BootD or Boot2/Diags image
- does support the multi-images partition format introduced by TI with Boot2 4.0.2
-> you can select the wanted image in the source partition or partition image
-> you can then select the partition or partition image dest location
(you may insert the image at the start, at the end, between two images, or even overwrite an existing image)


Support : http://tiplanet.org