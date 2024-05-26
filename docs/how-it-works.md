# How It Works

# Table of stuff

-   [Introduction](#introduction)
-   [Basic Idea](#basic-idea)
    -   [What's an OS?](#whats-an-os)
    -   [What is an OS made of?](#what-is-an-os-made-of)
    -   [How does it work together? _(IMPORTANT)_](#how-does-it-work-together-important)
-   [Booting](#booting)
    -   [The BIOS](#the-bios)

## Introduction

I'll be writing this for people searching for a project to learn from.
They'll be able to learn OSDev by understanding the structure of this OS.
This is basically for people who don't have decent knowledge 
about OS Development.

## Basic Idea

### What's an OS?

An **Operating System** is a piece of software that links the hardware 
with the software. For example, Windows takes care of the hardware so it can
run your programs and apps. Like now! You're reading this using an OS!
The browser you're using, from Android, MacOS, Windows or Linux is a program
that the OS runs by doing quirky stuff with hardware, like the screen,
to show you this document.

For short: An OS is a software that lets you stuff with your computer.
It manages resources like data and _<insert what else>_.

> You should read [this](https://wiki.osdev.org/Introduction#What_is_an_Operating_System.3F).

### What is an OS made of?

In a really basic idea, an OS is composed of two parts:
-   **The Boot Sector:** Its job is to start your Operating System properly,
    and this by putting the computer in a state expected by the Kernel.
-   **The Kernel:** Its technically the Operating System. Right now, 
    you're reading this through a Kernel. This Kernel manipulated resources
    and pixels to show you what you're reading right now.

I recommend checking out the [OSDev Wiki](https://wiki.osdev.org/).

> You should read about the Kernel [here](https://wiki.osdev.org/Kernel)
and [there](https://wiki.osdev.org/Introduction#What_is_a_kernel.3F),
and the [Boot Sector](https://wiki.osdev.org/Boot_Sequence).

### How does it work together? _(IMPORTANT)_

I'm assuming that you now understand what's a bootsector and a Kernel.
If not then, welcome to the club! Because before I was just like that!
So let's put this all together to understand better.

When turning on your computer, a little piece of software is copied
from the ROM to memory. This is the [**BIOS**](https://wiki.osdev.org/BIOS), 
or the Basic Input Output System. From its name, you can understand that it does
stuff. It is implemented because, how are you gonna control basic stuff?
By sending digital inputs _(1s & 0s)_?

The BIOS will go look for a valid boot sector at the first sector on the disk.
If it finds the boot signature `0xAA55` then it means that this sector's code
can be executed _(we made sure this is a boot sector)_.

## Booting

### The BIOS

The BIOS provides a bunch of useful stuff, and we're interested in two 
in particular:
-   **[Interrupts](https://wiki.osdev.org/Interrupts):** Signals sent by devices
    like the keyboard or a disk to the CPU telling it to stop whatever it's doing
    and do something else. For example, we can use the Interrupt **"10h"**
    from the BIOS Interrupts Calls to tell the CPU: 
    "Hey! Stop what you're doing, and print the character stored in `al` to 
    the screen! I've set `ah` to `0x0e` _(function 2)_, meaning you have to 
    display a character!".
-   **Executing the Boot Sequence:** The BIOS looks for a Boot Sector so it executes it.
    But don't execute any! The BIOS knows that the Boot Sector should be the first sector 
    on the disk and be exactly 512 bytes long. The Boot Sector must have 
    a [**"Boot Signature"**](https://wiki.osdev.org/Boot_Signature#Master_Boot_Record)
    which is `0xAA55`. It tells the BIOS that this is a valid boot sector and so
    it can execute its code.

The BIOS is extremly important when booting our OS, except if you can code you own BIOS
from scratch (: _(But if you ever plan on doing so, 
take a look at [this](https://stackoverflow.com/a/10891215))_.