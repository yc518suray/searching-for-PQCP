# searching-for-PQCP

This program searches for PQCPs (Periodic Quasi-Complementary Pairs), as an project of the class Sequence Design for Communications (通訊序列設計), NCKU, 2025 fall.

This program generates ADF (Almost Difference Families) candidates first, and then matches these candidates to find ADFs. Afterwards, the ADFs are converted to binary sequence. Note that the generation of candidates is not exhaustive, and is limited by the orbits available of the specific sequence length.

# Compile

```shell
./compile.sh
```

However, length and size of index sets and choices and stuff should be modified in the source code before compilation.

# Usage

```shell
./main.sh
```

Each program can also be executed separately, with careful handling on input/output file names.

# Notes

Pull requests with explanations are welcome.
