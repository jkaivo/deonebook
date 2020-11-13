DeOneBook
---------

This program extracts encryption keys from eOneBook SD cards. Eventually I will
add decryption as well, but for now it is enough to extract a key that can be
used passed to `openssl` to do the actual decryption.

Building
--------

If you have a system with a directly attached SD card reader (i.e. a device
that appears as `/dev/mmcblk0` or similar, *not* as `/dev/sdb`), you can build
a native version of the program with:

    make

If you do not (this includes all USB readers and some internal devices that are
attached through an internal USB adapter), you'll need to cross-compile the
tool to run directly on the eOneBook. First install a cross compiler. On
Debian and Ubuntu systems, you can do this with:

    sudo apt install crossbuild-essential-armhf

Then build with the cross compiler:

    make CC=arm-linux-gnueabihf-gcc

Use
---

If you are using the native version, simply place an eOneBook SD card in your
SD card reader, and run:

    ./deonebook -G [devicename]

Here, `devicename` is optional, and will default to `mmcblk0`. If your SD card
has a different device name, specify it there. The encryption key will be
presented in hexadecimal.

For the eOneBook version, it's a little more complex. You'll need to prep the
SD card to execute the program and save its output. First, make sure the SD
card is not write-protected by moving the write-protect slider to the end of
its slot nearest the pins of the card. Mount the SD card somewhere you can
write to it, `cd` to that mount point, and run the following commands,
replacing `$DEONEBOOKDIR` with the path to the compiled DeOneBook.

***Be very careful with these commands, as if you lose or overwrite the
original `.A001` file, you will not be able to use that SD card in your
eOneBook anymore. You are strongly advised to back up the entire SD card
before attempting this.***

    mv .A001 eonebook                   # rename the official firmware
    cp $DEONEBOOKDIR/deonebook .        # copy over the key extractor
    cp $DEONEBOOKDIR/extract.sh .A001   # copy over the extraction script

Put the SD card in your eOneBook and open it up. When the official interface
comes up, the key extraction is complete. Close the eOneBook and there will be
a file called `eonebook.hex` containing the encryption key for that card.

Decrypting
----------

Eventually this will be integrated into DeOneBook itself. For now, you can use
the extracted key along with `openssl` to decrypt files. Replace `$KEY` with
the hex key, either as printed with the native version or the contents of
`eonebook.hex` from the cross-compiled version. Replace `$INFILE` with the name
of an encrypted image (e.g. `002E`), and `$OUTFILE` with a path where you want
the decrypted image to go:

    openssl aes-128-cbc -d -iv 03030303030303030303030303030303 -K $KEY -in $INFILE -out $OUTFILE

You can do a whole directory with something like:

    for i in *E; do
        openssl aes-128-cbc -d -iv 03030303030303030303030303030303 -K $KEY -in $i -out $i.gray
    done
