# LIN-barcodes2tree
 A tool to generate a phylogenetic representation of a LIN barcdoe taxonomy

 ## To compile the tool:

 ```
cc LIN-barcode2tree.c -o LIN-barcode2tree
```
## To run the tool:

```
./LIN-barcode2tree <INPUTFILE>
```

where the input file is of the format:

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




