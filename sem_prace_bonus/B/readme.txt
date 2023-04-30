Pro druhou šifru už jsem byl informován, že se jedná o Vernamovu šifru, neprováděl jsem tedy testy na žádnou jinou

Nejprve jsem analyzoval co se stane, když se XORuje malé a velké písmeno abecedy
Zjistil jsem, že: 'a' ^ ' ' = 'A' a 'A' ^ ' ' = 'a'
Tedy mezera pouze mění, zda je písmenko velké, či malé

Dále jsem zpozoroval, že v zašifrovaných textech jsou pouze hexadec číslice (0-9, a-f)
Navíc jsou všechny texty sudé délky
Interpretoval jsem proto dvojice hexadec čísel jako jednotlivé byty

Texty mají různé délky, proto jsem doplnil kratší texty o nulové byty do max. délky

Nejprve mi přišla zajímavá možnost hledat dvě nuly vedle sebe, tedy byte 0x00, to by znamenalo, že znak v původním textu je stejný jako znak v klíči
Později jsem toto pozorování vůbec nepoužil

Dalším zajímavým zjištěním bylo:
p1 ^ k = c1
p2 ^ k = c2
c1 ^ c2 = (p1 ^ k) ^ (p2 ^ k) = (p1 ^ p2) ^ (k ^ k) = p1 ^ p2
Tedy XOR dvou zašifrovaných textů stejným klíčem je stejný jako XOR původních textů

Využil jsem proto znalosti XORu a mezer z dřívějška
Udělal jsem XOR každé šifry s každou a analyzoval jsem místa, kde by mohla být mezera,
tedy místa kde znak byl (>= 65 && <= 90) || == 0
(nula právě tehdy když oba texty mají mezeru na stejném místě)

Vypsal jsem si tyto texty s mezerami do konzole jako otazníky pro písmena co zatím neznám a mezery pro místa, kde nejspíše mezera opravdu má být

Když už jsem měl jasně určené mezery, mohl jsem jasně určit několik znaků, neboť mezera vznikla tam, kde v jiném textu byl určitě alfa numerický znak,
konkrétně velké písmeno (A-Z)
Abych dostal původní, stačilo ho zmenšit -> XOR s mezerou

Dostal jsem tak tento výpis:
Text 1: ??pre? ?i aby to byl? sn?zsi pr?j iikabysaby?ynjps?
Text 2: ??s m? ?lne kapsy pr?kva?eni
Text 3: ??yz ?r?hrajete neza?ome?te na ?u vekci
Text 4: ??dne? ?ak abys byl ?tas?ny ne ?byssesbstast?yoeje?a? a?
Text 5: ??ela? ?ec ktere se ?oji?e je p?vnsikrokskbu?pglhu
Text 6: ?? le?s? rozsvitit b?t j?n malo? slickuynezy?rmdli?a?dt??
Text 7: ?? vs?c?ni kteri blo?di ?sou zt?aceni
Text 8: ?? ka?d?u krasnou ve?i j? nejak? dhuhabolest?
Text 9: ??sle?u? sve sny a z?en ?vuj zi?ot
Text a: ?sstlx ?ajdes v zivo?e c?stu be? phekazekaur?ivjpn?k? a??????
Text b: ??pr ?r?in bya oybyl? sn?zs npr? yiikabysaby?y

z textu 9 jsem byl schopen vykoukat okometricky "nasleduj sve sny a zmen svuj zivot"
Použil jsem další zpozorovanou vlastnost XORu: (pg = plaintext guess)
c1 ^ pg1 = (p1 ^ k) ^ (pg1) = k
Tedy stačilo vzít tento odhad "nasleduj sve sny a zmen svuj zivot" a vyxorovat ho se šifrou odpovídající textu 9
Dostal jsem tak první částečný klíč a větší části  zbylých textů

Takto jsem postupně odhadoval texty a iteroval (konkrétně sedmkrát) (vše je zdokumentované v komentářích v kódu)
Takto jsem se dostal nakonec v výslednému klíči (ne k celému (1024 bitů ze zadání))

Výsledný klíč (HEX) vypadá takto: b0665d5a7f06be14282fd63d1912f156e437ff1cae5793b902bc9362bcee8e603aedd0d14abea621869a1652222ff20485f9e3cd532cffda3f4e0