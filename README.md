# sfp-i2c [en]
## what?
SFP transceiver use I2C to provide information about its manufacturer, the
supported capabilities and more.
Most modules also provide a diagnostic monitoring interface known as DOM,
which can also be read.

## how to interface?
There are different alternatives to interface a transceiver. I'm using a
TwinGig Converter Module from Cisco, which allows to interface two SFP
modules by the use of a PCA9544A.

## specification
look into doc/

# sfp-i2c [de]
## wat?
SFP(+)-Module (und noch einige andere) haben eine I2C-Schnittstelle, um
diverse Werte auslesen zu können. Standardmäßig vorhanden sind
Informationen wie Hersteller, Teilenummer, Seriennummer und andere, zudem
können auch Diagnoseinformationen vorhanden sein.

## Interface?
Da gibt es verschiedene Möglichkeiten. Einerseits kann man natürlich direkt
am SFP-Modul löten, dann gestaltet es sich aber mit der Weiterverwendung
etwas schwierig. Ich verwende ein TwinGig Converter Module von Cisco, da
erhält man direkt zwei SFP-Slots, zwischen denen man ebenfalls via I2C
umschalten kann. Daher wird da wohl noch etwas Code dazu drin landen.

## Mehr Informationen?
Benutze die SFF-Spezifikationen im doc-Verzeichnis
