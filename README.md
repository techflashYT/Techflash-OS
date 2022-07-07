# ![Techflash OS Logo](/kernel/kernel/misc/logo_64x64_embed.png) Techflash OS

# What is this?

Yup, I actually did it, I (kinda) made an operating system!  
In its current state, it doesn't really do much,  
it kinda just barfs some messages on to the screen (if it can even do that [on your computer](#"error-os-requires-a-console-but-none-is-available"))

# Installation

* Clone the repo

  * `git clone https://github.com/techflashYT/Techflash-OS.git`

* cd into the folder

  * `cd Techflash-OS`

* Run the interactive configuration script

  * `./configure`

* Run the `all` script

  * `./all.sh` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(This might take a while, so give it about a minute)

* Done! &nbsp;You should now have a bootable ISO image in `bin/TFOS_ISO.iso`.

* I ***HIGHLY*** recommend that you burn to a USB rather than to a CD/DVD. &nbsp;It would be a major waste of phyiscal discs since the USB can be rewritten, and a CD/DVD would be permanently written.

* Some programs for this include: [Rufus (Burn to USB)](), &nbsp;[Ventoy (Boot ISO from USB)](), &nbsp;[Etcher (Burn to USB)](), &nbsp;or any generic ISO burner for burning to a CD/DVD.

# Bugs

Yes, there is indeed 1 single bug known, yes, **already**.

## "error: OS requires a console but none is available"

Currently there is an issue with the bootloader that is being used (GRUB).  
If your computer is UEFI/EFI based (2008-ish or newer), the console will not work.  
  
This is due to the fact that UEFI dropped support for printing text via VGA RAM.  
Since this is the only (easy) way to print text to the screen, you will need to  
enable compatability mode in your BIOS in order to boot the OS.  
It may be called "Compatability Mode", "CSM", or "Legacy Boot", among others.  
While a [fix](https://github.com/users/techflashYT/projects/1) is currently being worked on, do not plan for it to come out any time soon.  
