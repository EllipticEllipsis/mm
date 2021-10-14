# The merging process

## Optional: Documentation

It is helpful to document the functions and variables in the actor before you Pull Request it. The aim is to provide code that is sufficiently clear to be self-documenting, but it is worth leaving a comment on anything you find obscure or confusing. (Pull Request reviews will let you know if you are leaving too many comments.) Useful things to do documentation-wise:

- Name all (or most) of the functions.
- Name all the variables in the actor struct.
- Create enums for params, and any other numbers that would benefit from that sort of clarity.

You can test things using the practice rom for a retail version (watches and memory view is especially helpful), as well as the generated rom with Project 64 and something like Spectrum. The most important things to know if using a retail version to test are:

- all the addresses will be different
- actor structs in debug have 10 bytes of padding at the end, so subtract `0x10` from any subsequent offsets.

If you want to use `diff` after renaming anything, particularly functions, remember to copy the `build/` folder into `expected/` so use the correct symbols. *Make sure that `make` gives `OK` before doing this, or you're going to get very confused.*

Finally, *if you are not sure what something does, either ask or leave it unnamed: it will be less confusing later if things are unnamed than if they are wrongly named*

## Preparing to PR

### Change the `spec`

Specifically, to use the automatically generated reloc, rather than the original. In the case of an entirely matched actor, you find the section relating to the actor that you edited before:

```
beginseg
    name "ovl_En_Recepgirl"
    include "build/src/overlays/actors/ovl_En_Recepgirl/z_en_recepgirl.o"
    //include "build/data/overlays/actors/ovl_En_Recepgirl.data.o"
    include "build/data/overlays/actors/ovl_En_Recepgirl.reloc.o"
endseg
```

and change to use our reloc:

```
beginseg
    name "ovl_En_Recepgirl"
    include "build/src/overlays/actors/ovl_En_Recepgirl/z_en_recepgirl.o"
    include "build/src/overlays/actors/ovl_En_Recepgirl/ovl_En_Recepgirl_reloc.o"
endseg
```

(copy the path, then copy the directory name and put `_reloc.o` after it).

### Non-matchings

If you can't match a function even with everyone's, don't worry overlong about it. Hopefully you can get it to do the same thing as the original (non-matching), and then you set it up to use the original asm for the matching build, and your code for the non-matching. This looks like

```c
#ifdef NON_MATCHING
// Helpful comment about the nature of the nonmatching
void function() {
    // ...
}
#else
#pragma GLOBAL_ASM(asm/.../function.s)
#endif
```

in the C file. Also, due to the way `GLOBAL_ASM` works, we also cannot use generated reloc for overlays with nonmatchings, so we have to do the same thing for the reloc file in the spec:

```
beginseg
    name "ovl_En_Recepgirl"
    compress
    include "build/src/overlays/actors/ovl_En_Recepgirl/z_en_recepgirl.o"
#ifdef NON_MATCHING
    include "build/src/overlays/actors/ovl_En_Recepgirl/ovl_En_Recepgirl_reloc.o"
#else
    include "build/data/overlays/actors/ovl_En_Recepgirl.reloc.o"
#endif
endseg
```

Ideally you should at least be able to get a function to have equivalent behaviour; if not, and you have exhausted all other avenues of getting help, it should be marked in the C file as `NON_EQUIVALENT`, in the same way as a nonmatching.

### Format

Run the formatting script `format.sh`, to format the C files in the standard way we use. If you have some arrays or struct definitions in your file, check that they have not been obnoxiously padded out: you can usually get a better format without a final comma for short things.

### Merge master

To make sure the PR builds correctly with the current master, you need to merge `upstream/master` before you make the PR. This tends to break things, that you have to fix to get it to compile correctly again.


## Pull Requests

Push commits to your fork of the repository on GitHub, and then open a pull request. Name the PR something sensible, like

- `EnRecepgirl OK and documented` (if all the functions match and your documentation is fairly complete)
- `EnRecepgirl OK` (if all the functions match)
- `EnRecepgirl (n nonmatching)` (if you couldn't get one or more functions to work, but to the best of your knowledge they are equivalent code)
- `EnRecepgirl (1 nonequivalent)` (if you couldn't get one or more functions to work, and do not believe the code has the same effect)

and so on, although these four tend to cover most cases. Feel free to add a comment describing anything interesting you had to do or issues in non-matchings.

Please also update the status of the file on Trello/the spreadsheet.


### Reviews

Pull requests may be reviewed by anyone (who knows enough about the conventions of the project), and all must be reviewed and approved by two leads and one extra contributor.

To implement suggestions made in reviews, it is generally easier to be consistent if you push more commits from your local branch. It is quite possible that in the meantime some other PR has gone in, and git will ask you to merge master before you add more commits. This is normally fairly painless, although often you have to resolve merge conflicts. If in doubt, backup your work before doing anything, and ask in Discord before doing anything drastic, or if you don't understand what git is telling you.

There is no need to wait for your PR to be approved and committed before working on your next actor.