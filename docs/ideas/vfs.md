# Virtual File System Idea

**REMEMBER:** We're just trying to make fs operations easier.

## The Concept

We could have two types: a **file** type and a **mountpoint** type.
When we want to do operations or manipulate a certain file, we'd
search each mountpoint for a **File ID**.
We could achieve this with a HashTable.

For example, if I wanted to write *"xxx bababoey xxx"* to
*"extra.txt"*, I'd first **open the file** using some kind
of `open` function. Then, this `open` function would give me 
a file which is of the `file_t` type. From that file I can
get its ID. Before that, when creating a file, we gotta make
sure that it has a unique ID. Then, when I call the `write` or
`read` function, I'd just take the ID provided with the arguments,
search **each** mountpoint for a file with the **same ID**.

That's slow and really un-optimized, but I'll stick to the core/base
of the idea.