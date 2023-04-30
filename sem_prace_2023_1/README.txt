Řešení je zhotovené v jazyce C++ (standard C++20, resp. C++23)

Hlavní funkce main() je přepsáním z dodané Pythonovské kostry
Funkce tedy kontroluje existenci složky decoded (případně ji vytváří)
Dále funkce detekuje veškerý obsah adresáře validation
Pro každý soubor tohoto adresáře se vytvoří výstupní soubor (zatím jako název) ve formátu "<FILENAME>___<STEGANOGRAPHY_IMG>"
Dále se volá funkce hide(), která bude popsána níže
V druhé části se pro veškerý obsah adresáře out volá níže popsaná funkce decode()

Funkce is_file_too_big() je implementována pouze tak, že porovnává dvě celá čísla
První porovnávané číslo je dostupný počet bytů = steganography_img_size - BMP_HEADER_SIZE (tedy velikost weber.bmp mínus 54)
Druhé vstupní číslo je velikost souboru, který chceme schovávat, plus velikost metadat, to celé krát osm
((input_file_size + METADATA_SIZE) * BITS_IN_BYTE) (krát osm protože jeden byte schováme na 8 bytů)

Funkce hide() načte binárně dva soubory - obrázek do kterého schováváme a soubor který se snažíme schovat
Načte si oba tyto obrázky do paměti RAM zapomocí struktury vektoru (arraylist)
Než započte samotná steganografie, zavolá se funkce is_file_too_big() a případně funkce ukončuje svůj běh předčasně (soubor by se do obrázku nevešel)
Samotná steganografie je provedena následovně, nejprve se vytvoří vektor výstupních bytů
Prvních 54 bytů (hlavička BMP formátu) se prostě překopíruje z obrázku, do kterého schováváme data
Následujících N bytů vznikne tak, že se vždy LSB bit obrázku zahodí a nahradí se jedním bitem dat, která se snažíme schovat (realizováno pomocí bitových operátorů & a |)
(Jeden schovávaný byte je tak uchován na 8 bytech v obrázku)
Prvních 16 bytů jsou metadata (8 bytů pro příponu (extension) a 8 bytů pro velikost)
Když jsou všechny byty schované dokopíruje se původní obrázek až do konce
Výsledné byty jsou jen uloženy do souboru (do složky out s výše popsaným formátem jména)

Funkce decode() načte binárně jeden soubor - obrázek se skrytými daty
Pomocí regulárního výrazu "(.*)___(.*)" je rozparsován název souboru, tím se získá původní jméno
Výsledné byty se načtou opět do RAM zapomocí struktury vektoru (arraylist)
Byty jsou tvořeny postupným čtením jednotlivých LSB bitů a následnými bitovými posuny
(Nejprve jsou načteny přípona a velikost souboru z prvních 16 bytů)
Výsledné byty jsou jen uloženy do souboru (do složky decoded s původním jménem a příponou)