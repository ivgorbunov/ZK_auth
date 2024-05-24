# Zero-Knowledge Authentification

This project implements the Feige-Fiat-Shamir identification scheme. This scheme is an Zero-Knowledge Interactive protocol, that makes it a very secure protocol for authentification.

Also the project has an effective implementation of modular BigInteger arithmetics.


## Build, run and test

How to use this repository
1. Clone it:\
`git clone git@github.com:ivgorbunov/ZK_auth.git`
2. Run `cmake`:
```
mkdir build
cd build
cmake ..
```
4. To build all the targets run\
`make`
5. To run tests\
`make test` or `make; ./ZK_auth_test`
6. To check main functions\
`make; ./ZK_auth`


### Developers
- Ivan Gorbunov (@ivgorbunov)
- Vsevolod Nagibin (@nagibin-v)
- Nikita Andrusov (@AndrusovN)

Part of a project for Complexity theory course, spring 2024
