**Credit to critor on** [**TI-Planet**](https://tiplanet.org/forum/viewtopic.php?f=57\&t=18920#installcx) **for the tutorial**\
**ControlX Complete Installation (CAS or if you only want ControlX too):**

**Note: where it says download I have linked downloads however everything is packaged in the folder named:** [TI-Nspire CX 4.4\_\_4.5.3 CAS Pack](https://github.com/LevitateAlt/CAS-install-pack-for-CX/tree/main/TI-Nspire%20CX%204.4__4.5.3%20CAS%20Pack) **however I would recommend downloading the current release.**

**Also, this tutorial is the best method for upgrade compatibility as the other option if you update after installing with the other tutorial can cause issues**

 \
**A) Preparations :** \
If not already done, download and install Ndless, following the [online tutorial](https://tiplanet.org/forum/ndl3ss.php).

**B)Repartitioning :**

 \
**B1)** Download [nsPartManagic 4.2](https://tiplanet.org/forum/archives_voir.php?id=43277), extract the zip file content, and send the _nsPartManagic.tns_ file on the calculator in any folder. \
**B2)** On the calculator open the _nsPartManagic_ file. \
**B3)** Type ![](https://lh7-us.googleusercontent.com/g-Arr0WUTDbo-HoZ_Nmfz9eIVOVbGJMdQ4uzpDZpiQ1JWbkWoj_w9Rc_UDyQRM0vo0uDfpoXFuduR9JpLyirP1hDgb5TqYmMihNIX9U2g9WxaRHo5H3G5D_QyiHPKLmCYt5crzDr8_wE4ckhJUcJXxg) to relocate the Diags partition. \
**B4)** Press ![](https://lh7-us.googleusercontent.com/-mEn0tUTomuuTpIov4CyHWwbOPKArcB3duEllJmYMByUcpQjZ0boIQuBa45A4UCi9yJt2tujN2Iwv0zHPgDZZMk0MBvTcqDFBAhCHHychnp7emgI6CgPsrEEpBelM-IjPnIoU9t5l9dZDXgxtT5h0NY) to make the BootData partition size go down to 0080 pages _(number shown in parenthesis at the end of the BootD line)_. \
**B5)** Press ![](https://lh7-us.googleusercontent.com/zjmCQbA_knaawkm6pgLT6UgBf8SfcVu7aONlRodR0QrSFFUvpO__Xfk2hFzcTJNfFjMwScbsSDaWU6s2AClRJ1TBvmhZxqPc95Oq1eNDJWrDe4s_4BhW6-Z3gH1T8aM_2bZ8l7ipKejmGYEyMooj3dk) to go back to the main menu. \
**Warning :** next step is going to completely wipe out your TI-Nspire data. Be sure to check that you have a backup of all important files. \
**B6)** Press ![](https://lh7-us.googleusercontent.com/ydH6sDqFe6I7-Fgwm3zAvN3bsz0qsU9Ak4hT9EtgfmOVTHs7kNdqMm7j8ia90vgpiiX3NE6fe0IheRSMP9uJiPSDw8LirYQtDEm_Wv7e9PY4oBPB5XpBXLQaOA7-oaEp_QIhd31rsPEXoLWObaHdLS8) to relocate the FileSystem partition.

**B7)** Press ![](https://lh7-us.googleusercontent.com/es1vnRAXEAMh6I5XAy30r-1jdMq7oK8W4gTIstGVS8BgJU39Gj-iH6Mdsxm7xuIV_owyVeJdGP6xlJ8fbNab78ClKhhxdqIRS0hgl9jeKG_IZgR8ImsVFafj4RYAX33P1qnD9atgc9w5UEGJhg6Ftzo) until the ExtendeD partition size goes up to the required size (number shown in parenthesis at the end of the ExtnD line):

0280 pages for the basic installation (that's 1280KB - the Boot2 4.4.0 / 4.0.3 size)

**B8)** Type ![](https://lh7-us.googleusercontent.com/zjmCQbA_knaawkm6pgLT6UgBf8SfcVu7aONlRodR0QrSFFUvpO__Xfk2hFzcTJNfFjMwScbsSDaWU6s2AClRJ1TBvmhZxqPc95Oq1eNDJWrDe4s_4BhW6-Z3gH1T8aM_2bZ8l7ipKejmGYEyMooj3dk) to go back to the main menu. \
**B9)** Type ![](https://lh7-us.googleusercontent.com/zjmCQbA_knaawkm6pgLT6UgBf8SfcVu7aONlRodR0QrSFFUvpO__Xfk2hFzcTJNfFjMwScbsSDaWU6s2AClRJ1TBvmhZxqPc95Oq1eNDJWrDe4s_4BhW6-Z3gH1T8aM_2bZ8l7ipKejmGYEyMooj3dk) to apply your changes and confirm by typing ![](https://lh7-us.googleusercontent.com/KiAqOn_uNqowOrpu_c3kaZO6HAFL4b-QD5ne5cpkFQYDErup89ThguY6mKNR4F0TZJW0xTJXGFMxlu-o0-QkwhmsUU7tK-fTPFL7JlZJbcwui4LJuT4IYTjLEzQZpgWnpD56lI0SW6KAzCq9OlvsIII). \
**B10)**Once the program has ended, press any key to exit. \
**B10)**Press the _reset_ button on the back - the calculator is going to prompt you for a new Operating System.

 \
 \
 \
**C) Repreparations :** \
Reinstall an Ndless-able OS and Ndless **4.0 or newer**, following once again the [online tutorial](https://tiplanet.org/forum/ndl3ss.php).

 \
**D) Install nBoot :** \
**D1)** Download [nBoot](https://tiplanet.org/forum/archives_voir.php?id=652780), extract the zip file content and put the _manuf.img.tns_ file on your calculator in any folder.

**D2)** Download [BtMg](https://tiplanet.org/forum/archives_voir.php?id=10080), extract the zip file content, and put the _BtMg.tns_ file on your calculator in the _/ndless/_ folder. \
**D3)** On the calculator open the _BtMg_ file to complete the installation.

**D4)** When you reach the _BtMg_ menu, press ![](https://lh7-us.googleusercontent.com/zjmCQbA_knaawkm6pgLT6UgBf8SfcVu7aONlRodR0QrSFFUvpO__Xfk2hFzcTJNfFjMwScbsSDaWU6s2AClRJ1TBvmhZxqPc95Oq1eNDJWrDe4s_4BhW6-Z3gH1T8aM_2bZ8l7ipKejmGYEyMooj3dk) to exit. \
**D5)** On the calculator open the transferred _manuf.img_ file - this should automatically open it in _BtMg_. \
**D6)** Once you reach the _BtMg_ menu, type ![](https://lh7-us.googleusercontent.com/8WAIHSAz3qejxodL_yS94HM4DZuhMFZ7qdFReDvXhuIX0Kqw0ufkq0dyImzlb6GGKq9onGOzI7OpdKqNOwwaVNit_DPHPMZ7C346RWue-0w4PI-imOloqzveq3wS7Nngo7ElzmJDZORItg4sdGBzvJ8) to select the Manuf partition. \
**D7)** Type ![](https://lh7-us.googleusercontent.com/IvzXh8xuNPO2eW9Tik0Be-oOd8_Y7KzR_iVHaUV_i6fz6VO5QEWYB1A87MDXsp3XGMjIo4BeU40zdDb0fCW6wh2Ly_4Jw9GHzTcDlqMcXexLKZJR5pQXnW1bFld8IpQnFV5M9INppKoOpln7hcl1VJE) to flash the new Manuf image. \
**D8)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to go back to the main menu.

**D9)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to quit _BtMg_.

 \
 \
**E) Preparations for ControlX :**

 \
_Current ControlX version will only fully work with Boot2 4.4.0.8 or 4.0.3.49. In case of 4.5+ OS updating your Boot2, let's keep a usable copy in a safe place._ \
**E1)** Download [Boot2 4.4.0.8](https://tiplanet.org/forum/archives_voir.php?id=481757) and send it on your calculator in any folder. \
**E2)** On the calculator, open the transferred _tinspirecx\_boot2\_4.4.0.8.img_ file - this should automatically launch _BtMg_. \
**E3)** Type ![](https://lh7-us.googleusercontent.com/ydH6sDqFe6I7-Fgwm3zAvN3bsz0qsU9Ak4hT9EtgfmOVTHs7kNdqMm7j8ia90vgpiiX3NE6fe0IheRSMP9uJiPSDw8LirYQtDEm_Wv7e9PY4oBPB5XpBXLQaOA7-oaEp_QIhd31rsPEXoLWObaHdLS8) to select the ExtendeD partition. \
**E4)** Type ![](https://lh7-us.googleusercontent.com/IvzXh8xuNPO2eW9Tik0Be-oOd8_Y7KzR_iVHaUV_i6fz6VO5QEWYB1A87MDXsp3XGMjIo4BeU40zdDb0fCW6wh2Ly_4Jw9GHzTcDlqMcXexLKZJR5pQXnW1bFld8IpQnFV5M9INppKoOpln7hcl1VJE) to flash the Boot2 image. \
**E5)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to go back to the main menu.

**E6)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to quit _BtMg_.

 

**F) Install ControlX :** \
**F1)** Download [ControlX](https://tiplanet.org/forum/archives_voir.php?id=652778), extract the zip file content and put the _controlx.img.tns_ file on your calculator in any folder. \
**F2)** On the calculator, open the _controlx.img_ file - this should automatically open it in _BtMg_. \
**F3)** Type ![](https://lh7-us.googleusercontent.com/P96nZYIoGL5fh9qZSzl1q4DViMRCiWoN2eAcvMMgl3TKwG-is7ToN2tYbXQUqIKBFQbRvvfcvxJh2wn-MxJDI42zmJ6K1LyPLGAlorxoqc4BvvghmqjmKxdL79qbUHDE5ANArL6d7o5agK5d9PWQDXQ) to select the Diags partition. \
**F4)** Type ![](https://lh7-us.googleusercontent.com/IvzXh8xuNPO2eW9Tik0Be-oOd8_Y7KzR_iVHaUV_i6fz6VO5QEWYB1A87MDXsp3XGMjIo4BeU40zdDb0fCW6wh2Ly_4Jw9GHzTcDlqMcXexLKZJR5pQXnW1bFld8IpQnFV5M9INppKoOpln7hcl1VJE) to flash the _ControlX_ image. \
**F5)** Type ![](https://lh7-us.googleusercontent.com/u8rOqemltaGDeX2vEE9Sy58U2FdKilEywzrZLxcl9t_8PxRO3Za_OM0-TForNROxreCaZPtQiGfjHUZYqJfYZZue-VFaozEbGEsa9jzYWxLWJ-qL5QZUdXlt2cL_neEC5KG8mJKRa-Bn4P0QEArUGbI) to confirm the default insertion as the 1st image in the Diags partition \
_(in all cases it must be the 1st image to be bootable, and you shouldn't overwrite the Diags image)_ \
**F6)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to go back to the main menu. \
**F7)** Type ![](https://lh7-us.googleusercontent.com/8ilOsAzTdqa7ETiWVxKFO_uQGM8M_9mABHgtxQpJ94jyFJOHTrwWQIn6v6TSatiJ1s9Xg2KoOjFOKkwrccgwS58JICxl-mQ8cl0XDVqPHiwIKZ3-31fij7-DsGbG5aJ4JSxOFIbzFVXYkdsjx2Rnrb8) to select the BootD partition. \
**F8)** Check the _boot=_ line - which should indicate either _Boot2_ either _Diags_. \
If it indicates _Boot2_, type ![](https://lh7-us.googleusercontent.com/m60uFMUsiT1iKoEAZNNy7erH54InzZaEJ_zWmQ4YUpd5TQ-j3CM6AI8nB4yqsdLs_wDvS7V74tIPyzaDd9N1ETKnGBn9476ydkf9LFdi2sY3HSpw4ebh0dheaPnTZNr_uXnN0UBCuiEvBqv6Y2C1oro) to switch it to _Diags_. \
**F9)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to go back to the main menu.

**F10)**Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to quit _BtMg_.

It's done - you may reset and in theory nBoot/ControlX/Ndless are now here forever - enjoy ! \
 \
&#x20;  **CONTROLX UPDATE :**\
**U1)** If you did remove it, redownload [**BtMg**](https://tiplanet.org/forum/archives_voir.php?id=10080), extract the zip file content, and put the _BtMg.tns_ file on your calculator in the _/ndless/_ folder.

**U2)** On the calculator, open the _BtMg_ file, and when you reach the _BtMg_ menu press ![](https://lh7-us.googleusercontent.com/zjmCQbA_knaawkm6pgLT6UgBf8SfcVu7aONlRodR0QrSFFUvpO__Xfk2hFzcTJNfFjMwScbsSDaWU6s2AClRJ1TBvmhZxqPc95Oq1eNDJWrDe4s_4BhW6-Z3gH1T8aM_2bZ8l7ipKejmGYEyMooj3dk) to exit.

**U3)** Redownload [ControlX](https://tiplanet.org/forum/archives_voir.php?id=652778) and send the newer _controlx.img.tns_ file to the calculator.

**U4)** Open the _controlx.img_ file on the calculator - this should automatically open it in _BtMg_.

**U5)** Type ![](https://lh7-us.googleusercontent.com/P96nZYIoGL5fh9qZSzl1q4DViMRCiWoN2eAcvMMgl3TKwG-is7ToN2tYbXQUqIKBFQbRvvfcvxJh2wn-MxJDI42zmJ6K1LyPLGAlorxoqc4BvvghmqjmKxdL79qbUHDE5ANArL6d7o5agK5d9PWQDXQ) to go the the Diags partition

**U6)** Type ![](https://lh7-us.googleusercontent.com/IvzXh8xuNPO2eW9Tik0Be-oOd8_Y7KzR_iVHaUV_i6fz6VO5QEWYB1A87MDXsp3XGMjIo4BeU40zdDb0fCW6wh2Ly_4Jw9GHzTcDlqMcXexLKZJR5pQXnW1bFld8IpQnFV5M9INppKoOpln7hcl1VJE) to flash the new ControlX image

**U7)** Type ![](https://lh7-us.googleusercontent.com/9kyW8dUIKDFDA-oxLpeEr_MoWVxz6DE5viXD9Bh_YbHORYcD8BWPP7SrNBt90nc1xJiipgXoPIyQl4llnSYDTlmI3AiyGxtaei1wkWsdLF7QutpOYE8q7CIQsuuRy6w6dN0na5Se91lgSYLMjsnw1y4) to select the previous ControlX image location and thus overwrite it.

**U8)** Confirm with ![](https://lh7-us.googleusercontent.com/fUDb4ET5nv6OFJ-rqJ3QcNkNpt8YpOcYgbiqWSNNd-A3zKpvElY0s-ABlTGWopjnleY67I8STqteiNXrSmNAWBIPMYfph7By5q1RFT9g231pf2zMrGVwCQv_hzN7AF6vpJ9D398mPV4yeLESHsHrFz0).

**U9)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to go back to the main menun and ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to quit _BtMg_.

 \
It's done - you may reset and in theory nBoot/ControlX/Ndless are now here forever - enjoy !

 \
**OS INSTALLATION :**

 \
Especially to install an OS not officially targeting your model : \
**O1)** Call the ControlX menu by holding ![](https://lh7-us.googleusercontent.com/BoCRSVsMubbNr9cwQkbs5KFMFqeoHLeUSkKTQSSn-rE6gL_bTUpXb3i3ITqp2YUn_LK29Uu7pJBnkBpGhJcjO1rB8RnGL9uxgjZVlHFq1J4huryZsMGGgRkys0618ux9_B9h6FHgZcQ1B1h1UgYCtq0) while pressing reset.

**O2)** Type ![](https://lh7-us.googleusercontent.com/8ilOsAzTdqa7ETiWVxKFO_uQGM8M_9mABHgtxQpJ94jyFJOHTrwWQIn6v6TSatiJ1s9Xg2KoOjFOKkwrccgwS58JICxl-mQ8cl0XDVqPHiwIKZ3-31fij7-DsGbG5aJ4JSxOFIbzFVXYkdsjx2Rnrb8) to reset the minOS version to 0.0.0.

**O3)** _The bottom status line should display your currently set model : TI-Nspire CX, TI-Nspire CX CAS, TI-Nspire CM, TI-Nspire CM CAS_ \
If it differs from the model targeted by the OS you'd like to install, make it match by : \
Typing ![](https://lh7-us.googleusercontent.com/m60uFMUsiT1iKoEAZNNy7erH54InzZaEJ_zWmQ4YUpd5TQ-j3CM6AI8nB4yqsdLs_wDvS7V74tIPyzaDd9N1ETKnGBn9476ydkf9LFdi2sY3HSpw4ebh0dheaPnTZNr_uXnN0UBCuiEvBqv6Y2C1oro) if you need to switch the CAS/numeric model type.

Typing ![](https://lh7-us.googleusercontent.com/8ASa9WFX9nmoNGZyjlX-3MkssJ7zfFWNzl75cNESuIe2dyXRnJq4hUUbUKlShz2a5KuJbQQq4lDv4-v-dmDHhZ_eonmvQk5PaSR0M9MNE5VhWFzrndJ-Q6S5y6qFynWkPIJZlkXH2eGbDr1_ZJjTvH4) if you need to switch the CX/CM model family.

**O4)** If it worked, store the options you've used as default boot options, by holding ![](https://lh7-us.googleusercontent.com/BoCRSVsMubbNr9cwQkbs5KFMFqeoHLeUSkKTQSSn-rE6gL_bTUpXb3i3ITqp2YUn_LK29Uu7pJBnkBpGhJcjO1rB8RnGL9uxgjZVlHFq1J4huryZsMGGgRkys0618ux9_B9h6FHgZcQ1B1h1UgYCtq0) while pressing their number. \
_(if you mess up, you can reset boot options by holding_  ![](https://lh7-us.googleusercontent.com/BoCRSVsMubbNr9cwQkbs5KFMFqeoHLeUSkKTQSSn-rE6gL_bTUpXb3i3ITqp2YUn_LK29Uu7pJBnkBpGhJcjO1rB8RnGL9uxgjZVlHFq1J4huryZsMGGgRkys0618ux9_B9h6FHgZcQ1B1h1UgYCtq0) _while typing_ ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k)_)_

**O5)** Type ![](https://lh7-us.googleusercontent.com/-tPLiV2EIwmmoDoPg57vwo-f9ckUv4GKrhQpKhqWyICqbxtkpOszzliGS5hk5O82s-gwybh4H_vcIK2LrNBRXiBgBEj5Bp5Z3wPOP4AnaNB3hj1C_KIlQsvcRWhHWt5SvkU4xBkc1p7uxnkkrXXdU1k) to restart. \
_Your calculator may remove its current OS._

**O6)** Check that your calculator is booting on the CX Boot2 4.40.8 or 4.3.49 _(shown on the top status line)_, and that the bottom status line is showing the name of the model targeted by the OS you want to intall. \
_Versions 3.6+ not targeting your model aren't going to work with a different Boot2 version._

**O7)** Now send the wanted OS - _the OS should be accepted_

**O8)** If you sent a 4.0.2+ version not targeting your model and are using **Boot2 4.3.49**, there is an additional mandatory step, or the OS is going to uninstall itself. \
_Once the transfer is complete the calculator should restart to install the OS and launch it for the 1st time._ \
So, check the progress bar. After it reaches 100% and you get the OS grey loading screen, immediately press reset. \
_It's only mandatory for the 1st install-boot, this won't be needed again._

**O9)** If you sent a 3.9+ version non targeting your model, add the correct calculation patcher to the /ndless/startup/ folder. \
\[i]Only available for OSes 4.4, 4.5.0 and 4.5.3 so far : \
CAS OS 4.5.3 on a non-CAS model, grab the **cas\_patcher\_4.5.3.14.tns** over there : [https://www.cncalc.org/forum.php?mod=at ... wwfDI1MzA5](https://www.cncalc.org/forum.php?mod=attachment\&aid=MjQwNzh8NjAxNGJkYjF8MTY4MTAyODE1NnwwfDI1MzA5)

CAS OS 4.5.0 on a non-CAS model, grab the **cas\_patch\_4.5.0.1180.tns** over there : [https://github.com/parrotgeek1/TI-Nspir ... atchfinder](https://github.com/parrotgeek1/TI-Nspire/tree/master/patchfinder) (broken link)

CAS OS 4.4 on a non-CAS model, grab the **cas\_patcher\_4.4.0.532.tns** over there : [https://www.cncalc.org/forum.php?mod=at ... wwfDI1MzA5](https://www.cncalc.org/forum.php?mod=attachment\&aid=MjQwNzl8ODU0MmQwZTZ8MTY4MTAyODE1NnwwfDI1MzA5)
