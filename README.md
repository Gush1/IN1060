# Fokusringen – IN1060-prosjekt (våren 2025)

Dette er koden og dokumentasjonen til *Fokusringen*, et velferdsteknologisk prosjekt utviklet som del av IN1060 ved Universitetet i Oslo.

## Kort beskrivelse

Fokusringen er en fysisk timer laget for å hjelpe personer med ADHD med å strukturere fokusøkter og huske pauser. Den bruker en kombinasjon av en trykksensor, en knapp, NeoPixel-ring og Charlieplexede LEDs for å visualisere arbeidsøkter på 25 minutter og pauser på 5 minutter.

## Funksjoner

- Trykksensor starter en 25/5-økt
- 5 blå LED-segmenter lyser opp hvert 5. minutt
- Hele ringen lyser gult under pause
- Gule segmenter slukkes ett og ett
- Knapp for å avbryte og restarte økta

## Brukte komponenter

- Arduino Uno
- Trykksensor (FSR)
- Tactile button
- NeoPixel-ring (24 LEDs)
- 12 LEDs via Charlieplexing (D4–D7)
- Diverse motstander og ledninger
- 3D-printet base (SolidWorks, Bambu Lab, PolyTerra PLA)
