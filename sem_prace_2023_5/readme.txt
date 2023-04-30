Použitá a lehce upravená implementace SHA256: https://github.com/hlilje/sha-256
Použitá implementace pro BigIntegery: https://github.com/qiray/BigInteger (nepříkládám tu svou z KIV/CPP kvůli tomu konceptu, který způsoboval nemožnost překladu při nižších verzích C++)

Nejprve se zahashuje moje osobní číslo (A20B0279P) pomocí SHA256
Hexadecimální otisek se převede na dekadické číslo pro pohodlnější aritmetiku později
(4bc8e0b9c2797e89a34c3d35b4daacffb4088e744439b4c876ccde5a6b0ffef3)
(34278384055953034937792344621743829904637077572711149882491112981826154528499)

Následně se vygeneruje parametr p jako náhodné prvočíslo
Pak se generují parametry g a x jako náhodná menší čísla než p
Parametr y se vypočítá jako g^x mod p
Trojice (p, g, y) tvoří veřejný klíč -- ověřování podpisu
x představuje privátní klíč -- vytvoření podpisu

Podpis je vytvořen vygenerováním náhodného čísla k, které má s (p - 1) nejvyšší společný dělitel 1
(k je v implementaci voleno jako prvočíslo -- zajištěna nesoudělnost)
Podpis vytvořený ElGamalem jsou dvě čísla (a, b)
První číslo a se spočítá jako g^k mod p
Druhé číslo b se spočítá z rovnice M = (xa + kb) mod (p - 1)
(vyjádření b: b = k^-1 * (M - xa) mod (p - 1)
Tato čísla jsou uložena do signature.txt ve formátu <a>,<b>

Ověření podpisu probíhá všech dostupných hodnot, až na k a x, ta jsou utajená
Ověření podpisu == ověření rovnosti: y^a * a^b mod p == g^M mod p
Všechny hodnoty jsou známé a ověření je tak snadné a rychlé