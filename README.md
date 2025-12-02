# TIMESTOMP BOF

So it turns out that at the time of writing this, the timestomp functionallity in Cobalt Strike 4.12 is broken...

As a result, I decided to put together an updated BOF to mimic the original `timestomp` command and to add some additional functionallity.

This new implementation has the ability to copy the MAC times from a file (like the builtin command) and it has the ability to set the MAC time to a user defined value.

**Why is the BOF named STOMPTIME?**

I named the BOF command `stomptime` as to avoid any conflicts with the builtin `timestomp` command. Just in case people are using a Cobalt Strike version that has a functioning `timestomp` command :).


## Usage

If you want to copy the timestamp of a file:

`stomptime COPY <Target File> <Source File>`


If you want to edit the timestamp of a file to be a user defined timestamp:

`stomptime EDIT <Target File> <Timestamp (YYYY-MM-DDThh:mm:ss)>`


## OPSEC

This BOF only modifies the MAC timestamp of the $STANDARD_INFORMATION structure in the file MFT entry. Meaning that a persistent network defender would be able to detect this technique. This is not unique to this implementation of timestomping but holds true for the original `timestomp` function that is pre-baked within Cobalt Strike.

There has been lots of research on the identification of "Timestomping" and there are many different timestamps that a file has which happen to be much harder to change in user mode. 

