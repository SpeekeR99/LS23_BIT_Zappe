V zadání je řečeno, že se bude pracovat pouze s bloky velikostí násobku bytů až do 8 bytů (64 bitů)
Klíče jsou tedy násobky čtyř až do velikost 32 (v bitech)
Může se tedy pro liché násobky čtyř stát, že budu muset jeden byte půlit mezi levý a pravý podblok
Je proto nutné pracovat v bitech, ne bytech

Funkce main() si nejprve ověří existenci složky se zdrojovými daty
Pokud existuje, načte si celý obsah adresáře do RAMky
Pro každý soubor ze zdrojového adresáře validation/ se provede následující:
Nejprve se zjistí název a přípona souboru, načte se obsah souboru binárně do proměnné a zkontroluje se, že soubor nebyl prázdný
Vytvoří se nové soubory do složky out/ - <filename>_hexoutput.txt a <filename>.bin
Soubor <filename>.bin je naplněn navrácenými byty po volání funkce perform_feistel() s předáním vstupních bytů a příznaku, že se jedná o kódování
Dále je vytvořen soubor do složky decoded/ - <filename>.<extension> - ten je naplněn navrácenými byty po volání funkce perform_feistel()
s přednáním zakódovaných bytů a příznaku, že se jedná o dekódování
Nakonec je naplněn soubor <filename>_hexoutput.txt prvními sto input byty, sto zakódovanými byty a sto dekódovanými byty

Funkce perform_feistel() přebere jako parametr vstupní byty a příznak, zda se jedná o kódování či dekódování
Nejprve se volá funkce pro načtení klíče load_feistel_key - získá se tím klíč a velikost bloku v bitech
Následně se vstupní byty zkopírují a doplňují zezadu nulami (padding), dokud není velikost vstupních bytů dělitelná velikostí bloku v bytech
Pokud se jedná o dekódování (příznak z parametru), otočí se klíč, aby se v iteracích četl pozadu
Nyní se iteruje přes všechny bloky ve vstupních bytech, pro každý takový blok se vytvoří unsigned int proměnná reprezentující levý a pravý podblok
Unsigned int byl zvolen jelikož zadání omezuje velikost podbloku na 32 bitů - vše se přesně vejde do unsigned integeru
Pomocí bitových operací se naplní tyto dvě proměnné poměrně hektickým zápisem (viz kód) (dále už je to lépe čitelné)
Následně se provede tolik iterací, kolik bylo řádků v souboru keys.txt
Levá strana se nahradí pravou, pravá strana se nahradí výrazem: levá XOR ((pravá XOR klíč) AND klíč)
Po všech iteracích se do pomocného pole bitů nejprve zapíší bity z pravé strany, následně bity z levé strany (ekvivalent posledního prohození po iteracích)
Následně už se jen bity konvertují na byty a zapíší se do výsledného vektoru, který je funkcí vracen
Pokud se navíc jednalo o dekódování jsou z výsledného vektoru odebrány všechny nuly z konce vektoru (odebrání přidaného paddingu)

Funkce load_feistel_key() přebírá parametr cestu k souboru s klíčem a int pointer na velikost bloku v bitech (side effekt funkce, dvě "návratové hodnoty")
Funkce nejprve spočítá délku první řádky a stanoví ji za velikost podbloku, jestli platí, že tato velikost je: > 0 AND <= 32 AND % 4 != 0
Dále se kontrolují zbylé řádky, zda jsou stejně dlouhé jako řádek první
Po kontrole formátu se vše přečte řádek po řádku a bity se zpracují do jednotlivých bytů a ukládají se do proměnné typu unsigned int
(Opět unsigned int je příhodně volen kvůli omezení velikost na právě 32 bitů)
Vektor podklíčů (jednotlivé řádky) je návratovou hodnotou této funkce