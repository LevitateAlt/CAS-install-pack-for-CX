+-------------------+
| nsPartManagic 4.2 |
+-------------------+
for TI-Nspire CX/CM running Ndless 3.1 or compatible
by Critor


GENERAL WARNING :
---------------
nsPartManagic may permanently brick your TI-Nspire CX/CM if not used correctly.

You should use it only:
- if you technically know what you're doing and are aware of what your changes may imply
- if a tutorial told you exactly what to do

You should disable any overclocking or TSR program while using it.

In all cases, changes aren't effective untill you reboot your calculator.
So you can still check your changes by running nsPartition again, and fix them if necessary.



What is it ?
----------
nsPartition is a tool which lets you view and modify the TI-Nspire CX/CM NAND memory partition table.
This partition table defines the maximum sizes of the following areas:

Area		Default size
-----------------------------------------------
Manuf		   128 KB	(64 pages)
Boot2		  2688 KB	(1344 pages)
Boot Data	   384 KB	(192 pages)
Diags		   896 KB	(448 pages)
File System	126996 KB	(63488 pages)

Note that any change appied to the file system are will make you loose all its content.


SPECIFIC WARNING :
----------------
nsPartition will try its best to prevent you from applying bad settings, by forbidding too small sizes.

Note that although you may have no problem just after altering the partition table, there is no garanty that future OS/Boot2 updates are still going to handle the modified partition table correcly, and those might permanently brick your calculator.

For example, if you limit the Boot2 area to 448 pages, which is enough for all currently released TI-Nspire CX/CM Boot2 versions (last and biggest Boot2 3.2.4 uses 435 pages), and if TI releases a new OS flashing a Boot2 over 448 pages...
If you decide to use a modified partition table don't install a newly released updated before confirming on a TI forum that there is no problem with altered partition tables.

After altering your partition table, don't use 3rd party NAND flashing tools which were released long before nsPartition.
nsNandMgr/BtMg v2.0+ are the only 3rd party NAND flashing tools handling correctly modified partition tables.
