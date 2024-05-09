﻿**Credit to critor on [TI-Planet](https://tiplanet.org/forum/viewtopic.php?f=57&t=18920#installcx) for the tutorial**

**ControlX Complete Installation (CAS or if you only want ControlX too):**

**Note: where it says download I have linked downloads however everything is packaged in the folder named:** [TI-Nspire CX 4.4__4.5.3 CAS Pack](https://github.com/LevitateAlt/CAS-install-pack-for-CX/tree/main/TI-Nspire%20CX%204.4__4.5.3%20CAS%20Pack) **however I would recommend downloading the repository until I create a release.**

**Also, this tutorial is the best method for upgrade compatibility as the other option if you update after installing with the other tutorial can cause issues**

**A) Preparations :**

If not already done, download and install Ndless, following the [online tutorial](https://tiplanet.org/forum/ndl3ss.php).

**B)Repartitioning :**

**B1)** Download [nsPartManagic 4.2](https://tiplanet.org/forum/archives_voir.php?id=43277), extract the zip file content, and send the *nsPartManagic.tns* file on the calculator in any folder. 

**B2)** On the calculator open the *nsPartManagic* file. 

**B3)** Type ![ref1] to relocate the Diags partition. 

**B4)** Press ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.002.png) to make the BootData partition size go down to 0080 pages *(numbershownin parenthesis attheend of theBootDline)*. 

**B5)** Press ![ref2] to go back to the main menu. 

**Warning :** next step is going to completely wipe out your TI-Nspire data. Be sure to check that you have a backup of all important files. 

**B6)** Press ![ref3] to relocate the FileSystem partition.

**B7)** Press ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.005.png) until the ExtendeD partition size goes up to the required size (number shown in parenthesis at the end of the ExtnD line):

0280 pages for the basic installation (that's 1280KB - the Boot2 4.4.0 / 4.0.3 size)

**B8)** Type ![ref2] to go back to the main menu. 

**B9)** Type ![ref2] to apply your changes and confirm by typing ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.006.png). 

**B10)**Once the program has ended, press any key to exit. 

**B10)**Press the *reset* button on the back - the calculator is going to prompt you for a new Operating System.

**C) Repreparations :**

Reinstall an Ndless-able OS and Ndless **4.0 or newer**, following once again the [online tutorial](https://tiplanet.org/forum/ndl3ss.php).

4) **Install nBoot :**

**D1)** Download [nBoot](https://tiplanet.org/forum/archives_voir.php?id=652780), extract the zip file content and put the *manuf.img.tns* file on your calculator in any folder.

**D2)** Download [BtMg](https://tiplanet.org/forum/archives_voir.php?id=10080), extract the zip file content, and put the *BtMg.tns* file on your calculator in the */ndless/* folder.  **D3)** On the calculator open the *BtMg* file to complete the installation.

**D4)** When you reach the *BtMg* menu, press ![ref2] to exit. 

**D5)** On the calculator open the transferred *manuf.img* file - this should automatically open it in *BtMg*. 

**D6)** Once you reach the *BtMg* menu, type ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.007.png) to select the Manuf partition.  **D7)** Type ![ref4] to flash the new Manuf image. 

**D8)** Type ![ref5] to go back to the main menu.

**D9)** Type ![ref5] to quit *BtMg*.

5) **Preparations for ControlX :**

*CurrentControlXversion willonlyfullywork with Boot2 4.4.0.8 or 4.0.3.49.In caseof 4.5+OSupdatingyour Boot2,let's keep a usablecopyin a safeplace.* 

**E1)** Download [Boot2 4.4.0.8](https://tiplanet.org/forum/archives_voir.php?id=481757) and send it on your calculator in any folder. 

**E2)** On the calculator, open the transferred *tinspirecx\_boot2\_4.4.0.8.img* file - this should automatically launch *BtMg*. 

**E3)** Type ![ref3] to select the ExtendeD partition.  **E4)** Type ![ref4] to flash the Boot2 image. 

**E5)** Type ![ref5] to go back to the main menu.

**E6)** Type ![ref5] to quit *BtMg*.

6) **Install ControlX :**

**F1)** Download [ControlX](https://tiplanet.org/forum/archives_voir.php?id=652778), extract the zip file content and put the *controlx.img.tns* file on your calculator in any folder.  **F2)** On the calculator, open the *controlx.img* file - this should automatically open it in *BtMg*. 

**F3)** Type ![ref1] to select the Diags partition.  **F4)** Type ![ref4] to flash the *ControlX* image. 

**F5)** Type ![ref6] to confirm the default insertion as the 1st image in the Diags partition  *(in allcases itmustbethe1stimagetobebootable,and you shouldn'toverwritetheDiags image)* 

**F6)** Type ![ref5] to go back to the main menu. 

**F7)** Type ![ref7] to select the BootD partition. 

**F8)** Check the *boot=*line - which should indicate either *Boot2* either *Diags*. 

If it indicates *Boot2*, type ![ref8] to switch it to *Diags*.  **F9)** Type ![ref5] to go back to the main menu.

**F10)**Type ![ref5] to quit *BtMg*.

It's done - you may reset and in theory nBoot/ControlX/Ndless are now here forever - enjoy !

**CONTROLX UPDATE :**

**U1)** If you did remove it, redownload **[BtMg**](https://tiplanet.org/forum/archives_voir.php?id=10080)**, extract the zip file content, and put the *BtMg.tns* file on your calculator in the */ndless/* folder.

**U2)** On the calculator, open the *BtMg* file, and when you reach the *BtMg* menu press ![ref2] to exit. **U3)** Redownload [ControlX](https://tiplanet.org/forum/archives_voir.php?id=652778) and send the newer *controlx.img.tns* file to the calculator.

**U4)** Open the *controlx.img* file on the calculator - this should automatically open it in *BtMg*.

**U5)** Type ![ref1] to go the the Diags partition

**U6)** Type ![ref4] to flash the new ControlX image

**U7)** Type ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.013.png) to select the previous ControlX image location and thus overwrite it.

**U8)** Confirm with ![ref6].

**U9)** Type ![ref5] to go back to the main menun and ![ref5] to quit *BtMg*.

It's done - you may reset and in theory nBoot/ControlX/Ndless are now here forever - enjoy !

**OS INSTALLATION :**

Especially to install an OS not officially targeting your model : 

**O1)** Call the ControlX menu by holding ![ref9] while pressing reset. **O2)** Type ![ref7] to reset the minOS version to 0.0.0.

**O3)** *Thebottom status lineshould displayyour currentlysetmodel:TI-NspireCX,TI-NspireCXCAS,TI-NspireCM,TI-Nspire CMCAS* 

If it differs from the model targeted by the OS you'd like to install, make it match by : 

Typing ![ref8] if you need to switch the CAS/numeric model type. Typing ![](Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.015.png) if you need to switch the CX/CM model family.

**O4)** If it worked, store the options you've used as default boot options, by holding ![ref9] while pressing their number. 

*(if you mess up,you can resetbootoptions byholding ![ref9] whiletyping ![ref5])*

**O5)** Type ![ref5] to restart.  *Your calculator mayremoveits currentOS.*

**O6)** Check that your calculator is booting on the CX Boot2 4.40.8 or 4.3.49 *(shownonthetopstatusline)*, and that the bottom status line is showing the name of the model targeted by the OS you want to intall. 

*Versions 3.6+nottargetingyour modelaren'tgoingtowork with a di erentBoot2 version.*

**O7)** Now send the wanted OS - *theOSshouldbeaccepted*

**O8)** If you sent a 4.0.2+ version not targeting your model and are using **Boot2 4.3.49**, there is an additional mandatory step, or the OS is going to uninstall itself. 

*Oncethetransfer is completethecalculator should restarttoinstalltheOSand launch itfor the1sttime.* 

So, check the progress bar. After it reaches 100% and you get the OS grey loading screen, immediately press reset. 

*It's onlymandatoryfor the1stinstall-boot,this won'tbeneeded again.*

**O9)** If you sent a 3.9+ version non targeting your model, add the correct calculation patcher to the /ndless/startup/ folder. 

[i]Only available for OSes 4.4, 4.5.0 and 4.5.3 so far : 

CAS OS 4.5.3 on a non-CAS model, grab the **cas\_patcher\_4.5.3.14.tns** over there : [https://www.cncalc.org/forum.php?mod=at ... wwfDI1MzA5](https://www.cncalc.org/forum.php?mod=attachment&aid=MjQwNzh8NjAxNGJkYjF8MTY4MTAyODE1NnwwfDI1MzA5)

CAS OS 4.5.0 on a non-CAS model, grab the **cas\_patch\_4.5.0.1180.tns** over there : [https://github.com/parrotgeek1/TI-Nspir ... atchfinder](https://github.com/parrotgeek1/TI-Nspire/tree/master/patchfinder) (broken link)

CAS OS 4.4 on a non-CAS model, grab the **cas\_patcher\_4.4.0.532.tns** over there : [https://www.cncalc.org/forum.php?mod=at ... wwfDI1MzA5](https://www.cncalc.org/forum.php?mod=attachment&aid=MjQwNzl8ODU0MmQwZTZ8MTY4MTAyODE1NnwwfDI1MzA5)

[ref1]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.001.png
[ref2]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.003.png
[ref3]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.004.png
[ref4]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.008.png
[ref5]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.009.png
[ref6]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.010.png
[ref7]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.011.png
[ref8]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.012.png
[ref9]: Aspose.Words.7caf6c38-f460-427d-ac2c-4af051d4d099.014.png
