Jelikož se v zadání mluví o číslech velikosti až 400 bitů, byl jsem nucen k této semestrální práci přiložit semestrální práci č. 2 z předmětu KIV/CPP - Arbitrary precision int library

Program si nejprve před nastaví všechny mocniny dvou od 2^1 až do 2^400

Dále se generují parametry p, q a klíče
Nejprve se vygeneruje privátní klíč funkcí gen_private_key()
Tato funkce zajišťuje, že výsledná posloupnost čísel bude vždy super roustoucí tím, že se průběžně počítá suma všech předchozích vygenerovaných čísel
Takže každé nové číslo je vypočítáno jako náhodné číslo na 100 bitů + suma všech předchozích čísel + 1
Dále se na základě privátního klíče vygeneruje parametr q tak, že se vygeneruje náhodné číslo na 350 bitů, k němu se přičte suma všech čísel privátního klíče + 1
Jako další se generuje parametr p, ten se volí jako náhodné prvočíslo z rozsahu <3; MAX_INT>
Tím, že je p vždy prvočíslo je zajištěná nesoudělnost parametrů p a q, čímž je zajištěná existence multiplikativní inverze parametru p při kongruenci na zbytkové třídě modula q
Jako poslední se generuje veřejný klíč tak, že se každé číslo z privátního klíče přenásobí parametrem p a výsledek se moduluje parametrem q
Z dřívejších kroků je zajištěná super roustoucí posloupnost privátního klíče a nesoudělnost parametrů p a q, nezbývá než vygenerovat multiplikativní inverzi parametru p
Ta je zjištěna pomocí rozšířeného Euklidova algoritmy (implementace je nerekurzivní)

Všechny dosud vypočítané informace jsou dle zadání uložena do souborů: p.txt, q.txt, private_key.txt a public_key.txt (formát klíčů dodržen)

Dále je načten obsah celého adresáře validation/ a pro každý soubor v tomto adresáři se vykoná následující
Zjistí se jméno a přípona, soubor se načte binárně do vektoru bytů
Voláním funkce encode() se vstupní byty převedou na posloupnost zakódovaných čísel (vah věcí v knapsacku)
Zpětným voláním funkce decode() nad vahami věcí (zakódovaná čísla) se zpětně dostávají původní byty

Encode() je implementován následovně:
Nejprve se vstupní byty převedou na bity (neboť bloky jsou délky 250, což je bohužel 31.25 bytu, musíme pracovat s bity, ne s byty)
Bity se následně doplní paddingem nul zprava (do posledního bloku)
Výsledná váha každého bloku vzniká tak, že je-li bit jednička, privátní klíč na indexu daného bitu se přičítá do výsledné sumy
Takto algoritmus zákoduje každý blok

Decode() je implementován následovně:
Vstupními čísli, které určují váhy předmětů v knapsacku, se postupně iteruje
Každé takové vstupní číslo je nejprve přenásobeno multiplikativní inverzí čísla p, tento výsledek je dále modulován parametrem q
Pro každé takto spočtené číslo se iteruje pozpátku privátním klíčem, je-li dané číslo v klíči < než je naše vypočtené číslo, prvek je určitě součástí výsledku, odpovídající bit se nastaví na 1
Následně se od našeho čísla odečte číslo klíče odpovídající nastavenému bitu
Po všech iteracích je nutné odstranit nulové bity na konci (dříve vzniklý padding)
Výsledné bity jsou jen zpět převedeny na byty a zapsány do výsledného souboru

Spolu s výsledky také vzniká mezi-soubor *_decrypted_hexoutput.txt obsahující maximálně 100 prvních bytů dekódovaných bytů (hexadecimální zápis)