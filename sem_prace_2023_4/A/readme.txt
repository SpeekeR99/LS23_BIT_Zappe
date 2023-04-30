Použitá a lehce upravená implementace SHA256: https://github.com/hlilje/sha-256

Implementace obsahuje soubor 10 000 nejčastějších hesel z odkazu ze zadání
Nejprve se tento soubor načte a nacachuje se 10 000 hashů do vektoru

Každý z crackovaných hashů je nejprve vystavěn slovníkovému útoku, prosté porovnání dvou stringů (10 000 krát)
Pokud slovníkový útok neuspěje, přijde na řadu útok hrubou silou
Pro tento útok byla vytvořena 4 vlákna, hlavní vlákno čeká, zda nějaké vlákno najde odpověď do 3 minut
Jedno vlákno prohledává pouze číselné kombinace (znaky: 012345689)
Druhé vlákno prohledává pouze písmenné kombinace (znaky: abcdefghijklmnopqrstuvwxyz)
Třeti a čtvrté vlákno prohledává všechny možné kombinace na dvou různých abecedách (012345689abcdefghijklmnopqrstuvwxyz, resp. abcdefghijklmnopqrstuvwxyz012345689)

Generování kombinací zajišťuje funkce gen_all_combinations(), která očekává jako předaný parametr počet symbolů, základní řetězec a abecedu znaků
Základní řetězec je užit pro kombinace délky >= 5, neboť by se všechny do paměti nemuseli vejít, jsou řešeny postupně jako kombinace délky 4 s pevným řetězcem na začátku
Abeceda znaků je odlišná pro každé vlákno, viz výše

(Co mě osobně zabralo pár hodin na rozluštění bylo, že mi nebylo nic nalezeno žádným útokem.. protože jsem nepočítal, že na konci každého hesla je znak nového řádku
Tedy heslo "napalm" nebylo zahashováno jako "napalm", ale jako "napalm\n")

Výsledek je uložen v cracked_results_A20B0279P.csv
bb18f2412a61c5d223ca68a050b08ce6754969e12e0cd40908ef376ff90bc993;18562;295133;1.512
412c08251937e45153fd61dc56fae6aa7dfe83e05140e9aa6f5768a254611793;napalm;9645;0.004
0d4c15e98733d022700b0b9d7080eafd12d3649745b89de4e32fcdcda76ce3ba;lncdw;15255551;89.47
144b45c1d97892a54ec40224ff4b067f0fd67d68390635c547c224af55a86242;03pno;7583511;43.811

Je vidět, že nejdéle trvalo prolomit heslo lncdw, neboť l je cca uprostřed abecedy, dostane se k němu nejspíše druhé vlákno jako první


Zamyslete se nad bezpečností hesel a jejich způsobu ukládání. Proč jsou důležité takové
zásady bezpečnosti jako solení a hlavně minimální požadavky na délku a sílu hesla?
- solení je důležité pro zvýšení bezpečnosti daného algoritmu -> pomáhá se bránit proti slovníkovému útoku, slovník pak nejde jednoduše nacachovat jako jsem to udělal v této práci
- délka a síla hesla opět zvyšuje počet možných kombinací