# LIN-barcodes2tree
 A tool to generate a phylogenetic representation of a LIN barcdoe taxonomy

 ## To compile the tool:

```
cc LIN-barcode2tree.c -o LIN-barcode2tree
```
## To run the tool:

```
./LIN-barcode2tree <BARCODE-FILE> [optional: <BARCODE-CATEGORIES-FILE>]
```

where the Barcode file is of the format:

```
NIRE-02009d_CH.1.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,-
NIRE-01ff2b_CH.1.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,1
NIRE-01feea_CV.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,2
NIRE-01fee6_CH.1.1.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,3
NIRE-01fec7_CH.1.1.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,4
NIRE-01ea4e_BA.2.75.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,-,5
NIRE-020039_BN.1.3.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,1,-
NIRE-01fb9a_BN.1.4_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,1,1
NIRE-01fa7c_BN.1.5_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,1,2
NIRE-01f8c0_BN.1.3.1_Omicron	-,-,-,-,-,-,-,-,-,-,-,-,1,3
```

The file `input_barcodes.txt` is provided as an example.

The output is a phylogenetic tree named `<INPUTFILE>.outtree.ph`

This can be visualised in a phylogenetic tree viewer, such as https://itol.embl.de/

## Optional

The Optional `<BARCODE-CATEGORIES-FILE>` allows the user to provide the %ID cutoffs used for each Barcode category, so that this information can provide branchlengths on the tree

This file should contain a single line with all the %cutoffs seperated by commas, like this:

```
0.30,0.50,0.60,0.70,0.80,0.90,0.95,0.97,0.98,0.985.0.99,0.999,0.9999,1.0
```

The example file `barcode-categories.txt` is provided to allow testing of this option with the provided example barcode file

The branch lengths add to the tree are `1-%ID` as provided by the `<BARCODE-CATEGORIES-FILE>` file.


## NOTES/CAVEATES

IF no `<BARCODE-CATEGORIES-FILE>` is provided, the tool assumes that there are 14 barcode categories. This can be changed in the code. Search for `14` in the file `LIN-barcode2tree.c` and chang it to the desired number and re-compile the tool.

All branchlengths are set to `1` in the absence of a `<BARCODE-CATEGORIES-FILE>`.


