Pro první šifru jsem začal zkouškou, zda se nejedná o Vigenerovu šifru
Tato zkouška probíhá jednoduchým nalezením opakujících se slov, minimální vzdáleností v počtu charů mezi opakujícími se slovy
Následně se najdou všechny faktory všech opakujících slov a hledá se průnik
Takový průnik nebyl však nalezen -> nejedná se o Vigenera

Frekvenční analýza také nevypovídala o jednoduchém posunu -> není to ani Cézar

Proto jsem vzal z wikipedie frekvenční analýzu obecného anglického textu a pokusil se napamovat pravděpodobnosti písmen
ze šifry na pravděpodobnosti výskytu písmen v angličtině
Navíc jsem provedl frekvenční analýzu bigramů a trigramů (využil jsem opět jako referenci statistiky z wikipedie
a uvedené webové stránky v komentáři v kódu)
Pomocí bigramové analýzy jsem byl schopen namapovat časté zm, md a ft
zm a md se navíc vyskytovalo i v kombinaci v trigramové analýze jako zm
Usoudil jsem tedy, že zm -> th a md -> he (navíc tedy zmd -> the)
Další byl bigram ft, dle referencí by mohl odpovídat na "in"
Jelikož už jsem měl ft jako in, tak jsem mohl vzít opět častý trigram fty a napamovat ho dle reference na ing
V kódu v komentářích je postupně uvedeno, jaké trigramy jsem analyzoval:
zmp a mpz -> th? a h?t (podle referencí jsem usoudil, že se jedná o tha a hat)
když už jsem měl a, chtěl jsem najít all (opakuje se 'l'), našel jsem prr
Dále jsem and napamoval na pta, her a ere na mdq a dqd
Nakonec jsem už jen použil trojici thi, tio a ion -> hledal jsem zm? z?! a ?!t
nasel jsem zmf, zfc a fct

Díky bigramové a trigramové analýze jsem usoudil následující substituce za správné:
z -> t
m -> h
d -> e
f -> i
t -> n
y -> g
p -> a
r -> l
a -> d
q -> r
f -> i
c -> o

Našel jsem tedy už 12 písmen a chybělo jen zbývajících 14

Nejprve jsem provedl okometrický bruteforce, ale pak jsem ze zadání zjistil, že program musí automaticky sám usoudit, zda má správnou abecedu

Využil jsem tedy slovník z uvedené adresy v komentářích v kódu a napsal jsem jednoduchý genetický algoritmus
Na začátku každé iterace se inicializuje zkoušená abecedu na tu dosavadní nejlepší
Hodnocení abecedy probíhá na základě slovníku a správnosti slov
K mutacím dochází s pravděpodobností (1 - nejlepší dosavadní skóre) (skóre <0;1>)
Navíc si algoritmus pamatuje tzv. špatné a dobré prohození
Pokud nějaké prohození písmen v abecedě (mutace) vede ke zlepšení skóre, uloží se do listu dobrých prohození
Pokud určitá prohození skóre zhoršují, uloží se do listu špatných prohození
Špatná prohození se uskutečňují s pouhou 50 % pravděpodobností
Dobrá prohození se navíc stanou po mutacích pokaždé
Navíc po dobrých prohozeních dojde k prohozením z výše uvedené bigramové a trigramové analýzy (12 písmen je jistých)
Vypisuje se každých 10 generací
Každých 50 generací se zapomíná na všechny "špatné prohození" neboť mezi nimi mohlo být i správné řešení
Každých 100 generací se zapomíná na všechny "dobré prohození" neboť mezi nimi klidně mohlo být i špatné prohození

Když je skóre dostatečně dobré (konstanta byla zvolena cca 0.9) algoritmus ukončuje svůj běh
Přikládám screenshot s ukázkou, že algoritmus opravdu někdy doiteruje (někdy to trvá až moc dlouho :/ (3370 generací nejhorší případ))

Výsledný dešifrovaný text je The Hound of the Baskervilles od Sir Arthur Conan Doyle