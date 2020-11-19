DeOneBook
---------

This program is designed to decrypt the contents of eOneBook SD cards. It is
able to extract the encryption keys from SD cards to make this automatic with
the proper hardware. Without hardware support, decryption is a two-step
process, extracting a key using the eOneBook itself, and decryption using that
key.

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

Automatic Decryption
--------------------

If your system has a directly connected SD card reader, you can decrypt files
with:

    deonebook [-d devicename] <input-file> <output-file>

The default device is `mmcblk0`. If your SD card reader is something else
(n.b. it *must* be an `mmcblk` device), specify that via the `-d` option.
Replace <input-file> with the path to an encrypted file (e.g.
`/media/sd/001/100E`) and <output-file> with the path to place the newly
decrypted file. DeOneBook will determine the encryption key by reading
information directly from the SD card.

On-device Key Extraction
------------------------

If your system does not have a directly connected SD card reader, you can
use your eOneBook device to extract the encryption key. You'll need to prep
the SD card to execute the program and save its output. First, make sure the
SD card is not write-protected by moving the write-protect slider to the end
of its slot nearest the pins of the card. Mount the SD card somewhere you can
write to it, `cd` to that mount point, and run the following commands,
replacing `$DEONEBOOKDIR` with the path to the compiled DeOneBook.

***Be very careful with these commands, as if you lose or overwrite the
original `.A001` file, you will not be able to use that SD card in your
eOneBook anymore. You are strongly advised to back up the entire SD card
before attempting this.***

    mv .A001 eonebook                   # rename the official firmware
    cp $DEONEBOOKDIR/deonebook .        # copy over the key extractor
    cp $DEONEBOOKDIR/extract.sh .A001   # copy over the extraction script
    chmod +x deonebook .A001            # ensure deonebook files are executable

Put the SD card in your eOneBook and open it up. When the official interface
comes up, the key extraction is complete. Close the eOneBook and there will be
a file called `eonebook.key` containing the encryption key for that card.

Manual Decryption
-----------------

Once you have extraced the key using your eOneBook, you can use that key to
manually decrypt that SD card's files. Run:

    deonebook -k $KEY <input-file> <output-file>

Replace `$KEY` with the contents of `eonebook.key` from the previous step (you
could also simply replace it with `$(cat eonebook.key)`). Replace <input-file>
with the path to an encrypted file, and <output-file> with the path for the
newly decrypted file.
