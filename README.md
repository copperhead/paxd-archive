PaX exception daemon 
---

`paxd` is a daemon for automatically maintaining PaX exceptions.
Typically you have to run `paxctl <flags> <executable>` for certain
executables when using PaX which can be cumbersome. `paxd` solves this
problem by maintaining a list of commonly used exceptions and
automatically applies them.

For example, with `Firefox` it will automatically disable PAGEEXEC,
EMUTRAMP, and MPROTECT (-pem) which is required to get the program to
run when PaX is enabled.

### How it Works

`paxd` re-applies exceptions whenever an executable is created or
replaced. Such as when a new binary is installed in /usr/bin. It also
applies all of the exceptions at start-up.

Since `paxd` watches the parent directory chain for each executable, it
has no problem dealing with the creation of an executable in a directory
that did not exist when it was started. It works fine with all package
managers it has been tested with.

### Configuration

The sample `paxd.conf` is targeted at Arch Linux, and the expectation is
that maintainers and/or users of other distributions will maintain a
modified version downstream.

The `paxd` daemon watches this configuration file and automatically
updates the exception list whenever this file is modified.
