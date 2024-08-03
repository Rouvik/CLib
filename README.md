# CLib
CLib is a simple header only library to add common missing components in C,
CLib is build to be generic with data structures and have minimal code in its components

# Why CLib?
Often in C you will require components like dynamically sized strings, linked lists, vectors, cross platform file system handlers, etc.
But this is not as easy as it seems, especially with the ton of code associated with it, also code quality matters as an important factor.
This is where CLib comes, a simple header only library containing code to fill in these requirements and help devs focus more into their own application.

# Dev
The ./dev folder contains base type code and other CLib specific code used to build docs and source for templated libraries
and has no real value for users, people interested to update CLib may update the code in ./dev and use it to update code in the actual
templated headers and write docs for such templated headers

# How to CLib?
Have a look at ./examples/ folder for examples

# Author
Rouvik Maji [Gmail](mailto:majirouvik@gmail.com)
