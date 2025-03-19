# Binary Image Recognition using Hash Tables

This project implements a **basic image detection system** using **Run-Length Encoding (RLE)** and a **hash table**. It stores **28x28 binary images**, encodes them for efficient storage, and searches for query images in the hash table.

## Features
- Reads **binary images (28x28)** from text files.
- **Encodes images** using **Run-Length Encoding (RLE)** for efficient storage.
- Stores encoded images in a **hash table** with **linear probing** for collision handling.
- Allows users to **insert images** into the hash table.
- Supports **querying images**, checking if they exist in the table.
- If a match is found, **decodes and prints** the image; otherwise, displays the RLE string.

## Files
- `main.cpp` → Main program handling **image insertion, encoding, querying, and hash table operations**.
