Použitá implementace pro BigIntegery: https://github.com/qiray/BigInteger (nepříkládám tu svou z KIV/CPP kvůli tomu konceptu, který způsoboval nemožnost překladu při nižších verzích C++)

Nejprve se načte soubor s čísly potřebnými k útoku
Osobně moc nevím k čemu použít bitovou délku čísla p a privátního klíče (označuji v kódu často jako x)

Útok probíhá hrubou silou a zkoušením všech možných hodnot x (privátní klíč) tak, aby platilo (g^x) % p == zadaná hodnota ze souboru
Optimalizací je zbavení se funkce na počítání mocnin (pow) a neustálým přepočítáváním od začátku
V průběhu iterací je předchozí mocnina vždy uložena v proměnné g_pow_x_mod_p, to že je modulována p nevadí a pro další iteraci je pouze hodnota přenásobena hodnotou g a opět modulována p
Když nastává rovnost, algoritmus vrací hodnotu, která se v průběhu napočítává v proměnné x

Výsledek je uložen v alices_private_keys_A20B0279P.csv
28226;0.035
56231;0.067
3727936;3.835
89496045;90.089

Je vidět, že nejdéle trvalo prolomit poslední číslo 89496045, neboť se jednalo už o větší čísla, než předchozí kombinace čísel