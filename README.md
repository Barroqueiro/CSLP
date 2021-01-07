# Project for the class of CSLP

## Deliverable nº 1

### Testing opencv libraries

Just some basic programs following some open cv tutorials as indicated in the project guide
 
## Diverable nº 2

### BitStreams
Write Bit Stream and Read Bit stream, to read and write from and to binary files

## Deliverable nº 3

### Golomb encode
Encoding and decoding integers to binary files using golomb codes with truncated code if the m isn't a power of 2

## Deliverable nº 4

### Prediction encoding
Encoding and decoding videos (intra-frame) using the linear prediction technics by JPEG (1..7) and a non-linear method (JPEG-LS)

To run the encoder

```bash
./testE [File to Encode] [Type of predictor] [M param of golomb code] [Name of the file to encode to]  [Type of the video]
```

## Deliverable nº 5

### Block encoding
Encoding and decoding videos (Hybrid encoding, inter and intra frame), encoding intra frames from frame to frame (configurable) and the rest using block encoding

To run the encoder

```bash
./testE [File to encode] [Block Size] [Search Space] [Type of the video] [Type of predictor] [M param of golomb code] [Period] [Name of the file to encode to]
```

## Deliverable nº 6

### Lossy encoding
Encoding and decoding videos (Hybrid encoding, inter and intra frame), encoding intra frames from frame to frame (configurable) and the rest using block encoding but adding lossy encoding to help with file compression

To run the encoder

```bash
./testE [File to Encode] [Block Size] [Search Space] [Type of the video] [Type of predictor] [M param of golomb code] [Period] [Quantization level] [Name of the file to encode to]
```
## Decoding

To run the decoder (Deliverables 4,5,6)

```bash
./testD [File to decode from]
```

## Parameters
    Predictors supported : JPEG 1 tru 7 (0..6) and JPEG-LS (7)
    Quantization Level :  1,2 can hardly notice a difference but 3 we start to see the errors
    
