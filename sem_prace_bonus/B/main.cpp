#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

std::string string_xor(const std::string &a, const std::string &b) {
    std::string result;
    for (int i = 0; i < a.length(); i++)
        result += (char) (a[i] ^ b[i]);
    return result;
}

std::string byte_vector_xor(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b) {
    std::string result;
    for (int i = 0; i < a.size(); i++)
        result += (char) (a[i] ^ b[i]);
    return result;
}

std::vector<unsigned char> string_to_byte_vector(const std::string &s) {
    std::vector<unsigned char> result;
    for (char i : s)
        result.push_back(i);
    return result;
}

int main() {
    std::string ciphered_text_2_1 = "de6315a7c29a4b922ba26eb44439669e76864e639eea96175aea71d5b312ce8be4404984f0b028c7d501e4e37a724e4a7f5321";
    std::string ciphered_text_2_2 = "d36716f5ca914b912eec6af6567862822fc4477d94a193184bf56cd5";
    std::string ciphered_text_2_3 = "db621caf8780198e2af06ebc586d77d138814d6e81a5881c55e4679cfd039c9afb405688bbb223";
    std::string ciphered_text_2_4 = "da6301bbc29a4b9523e92fb75f6061d1349d5b2f82be840a4ffe7b9cfd079c8fec191a9eb5a26acdd240f5ee782b4f0f6545267fe458b45e";
    std::string ciphered_text_2_5 = "c56200b9c6844b9727e12fbd497c60947697522f93a58f1056f522d6f642cc9cf80e53cdbba325d5864aa6ef6522474c6755";
    std::string ciphered_text_2_6 = "da6345b9c280188862f060ac4e6f7b853f90176d88bec5135efe22d1f20ed39bae134c84b3ba3f9ec844fcba66204d446349263eea1ca15fba";
    std::string ciphered_text_2_7 = "de6345a3d49908892ceb2fbd497c609876865b6084ae8c5951e36dc9b318c89cef035f83b9";
    std::string ciphered_text_2_8 = "ca6745bec68a0f8e37a264a45c6a7c9e23c4416a92a3c5135eb06cd9f903d797ae044898b8f128d1ca44f5ee7f";
    std::string ciphered_text_2_9 = "de6716b9c2941e8b62f179b31d6a7c88768517759caf8b5948e677d6b318d598e114";
    std::string ciphered_text_2_10 = "da6316a1cb994b8f23e86bb34e3964d12c8d416085afc51a5ee376c9b300d994ae104888bbb030dbcd01f3e8753b564a2f4e2134ff51f55caa8bc69085";
    std::string ciphered_text_2_11 = "db621caf879e0e972bf12fbd5c7432813a915d6a82ea9f185ffe7b9ce50bc89cae0e5f83b9f13acccf5be8f3602b";
    std::array<std::string, 11> ciphered_text_2 = {
            ciphered_text_2_1,
            ciphered_text_2_2,
            ciphered_text_2_3,
            ciphered_text_2_4,
            ciphered_text_2_5,
            ciphered_text_2_6,
            ciphered_text_2_7,
            ciphered_text_2_8,
            ciphered_text_2_9,
            ciphered_text_2_10,
            ciphered_text_2_11
    };

    std::cout << "'a' xor ' ' = " << string_xor("a", " ") << std::endl;
    std::cout << "'z' xor ' ' = " << string_xor("z", " ") << std::endl;
    std::cout << "'A' xor ' ' = " << string_xor("A", " ") << std::endl;
    std::cout << "'Z' xor ' ' = " << string_xor("Z", " ") << std::endl;
    std::cout << "'a' xor 'a' (97 xor 97) = " << (97 ^ 97) << std::endl;
    std::cout << "'a' xor 'b' (97 xor 98) = " << (97 ^ 98) << std::endl;
    std::cout << "'a' xor 'y' (97 xor 121) = " << (97 ^ 121) << std::endl;
    std::cout << "'a' xor 'z' (97 xor 122) = " << (97 ^ 122) << std::endl;

    std::cout << "length 1: " << ciphered_text_2_1.length() << std::endl;
    std::cout << "length 2: " << ciphered_text_2_2.length() << std::endl;
    std::cout << "length 3: " << ciphered_text_2_3.length() << std::endl;
    std::cout << "length 4: " << ciphered_text_2_4.length() << std::endl;
    std::cout << "length 5: " << ciphered_text_2_5.length() << std::endl;
    std::cout << "length 6: " << ciphered_text_2_6.length() << std::endl;
    std::cout << "length 7: " << ciphered_text_2_7.length() << std::endl;
    std::cout << "length 8: " << ciphered_text_2_8.length() << std::endl;
    std::cout << "length 9: " << ciphered_text_2_9.length() << std::endl;
    std::cout << "length 10: " << ciphered_text_2_10.length() << std::endl;
    std::cout << "length 11: " << ciphered_text_2_11.length() << std::endl;

    auto min_len = 56 / 2;
    auto max_len = 122 / 2;

    // Zjisteni: v textech jsou pouze hexadecimalni cislice (0-9, a-f)
    // Zjisteni: texty jsou sude delky
    // Brat znaky po dvojicich, aby tvorily byte?

    std::array<std::vector<unsigned char>, 11> ciphered_text_2_bytes;
    for (int i = 0; i < ciphered_text_2.size(); i++) {
        std::vector<unsigned char> bytes;
        for (int j = 0; j < ciphered_text_2[i].length(); j += 2) {
            std::string byte = ciphered_text_2[i].substr(j, 2);
            bytes.push_back((unsigned char) std::stoi(byte, nullptr, 16));
        }
        ciphered_text_2_bytes[i] = bytes;
    }

    for (auto & ciphered_text_2_byte : ciphered_text_2_bytes) {
        while (ciphered_text_2_byte.size() < max_len)
            ciphered_text_2_byte.push_back(0x00);
    }

    for (int i = 0; i < ciphered_text_2_bytes.size(); i++) {
        std::cout << "Text " << i + 1 << ": ";
        for (unsigned char j : ciphered_text_2_bytes[i])
            std::cout << j << "";
        std::cout << std::endl;
        std::cout << "Text " << i + 1 << " (HEX): ";
        for (unsigned char j : ciphered_text_2_bytes[i])
            std::cout << std::hex << (int) j << "";
        std::cout << std::endl;
    }

    // Zajimave jsou dve nuly vedle sebe (0x00)
    // Kdy vznikne 0 pri operaci XOR? kdyz jsou oba operandy stejne -> znak textu je stejny jako znak klice

    // Zjisteni: https://crypto.stackexchange.com/questions/59/taking-advantage-of-one-time-pad-key-reuse
    // a XOR b = f(a) XOR f(b) resp. a XOR key   b XOR key   =>   a XOR b = (a XOR key) XOR (b XOR key)
    // tedy XOR dvou sifrovanych textu (pri pouziti stejneho klice) je to same jako XOR ekvivalentnich puvodnich textu

    // Kdyz udelam XOR dvou sifer, dostanu XOR dvou plaintextu. Pokud v nem najdu velke pismeno tak jeden z textu
    // obsahuje odpovidajici male pismeno, ten druhy obsahuje mezeru

    std::array<std::array<std::string, 10>, 11> ciphered_text_2_xor;
    for (int i = 0; i < ciphered_text_2_bytes.size(); i++) {
        for (int j = 0; j < ciphered_text_2_bytes.size(); j++) {
            if (i == j)
                continue;
            ciphered_text_2_xor[i][j] = byte_vector_xor(ciphered_text_2_bytes[i], ciphered_text_2_bytes[j]);
        }
    }

    std::array<std::array<int, 122 / 2>, 11> is_space = {0};
    for (int i = 0; i < ciphered_text_2_xor.size(); i++) {
        for (int j = 0; j < ciphered_text_2_xor[i].size(); j++) {
            for (int k = 0; k < 122 / 2; k++) {
                if ((ciphered_text_2_xor[i][j][k] >= 65 && ciphered_text_2_xor[i][j][k] <= 90) || ciphered_text_2_xor[i][j][k] == 0)
                    is_space[i][k]++;
            }
        }
    }

    for (int i = 0; i < is_space.size(); i++) {
        for (int j = 0; j < is_space[i].size(); j++) {
            std::cout << "is_space[" << i << "][" << j << "] = " << is_space[i][j] << std::endl;
        }
    }

    // Zjisteni text 2: tri pismena mezera dve pismena mezera ctyri pismena mezera pet pismen mezera deset pismen
    // Zjisteni text 1: "?????? ?? ??? ?? ???? ??????..."
    // Zjisteni text 2: "??? ?? ???? ????? ??????????"
    // atd..

    std::array<std::string, 11> unknown_letters = {""};
    for (int i = 0; i < is_space.size(); i++) {
        std::cout << "Text " << i + 1 << ": ";
        for (int j : is_space[i]) {
            if (j >= 9)
                unknown_letters[i] += " ";
            else
                unknown_letters[i] += "?";
        }
        unknown_letters[i] = unknown_letters[i].substr(0, ciphered_text_2[i].length() / 2);
        std::cout << unknown_letters[i] << std::endl;
    }

    // Diky mezeram bych ted mel byt schopen desifrovat i nejake znaky...

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 122 / 2; j++) {
            if (unknown_letters[i][j] == ' ') {
                for (int k = 0; k < 11; k++) {
                    if (i == k)
                        continue;
                    for (int l = 0; l < 122 / 2; l++) {
                        if ((ciphered_text_2_xor[i][k][l] >= 65 && ciphered_text_2_xor[i][k][l] <= 90) && unknown_letters[k][l] == '?')
                            unknown_letters[k][l] = ciphered_text_2_xor[i][k][l] ^ 0x20;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 11; i++)
        std::cout << "Text " << i + 1 << ": " << unknown_letters[i] << std::endl;

    // (c1 XOR k) XOR p1 = k
    // ??sle?u? sve sny a z?en ?vuj zi?ot (text 9)
    // nasleduj sve sny a zmen svuj zivot (text 9 odhad)

    std::string partial_key = byte_vector_xor(ciphered_text_2_bytes[8], string_to_byte_vector("nasleduj sve sny a zmen svuj zivot"));
    std::cout << "Partial key: " << partial_key << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // ne vsichni kteri bloudi jsou ztrac_â╣ (text 7)
    // ne vsichni kteri bloudi jsou ztraceni (text 7 predpoklad)

    std::string partial_key_2 = byte_vector_xor(ciphered_text_2_bytes[6], string_to_byte_vector("ne vsichni kteri bloudi jsou ztraceni"));
    std::cout << "Partial key 2: " << partial_key_2 << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key_2)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // jestli najdes v zivote cestu bez prekŢ►źítÖŹ
    // jestli najdes v zivote cestu bez prekazek (predpoklad)

    std::string partial_key_3 = byte_vector_xor(ciphered_text_2_bytes[9], string_to_byte_vector("jestli najdes v zivote cestu bez prekazek"));
    std::cout << "Partial key 3: " << partial_key_3 << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key_3)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // za kazdou krasnou veci je nejaky druh bol+⌂&|
    // za kazdou krasnou veci je nejaky druh bolesti (predpoklad)

    std::string partial_key_4 = byte_vector_xor(ciphered_text_2_bytes[7], string_to_byte_vector("za kazdou krasnou veci je nejaky druh bolesti"));
    std::cout << "Partial key 4: " << partial_key_4 << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key_4)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // udelat vec ktere se bojime je prvni krok k usK3>G;
    // udelat vec ktere se bojime je prvni krok k uspechu (predpoklad)

    std::string partial_key_5 = byte_vector_xor(ciphered_text_2_bytes[4], string_to_byte_vector("udelat vec ktere se bojime je prvni krok k uspechu"));
    std::cout << "Partial key 5: " << partial_key_5 << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key_5)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // jednej tak abys byl stastny ne aby ses stastnym jeH▬ĺ→└,
    // jednej tak abys byl stastny ne aby ses stastnym jen zdal (predpoklad)

    std::string partial_key_6 = byte_vector_xor(ciphered_text_2_bytes[3], string_to_byte_vector("jednej tak abys byl stastny ne aby ses stastnym jen zdal"));
    std::cout << "Partial key 6: " << partial_key_6 << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(partial_key_6)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Text " << i + 1 << ": " << text << std::endl;
    }

    // jestli najdes v zivote cestu bez prekazek urcite nikam nŐňú■ů
    // jestli najdes v zivote cestu bez prekazek urcite nikam nevede :) hotovo

    std::string final_key = byte_vector_xor(ciphered_text_2_bytes[9], string_to_byte_vector("jestli najdes v zivote cestu bez prekazek urcite nikam nevede"));
    std::cout << "Final key: " << final_key << std::endl << "Final key in hex: ";
    for (unsigned char i : final_key) {
        std::cout << std::hex << (int) i;
    }
    std::cout << std::endl;

    for (int i = 0; i < 11; i++) {
        auto text = (byte_vector_xor(ciphered_text_2_bytes[i], string_to_byte_vector(final_key)));
        text = text.substr(0, ciphered_text_2[i].length() / 2);
        std::cout << "Deciphered text " << i + 1 << ": " << text << std::endl;
    }

    // Rozlusteny text 11 je:
    // kdyz nevis kam plujes zadny vitr neni priznivy

    return 0;
}