# sfp-i2c
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
Benutze die SFF-Spezifikationen:
[Datenblatt des PCA9544A](http://www.nxp.com/documents/data_sheet/PCA9544A.pdf) (ist auf dem Twingigconverter enthalten)
[SFP Transceiver Spezifikation](ftp://ftp.seagate.com/sff/INF-8074.PDF)
[SFP+ Transceiver Spezifikation](ftp://ftp.seagate.com/sff/SFF-8431.PDF)
[Spezifikation des Diagnoseinterface](ftp://ftp.seagate.com/sff/SFF-8472.PDF)
