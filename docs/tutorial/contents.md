# Getting started

## [Introduction to decomp](introduction.md)
- What we are doing
- Structure of the code

## Pre-decompilation
- Building the repo (follow the instructions in the README.md)
- Most of us use VSCode. Some useful information is [here](vscode.md).
- Choosing a first actor (You want something small that has simple interactions with the environment. A simple NPC can also work, and is what we will use as an illustration for most of the tutorial. There is a collection of actors we think are suitable for beginners on the spreadsheet or Trello)

## Decompilation

- [Begining decompilation: order, Init and the actor struct](beginning_decomp.md)
	- Order of decompilation
	- Init and common actor features
		- Initchains
		- Actors and dynapoly actors
		- Colliders
		- Skelanime
	
	- Matching
		- Using diff
		- control flow (branches) -> instruction ordering -> register allocation -> stack

- [The rest of the functions in the actor](other_functions.md)
    - Order of decompilation
    - Action Functions and other functions

	- More on matching: the permuter

- [Draw functions](draw_functions.md)

- [Data, migration and non-migration](data.md)
	- Importing the data: early and late
	- Fake symbols
	- Inlining

## [Object Decompilation](object_decomp.md)
- Object files
- How we decompile objects
- [Example](object_decomp_example.md)


## After Decompilation

- See the [CONTRIBUTING.md](../../CONTRIBUTING.md) for most of the details for submitting PRs. Remember to format again after making adjustments from reviews!
- More information about specific preparations is in [this document](merging.md).

## Appendices
- [Types, Structs and Padding](types_structs_padding.md) (a miscellany of useful stuff)
- [Helper scripts](helper_scripts.md)

To be written, maybe

- How we use git and GitHub
- Some notes on the basic structure of N64 MIPS
- Glossary
- Conventions