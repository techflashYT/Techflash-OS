# ![Techflash OS Logo](/misc/logo_64x64_embed.png) Techflash OS

# What is this?

Yup, I actually did it, I (kinda) made an operating system!  
In its current state, it doesn't really do much,  
it kinda just barfs some messages on to the screen and down the serial port.

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

# Booting

### Virtual Machine

Pretty easy, just set the ISO as the CD/DVD drive of the virtual machine and start it up!

### Real hardware

* I ***HIGHLY*** recommend that you burn to a USB (or Re-Writeable CD/DVD) rather than to a regular CD/DVD. &nbsp;It would be a major waste of phyiscal discs since the USB can be rewritten, and a CD/DVD would be permanently written.

* Some programs for this include: [Rufus (Burn to USB)](), &nbsp;[Ventoy (Boot ISO from USB)](), &nbsp;[Etcher (Burn to USB)](), &nbsp;or any generic ISO burner for burning to a CD/DVD.

# Bugs

None currently.  But since functionaly is extremely limited, it's kind of hard for there to be any bugs.
