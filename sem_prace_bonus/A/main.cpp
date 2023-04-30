#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>

std::array<int, 26> analyze_frequency(const std::string &text, int start = 0, int skip = 1) {
    std::array<int, 26> frequency = {0};
    int istart = 0;
    int iskip = 0;

    for (auto &c : text) {
        if (istart < start) {
            istart++;
            continue;
        }
        if (c == ' ')
            continue;
        if (iskip % skip == 0) {
            frequency[c - 'a']++;
        }
        iskip++;
    }

    return frequency;
}

std::array<float, 26> analyze_frequency_relative(const std::string &text, int start = 0, int skip = 1) {
    std::array<float, 26> frequency = {0};

    int istart = 0;
    int iskip = 0;

    for (auto &c : text) {
        if (istart < start) {
            istart++;
            continue;
        }
        if (c == ' ')
            continue;
        if (iskip % skip == 0) {
            frequency[c - 'a']++;
        }
        iskip++;
    }

    for (auto &f : frequency)
        f /= text.size();

    return frequency;
}

std::vector<std::pair<std::string, float>> analyze_bigrams_relative(const std::string &text) {
    std::vector<std::pair<std::string, float>> bigrams;

    for (int i = 0; i < text.size() - 1; i++) {
        if (text[i] == ' ' || text[i + 1] == ' ')
            continue;
        std::string bigram = text.substr(i, 2);
        bool found = false;
        for (auto &b : bigrams) {
            if (b.first == bigram) {
                b.second++;
                found = true;
                break;
            }
        }
        if (!found) {
            bigrams.emplace_back(bigram, 1);
        }
    }

    for (auto &b : bigrams)
        b.second /= text.size();

    return bigrams;
}

std::vector<std::pair<std::string, float>> analyze_trigrams_relative(const std::string &text) {
    std::vector<std::pair<std::string, float>> trigrams;

    for (int i = 0; i < text.size() - 2; i++) {
        if (text[i] == ' ' || text[i + 1] == ' ' || text[i + 2] == ' ')
            continue;
        std::string trigram = text.substr(i, 3);
        bool found = false;
        for (auto &t : trigrams) {
            if (t.first == trigram) {
                t.second++;
                found = true;
                break;
            }
        }
        if (!found) {
            trigrams.emplace_back(trigram, 1);
        }
    }

    for (auto &t : trigrams)
        t.second /= text.size();

    return trigrams;
}

std::vector<int> chars_between_words(const std::string &text, const std::string &word) {
    std::vector<int> temp;

    for (int i = 0; i < text.size(); i++) {
        if (text.substr(i, word.size()) == word) {
            temp.push_back(i);
        }
    }

    std::vector<int> chars_between;
    for (int i = 0; i < temp.size() - 1; i++) {
        chars_between.push_back(temp[i + 1] - temp[i]);
    }

    return chars_between;
}

std::vector<std::string> find_repeating_words(const std::string &text, int min_length = 3, int max_length = 10) {
    std::vector<std::string> repeating_words;

    for (int len = min_length; len < max_length; len++) {
        for (int i = 0; i < text.size() - len; i++) {
            std::string word = text.substr(i, len);
            for (int j = 0; j < text.size() - len; j++) {
                if (i == j)
                    continue;
                if (word == text.substr(j, len))
                    repeating_words.push_back(word);
            }
        }
    }

    return repeating_words;
}

std::vector<int> analyze_repeating_word(const std::string &text, const std::string &word, bool verbose = false) {
    std::vector<int> chars_between = chars_between_words(text, word);

    if (verbose) {
        std::cout << word << ": ";
        for (auto &c : chars_between)
            std::cout << c << " ";
    }
    auto min = std::min_element(chars_between.begin(), chars_between.end());
    if (verbose)
        std::cout << "  |   min: " << *min;

    std::vector<int> factors;
    for (int i = 2; i <= *min; i++) {
        if (*min % i == 0)
            factors.push_back(i);
    }

    if (verbose) {
        std::cout << "  |   factors: ";
        for (auto &f : factors)
            std::cout << f << " ";
        std::cout << std::endl;
    }

    return factors;
}

std::string substitute_alphabet_decipher(std::string cipher_text, const std::string &alphabet1) {
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string plain_text = cipher_text;

    for (auto &c : plain_text) {
        if (c == ' ')
            continue;
        c = alphabet[alphabet1.find(c)];
    }

    return plain_text;
}

int main() {
    std::string ciphered_text_1 = "sq lmdqrcej mcrsdl imc ipl hlhprrv wdqv rpzd ft zmd scqtftyl lpwd hkct zmcld tcz ftgqdnhdtz ceeplfctl imdt md ipl hk prr tfymz ipl ldpzda pz zmd uqdpjgplz zpurd f lzcca hkct zmd mdpqzmqhy pta kfejda hk zmd lzfej imfem chq wflfzcq mpa rdgz udmfta mfs zmd tfymz udgcqd fz ipl p gftd zmfej kfded cg icca uhruchlmdpada cg zmd lcqz imfem fl jtcit pl p kdtpty rpivdq xhlz htadq zmd mdpa ipl p uqcpa lfrwdq upta tdpqrv pt ftem peqcll zc xpsdl scqzfsdq sqel gqcs mfl gqfdtal cg zmd eem ipl dtyqpwda hkct fz ifzm zmd apzd fz ipl xhlz lhem p lzfej pl zmd cragplmfctda gpsfrv kqpezfzfctdq hlda zc epqqvafytfgfda lcrfa pta qdpllhqfty idrr ipzlct impz ac vch spjd cg fz mcrsdl ipl lfzzfty ifzm mfl upej zc sd pta f mpa yfwdt mfs tc lfyt cg sv ceehkpzfct mci afa vch jtci impz f ipl acfty f udrfdwd vch mpwd dvdl ft zmd upej cg vchq mdpa f mpwd pz rdplz p idrrkcrflmda lfrwdqkrpzda ecggddkcz ft gqctz cg sd lpfa md uhz zdrr sd ipzlct impz ac vch spjd cg chq wflfzcql lzfej lfted id mpwd uddt lc htgcqzhtpzd pl zc sfll mfs pta mpwd tc tczfct cg mfl dqqpta zmfl peefadtzpr lchwdtfq udecsdl cg fskcqzpted rdz sd mdpq vch qdectlzqhez zmd spt uv pt dopsftpzfct cg fz f zmftj lpfa f gcrrcifty pl gpq pl f echra zmd sdzmcal cg sv ecskptfct zmpz aq scqzfsdq fl p lheedllghr dradqrv sdafepr spt idrrdlzddsda lfted zmcld imc jtci mfs yfwd mfs zmfl spqj cg zmdfq pkkqdefpzfct ycca lpfa mcrsdl doedrrdtz f zmftj prlc zmpz zmd kqcupufrfzv fl ft gpwchq cg mfl udfty p echtzqv kqpezfzfctdq imc acdl p yqdpz adpr cg mfl wflfzfty ct gccz imv lc udephld zmfl lzfej zmchym cqfyftprrv p wdqv mptalcsd ctd mpl uddt lc jtcejda puchz zmpz f ept mpqarv fspyftd p zcit kqpezfzfctdq epqqvfty fz zmd zmfejfqct gdqqhrd fl icqt acit lc fz fl dwfadtz zmpz md mpl actd p yqdpz pschtz cg iprjfty ifzm fz kdqgdezrv lchta lpfa mcrsdl pta zmdt pypft zmdqd fl zmd gqfdtal cg zmd eem f lmchra yhdll zmpz zc ud zmd lcsdzmfty mhtz zmd rcepr mhtz zc imcld sdsudql md mpl kcllfurv yfwdt lcsd lhqyfepr pllflzpted pta imfem mpl spad mfs p lsprr kqdldtzpzfct ft qdzhqt qdprrv ipzlct vch doedr vchqldrg lpfa mcrsdl khlmfty upej mfl empfq pta rfymzfty p efypqdzzd f ps uchta zc lpv zmpz ft prr zmd peechtzl imfem vch mpwd uddt lc ycca pl zc yfwd cg sv cit lsprr pemfdwdsdtzl vch mpwd mpufzhprrv htadqqpzda vchq cit pufrfzfdl fz spv ud zmpz vch pqd tcz vchqldrg rhsftchl uhz vch pqd p ectahezcq cg rfymz lcsd kdckrd ifzmchz kclldllfty ydtfhl mpwd p qdspqjpurd kcidq cg lzfshrpzfty fz f ectgdll sv adpq gdrrci zmpz f ps wdqv shem ft vchq aduz md mpa tdwdq lpfa pl shem udgcqd pta f shlz pasfz zmpz mfl icqal ypwd sd jddt krdplhqd gcq f mpa cgzdt uddt kfnhda uv mfl ftafggdqdted zc sv pasfqpzfct pta zc zmd pzzdskzl imfem f mpa spad zc yfwd khurfefzv zc mfl sdzmcal f ipl kqcha zcc zc zmftj zmpz f mpa lc gpq splzdqda mfl lvlzds pl zc pkkrv fz ft p ipv imfem dpqtda mfl pkkqcwpr md tci zccj zmd lzfej gqcs sv mptal pta dopsftda fz gcq p gdi sfthzdl ifzm mfl tpjda dvdl zmdt ifzm pt dokqdllfct cg ftzdqdlz md rpfa acit mfl efypqdzzd pta epqqvfty zmd eptd zc zmd iftaci md rccjda cwdq fz pypft ifzm p ectwdo rdtl ftzdqdlzfty zmchym drdsdtzpqv lpfa md pl md qdzhqtda zc mfl gpwchqfzd ecqtdq cg zmd ldzzdd zmdqd pqd edqzpftrv ctd cq zic ftafepzfctl hkct zmd lzfej fz yfwdl hl zmd uplfl gcq ldwdqpr adahezfctl mpl ptvzmfty dlepkda sd f pljda ifzm lcsd ldrgfskcqzpted f zqhlz zmpz zmdqd fl tczmfty cg ectldnhdted imfem f mpwd cwdqrccjda f ps pgqpfa sv adpq ipzlct zmpz sclz cg vchq ecterhlfctl idqd dqqctdchl imdt f lpfa zmpz vch lzfshrpzda sd f sdptz zc ud gqptj zmpz ft tczfty vchq gprrpefdl f ipl ceeplfctprrv yhfada zcipqal zmd zqhzm tcz zmpz vch pqd dtzfqdrv iqcty ft zmfl ftlzpted zmd spt fl edqzpftrv p echtzqv kqpezfzfctdq pta md iprjl p ycca adpr zmdt f ipl qfymz zc zmpz dozdtz uhz zmpz ipl prr tc tc sv adpq ipzlct tcz prruv tc sdptl prr f ichra lhyydlz gcq dopskrd zmpz p kqdldtzpzfct zc p acezcq fl scqd rfjdrv zc ecsd gqcs p mclkfzpr zmpt gqcs p mhtz pta zmpz imdt zmd ftfzfprl ee pqd krpeda udgcqd zmpz mclkfzpr zmd icqal empqfty eqcll wdqv tpzhqprrv lhyydlz zmdsldrwdl vch spv ud qfymz zmd kqcupufrfzv rfdl ft zmpz afqdezfct pta fg id zpjd zmfl pl p icqjfty mvkczmdlfl id mpwd p gqdlm uplfl gqcs imfem zc lzpqz chq ectlzqhezfct cg zmfl htjtcit wflfzcq idrr zmdt lhkkclfty zmpz eem acdl lzpta gcq empqfty eqcll mclkfzpr impz ghqzmdq ftgdqdtedl spv id aqpi ac tctd lhyydlz zmdsldrwdl vch jtci sv sdzmcal pkkrv zmds f ept ctrv zmftj cg zmd cuwfchl ecterhlfct zmpz zmd spt mpl kqpezflda ft zcit udgcqd ycfty zc zmd echtzqv f zmftj zmpz id sfymz wdtzhqd p rfzzrd gpqzmdq zmpt zmfl rccj pz fz ft zmfl rfymz ct impz ceeplfct ichra fz ud sclz kqcupurd zmpz lhem p kqdldtzpzfct ichra ud spad imdt ichra mfl gqfdtal htfzd zc yfwd mfs p krdayd cg zmdfq ycca ifrr cuwfchlrv pz zmd scsdtz imdt aq scqzfsdq ifzmaqdi gqcs zmd ldqwfed cg zmd mclkfzpr ft cqadq zc lzpqz ft kqpezfed gcq mfsldrg id jtci zmdqd mpl uddt p kqdldtzpzfct id udrfdwd zmdqd mpl uddt p emptyd gqcs p zcit mclkfzpr zc p echtzqv kqpezfed fl fz zmdt lzqdzemfty chq ftgdqdted zcc gpq zc lpv zmpz zmd kqdldtzpzfct ipl ct zmd ceeplfct cg zmd emptyd fz edqzpftrv lddsl kqcupurd tci vch ifrr culdqwd zmpz md echra tcz mpwd uddt ct zmd lzpgg cg zmd mclkfzpr lfted ctrv p spt idrrdlzpurflmda ft p rctact kqpezfed echra mcra lhem p kclfzfct pta lhem p ctd ichra tcz aqfgz ftzc zmd echtzqv impz ipl md zmdt fg md ipl ft zmd mclkfzpr pta vdz tcz ct zmd lzpgg md echra ctrv mpwd uddt p mchldlhqydct cq p mchldkmvlfefptrfzzrd scqd zmpt p ldtfcq lzhadtz pta md rdgz gfwd vdpql pyczmd apzd fl ct zmd lzfej lc vchq yqpwd sfaardpyda gpsfrv kqpezfzfctdq wptflmdl ftzc zmft pfq sv adpq ipzlct pta zmdqd dsdqydl p vchty gdrrci htadq zmfqzv psfpurd htpsufzfchl puldtzsftada pta zmd kclldllcq cg p gpwchqfzd acy imfem f lmchra adleqfud qchymrv pl udfty rpqydq zmpt p zdqqfdq pta lsprrdq zmpt p splzfgg f rphymda fteqdahrchlrv pl lmdqrcej mcrsdl rdptda upej ft mfl ldzzdd pta urdi rfzzrd ipwdqfty qftyl cg lscjd hk zc zmd edfrfty pl zc zmd rpzzdq kpqz f mpwd tc sdptl cg emdejfty vch lpfa f uhz pz rdplz fz fl tcz afggfehrz zc gfta chz p gdi kpqzfehrpql puchz zmd sptl pyd pta kqcgdllfctpr epqddq gqcs sv lsprr sdafepr lmdrg f zccj acit zmd sdafepr afqdezcqv pta zhqtda hk zmd tpsd zmdqd idqd ldwdqpr scqzfsdql uhz ctrv ctd imc echra ud chq wflfzcq f qdpa mfl qdecqa prcha scqzfsdq xpsdl sqel yqfskdt apqzsccq adwct mchldlhqydct gqcs zc pz empqfty eqcll mclkfzpr ifttdq cg zmd xpejlct kqfbd gcq ecskpqpzfwd kpzmcrcyv ifzm dllpv dtzfzrda fl afldpld p qdwdqlfct ecqqdlkctafty sdsudq cg zmd lidaflm kpzmcrcyfepr lcefdzv phzmcq cg lcsd gqdpjl cg pzpwfls rptedz ac id kqcyqdll xchqtpr cg klvemcrcyv spqem sdafepr cggfedq gcq zmd kpqflmdl cg yqfskdt zmcqlrdv pta mfym upqqci tc sdtzfct cg zmpz rcepr mhtz ipzlct lpfa mcrsdl ifzm p sflemfdwchl lsfrd uhz p echtzqv acezcq pl vch wdqv plzhzdrv culdqwda f zmftj zmpz f ps gpfqrv xhlzfgfda ft sv ftgdqdtedl pl zc zmd paxdezfwdl f lpfa fg f qdsdsudq qfymz psfpurd htpsufzfchl pta puldtzsftada fz fl sv dokdqfdted zmpz fz fl ctrv pt psfpurd spt ft zmfl icqra imc qdedfwdl zdlzfsctfprl ctrv pt htpsufzfchl ctd imc puptactl p rctact epqddq gcq zmd echtzqv pta ctrv pt puldtzsftada ctd imc rdpwdl mfl lzfej pta tcz mfl wflfzftyepqa pgzdq ipfzfty pt mchq ft vchq qccs pta zmd acy mpl uddt ft zmd mpufz cg epqqvfty zmfl lzfej udmfta mfl splzdq udfty p mdpwv lzfej zmd acy mpl mdra fz zfymzrv uv zmd sfaard pta zmd spqjl cg mfl zddzm pqd wdqv krpftrv wflfurd zmd acyl xpi pl lmcit ft zmd lkped udziddt zmdld spqjl fl zcc uqcpa ft sv ckftfct gcq p zdqqfdq pta tcz uqcpa dtchym gcq p splzfgg fz spv mpwd uddtvdl uv xcwd fz fl p ehqrvmpfqda lkptfdr md mpa qfldt pta kpeda zmd qccs pl md lkcjd tci md mprzda ft zmd qdedll cg zmd iftaci zmdqd ipl lhem p qfty cg ectwfezfct ft mfl wcfed zmpz f yrpteda hk ft lhqkqfld sv adpq gdrrci mci ept vch kcllfurv ud lc lhqd cg zmpz gcq zmd wdqv lfskrd qdplct zmpz f ldd zmd acy mfsldrg ct chq wdqv accqlzdk pta zmdqd fl zmd qfty cg fzl citdq actz scwd f udy vch ipzlct md fl p kqcgdllfctpr uqczmdq cg vchql pta vchq kqdldted spv ud cg pllflzpted zc sd tci fl zmd aqpspzfe scsdtz cg gpzd ipzlct imdt vch mdpq p lzdk hkct zmd lzpfq imfem fl iprjfty ftzc vchq rfgd pta vch jtci tcz imdzmdq gcq ycca cq frr impz acdl aq xpsdl scqzfsdq zmd spt cg lefdted plj cg lmdqrcej mcrsdl zmd lkdefprflz ft eqfsd ecsd ft zmd pkkdpqpted cg chq wflfzcq ipl p lhqkqfld zc sd lfted f mpa dokdezda p zvkfepr echtzqv kqpezfzfctdq md ipl p wdqv zprr zmft spt ifzm p rcty tcld rfjd p udpj imfem xhzzda chz udziddt zic jddt yqpv dvdl ldz ercldrv zcydzmdq pta lkpqjrfty uqfymzrv gqcs udmfta p kpfq cg ycraqfssda yrplldl md ipl erpa ft p kqcgdllfctpr uhz qpzmdq lrcwdtrv gplmfct gcq mfl gqcejecpz ipl aftyv pta mfl zqchldql gqpvda zmchym vchty mfl rcty upej ipl prqdpav ucida pta md iprjda ifzm p gcqipqa zmqhlz cg mfl mdpa pta p ydtdqpr pfq cg kddqfty udtdwcrdted pl md dtzdqda mfl dvdl gdrr hkct zmd lzfej ft mcrsdll mpta pta md qpt zcipqal fz ifzm pt doerpspzfct cg xcv f ps lc wdqv yrpa lpfa md f ipl tcz lhqd imdzmdq f mpa rdgz fz mdqd cq ft zmd lmfkkfty cggfed f ichra tcz rcld zmpz lzfej gcq zmd icqra p kqdldtzpzfct f ldd lpfa mcrsdl vdl lfq gqcs empqfty eqcll mclkfzpr gqcs ctd cq zic gqfdtal zmdqd ct zmd ceeplfct cg sv spqqfpyd adpq adpq zmpzl upa lpfa mcrsdl lmpjfty mfl mdpa aq scqzfsdq urftjda zmqchym mfl yrplldl ft sfra plzctflmsdtz imv ipl fz upa ctrv zmpz vch mpwd aflpqqptyda chq rfzzrd adahezfctl vchq spqqfpyd vch lpv vdl lfq f spqqfda pta lc rdgz zmd mclkfzpr pta ifzm fz prr mckdl cg p ectlhrzfty kqpezfed fz ipl tdedllpqv zc spjd p mcsd cg sv cit ecsd ecsd id pqd tcz lc gpq iqcty pgzdq prr lpfa mcrsdl pta tci aq xpsdl scqzfsdq sflzdq lfq sflzdqp mhsurd sqel pta p spt cg kqdefld sfta dwfadtzrv p apuurdq ft lefdted sq mcrsdl p kfejdq hk cg lmdrrl ct zmd lmcqdl cg zmd yqdpz htjtcit cedpt f kqdlhsd zmpz fz fl sq lmdqrcej mcrsdl imcs f ps paaqdllfty pta tcz tc zmfl fl sv gqfdta aq ipzlct yrpa zc sddz vch lfq f mpwd mdpqa vchq tpsd sdtzfctda ft ecttdezfct ifzm zmpz cg vchq gqfdta vch ftzdqdlz sd wdqv shem sq mcrsdl f mpa mpqarv dokdezda lc acrfemcedkmprfe p ljhrr cq lhem idrrspqjda lhkqpcqufzpr adwdrcksdtz ichra vch mpwd ptv cuxdezfct zc sv qhttfty sv gftydq prcty vchq kpqfdzpr gfllhqd p eplz cg vchq ljhrr lfq htzfr zmd cqfyftpr fl pwpfrpurd ichra ud pt cqtpsdtz zc ptv ptzmqckcrcyfepr shldhs fz fl tcz sv ftzdtzfct zc ud ghrlcsd uhz f ectgdll zmpz f ecwdz vchq ljhrr lmdqrcej mcrsdl ipwda chq lzqptyd wflfzcq ftzc p empfq vch pqd pt dtzmhlfplz ft vchq rftd cg zmchymz f kdqedfwd lfq pl f ps ft sftd lpfa md f culdqwd gqcs vchq gcqdgftydq zmpz vch spjd vchq cit efypqdzzdl mpwd tc mdlfzpzfct ft rfymzfty ctd zmd spt aqdi chz kpkdq pta zcupeec pta zifqrda zmd ctd hk ft zmd czmdq ifzm lhqkqflfty adozdqfzv md mpa rcty nhfwdqfty gftydql pl pyfrd pta qdlzrdll pl zmd ptzdttpd cg pt ftldez mcrsdl ipl lfrdtz uhz mfl rfzzrd apqzfty yrptedl lmcida sd zmd ftzdqdlz imfem md zccj ft chq ehqfchl ecskptfct f kqdlhsd lfq lpfa md pz rplz zmpz fz ipl tcz sdqdrv gcq zmd khqkcld cg dopsftfty sv ljhrr zmpz vch mpwd actd sd zmd mctchq zc eprr mdqd rplz tfymz pta pypft zcapv tc lfq tc zmchym f ps mpkkv zc mpwd mpa zmd ckkcqzhtfzv cg acfty zmpz pl idrr f epsd zc vch sq mcrsdl udephld f qdecytfbda zmpz f ps svldrg pt htkqpezfepr spt pta udephld f ps lhaadtrv ectgqctzda ifzm p sclz ldqfchl pta dozqpcqaftpqv kqcurds qdecytfbfty pl f ac zmpz vch pqd zmd ldecta mfymdlz dokdqz ft dhqckd ftadda lfq spv f ftnhfqd imc mpl zmd mctchq zc ud zmd gfqlz pljda mcrsdl ifzm lcsd plkdqfzv zc zmd spt cg kqdefldrv lefdtzfgfe sfta zmd icqj cg sctlfdhq udqzfrrct shlz pripvl pkkdpr lzqctyrv zmdt mpa vch tcz udzzdq ectlhrz mfs f lpfa lfq zc zmd kqdefldrv lefdtzfgfe sfta uhz pl p kqpezfepr spt cg pggpfql fz fl pejtcirdayda zmpz vch lzpta prctd f zqhlz lfq zmpz f mpwd tcz ftpawdqzdtzrv xhlz p rfzzrd lpfa mcrsdl f zmftj aq scqzfsdq vch ichra ac ifldrv fg ifzmchz scqd pac vch ichra jftarv zdrr sd krpftrv impz zmd dopez tpzhqd cg zmd kqcurds fl ft imfem vch adspta sv pllflzpted f mpwd ft sv kcejdz p spthleqfkz lpfa aq xpsdl scqzfsdq f culdqwda fz pl vch dtzdqda zmd qccs lpfa mcrsdl fz fl pt cra spthleqfkz dpqrv dfymzddtzm edtzhqv htrdll fz fl p gcqydqv mci ept vch lpv zmpz lfq vch mpwd kqdldtzda pt ftem cq zic cg fz zc sv dopsftpzfct prr zmd zfsd zmpz vch mpwd uddt zprjfty fz ichra ud p kccq dokdqz imc echra tcz yfwd zmd apzd cg p acehsdtz ifzmft p adepad cq lc vch spv kcllfurv mpwd qdpa sv rfzzrd sctcyqpkm hkct zmd lhuxdez f khz zmpz pz zmd dopez apzd fl aq scqzfsdq aqdi fz gqcs mfl uqdplzkcejdz zmfl gpsfrv kpkdq ipl ecssfzzda zc sv epqd uv lfq empqrdl upljdqwfrrd imcld lhaadt pta zqpyfe adpzm lcsd zmqdd sctzml pyc eqdpzda lc shem doefzdsdtz ft adwctlmfqd f spv lpv zmpz f ipl mfl kdqlctpr gqfdta pl idrr pl mfl sdafepr pzzdtaptz md ipl p lzqctysftada spt lfq lmqdia kqpezfepr pta pl htfspyftpzfwd pl f ps svldrg vdz md zccj zmfl acehsdtz wdqv ldqfchlrv pta mfl sfta ipl kqdkpqda gcq xhlz lhem pt dta pl afa dwdtzhprrv cwdqzpjd mfs mcrsdl lzqdzemda chz mfl mpta gcq zmd spthleqfkz pta grpzzdtda fz hkct mfl jtdd vch ifrr culdqwd ipzlct zmd przdqtpzfwd hld cg zmd rcty l pta zmd lmcqz fz fl ctd cg ldwdqpr ftafepzfctl imfem dtpurda sd zc gfo zmd apzd f rccjda cwdq mfl lmchradq pz zmd vdrrci kpkdq pta zmd gpada leqfkz pz zmd mdpa ipl iqfzzdt upljdqwfrrd mprr pta udrci ft rpqyd leqpirfty gfyhqdl fz pkkdpql zc ud p lzpzdsdtz cg lcsd lcqz vdl fz fl p lzpzdsdtz cg p edqzpft rdydta imfem qhtl ft zmd upljdqwfrrd gpsfrv uhz f htadqlzpta zmpz fz fl lcsdzmfty scqd scadqt pta kqpezfepr hkct imfem vch iflm zc ectlhrz sd sclz scadqt p sclz kqpezfepr kqdllfty spzzdq imfem shlz ud adefada ifzmft zidtzvgchq mchql uhz zmd spthleqfkz fl lmcqz pta fl ftzfspzdrv ecttdezda ifzm zmd pggpfq ifzm vchq kdqsfllfct f ifrr qdpa fz zc vch mcrsdl rdptda upej ft mfl empfq krpeda mfl gftydqzfkl zcydzmdq pta erclda mfl dvdl ifzm pt pfq cg qdlfytpzfct aq scqzfsdq zhqtda zmd spthleqfkz zc zmd rfymz pta qdpa ft p mfym eqpejfty wcfed zmd gcrrcifty ehqfchl craicqra tpqqpzfwd cg zmd cqfyft cg zmd mchta cg zmd upljdqwfrrdl zmdqd mpwd uddt sptv lzpzdsdtzl vdz pl f ecsd ft p afqdez rftd gqcs mhyc upljdqwfrrd pta pl f mpa zmd lzcqv gqcs sv gpzmdq imc prlc mpa fz gqcs mfl f mpwd ldz fz acit ifzm prr udrfdg zmpz fz ceehqqda dwdt pl fl mdqd ldz gcqzm pta f ichra mpwd vch udrfdwd sv lctl zmpz zmd lpsd xhlzfed imfem khtflmdl lft spv prlc sclz yqpefchlrv gcqyfwd fz pta zmpz tc upt fl lc mdpwv uhz zmpz uv kqpvdq pta qdkdtzpted fz spv ud qdscwda rdpqt zmdt gqcs zmfl lzcqv tcz zc gdpq zmd gqhfzl cg zmd kplz uhz qpzmdq zc ud efqehslkdez ft zmd ghzhqd zmpz zmcld gchr kpllfctl imdqduv chq gpsfrv mpl lhggdqda lc yqfdwchlrv spv tcz pypft ud rcclda zc chq htacfty jtci zmdt zmpz ft zmd zfsd cg zmd yqdpz qdudrrfct zmd mflzcqv cg imfem uv zmd rdpqtda rcqa erpqdtact f sclz dpqtdlzrv ecssdta zc vchq pzzdtzfct zmfl sptcq cg upljdqwfrrd ipl mdra uv mhyc cg zmpz tpsd tcq ept fz ud ypftlpfa zmpz md ipl p sclz ifra kqcgptd pta ycardll spt zmfl ft zqhzm mfl tdfymuchql sfymz mpwd kpqactda lddfty zmpz lpftzl mpwd tdwdq grchqflmda ft zmcld kpqzl uhz zmdqd ipl ft mfs p edqzpft iptzct pta eqhdr mhschq imfem spad mfl tpsd p uvicqa zmqchym zmd idlz fz empteda zmpz zmfl mhyc epsd zc rcwd fg ftadda lc apqj p kpllfct spv ud jtcit htadq lc uqfymz p tpsd zmd aphymzdq cg p vdcspt imc mdra rptal tdpq zmd upljdqwfrrd dlzpzd uhz zmd vchty spfadt udfty afleqddz pta cg ycca qdkhzd ichra dwdq pwcfa mfs gcq lmd gdpqda mfl dwfr tpsd lc fz epsd zc kpll zmpz ctd sfempdrspl zmfl mhyc ifzm gfwd cq lfo cg mfl fard pta ifejda ecskptfctl lzcrd acit hkct zmd gpqs pta epqqfda cgg zmd spfadt mdq gpzmdq pta uqczmdql udfty gqcs mcsd pl md idrr jtdi imdt zmdv mpa uqchymz mdq zc zmd mprr zmd spfadt ipl krpeda ft pt hkkdq empsudq imfrd mhyc pta mfl gqfdtal lpz acit zc p rcty epqchld pl ipl zmdfq tfymzrv ehlzcs tci zmd kccq rpll hklzpfql ipl rfjd zc mpwd mdq ifzl zhqtda pz zmd lftyfty pta lmchzfty pta zdqqfurd cpzml imfem epsd hk zc mdq gqcs udrci gcq zmdv lpv zmpz zmd icqal hlda uv mhyc upljdqwfrrd imdt md ipl ft iftd idqd lhem pl sfymz urplz zmd spt imc lpfa zmds pz rplz ft zmd lzqdll cg mdq gdpq lmd afa zmpz imfem sfymz mpwd aphtzda zmd uqpwdlz cq sclz pezfwd spt gcq uv zmd pfa cg zmd yqcizm cg fwv imfem ecwdqda pta lzfrr ecwdql zmd lchzm iprr lmd epsd acit gqcs htadq zmd dpwdl pta lc mcsdipqa peqcll zmd sccq zmdqd udfty zmqdd rdpyhdl udzifoz zmd mprr pta mdq gpzmdql gpqs fz empteda zmpz lcsd rfzzrd zfsd rpzdq mhyc rdgz mfl yhdlzl zc epqqv gcca pta aqftjifzm czmdq icqld zmftyl kdqemptedzc mfl epkzfwd pta lc gchta zmd epyd dskzv pta zmd ufqa dlepkda zmdt pl fz ichra ldds md udepsd pl ctd zmpz mpzm p adwfr gcq qhlmfty acit zmd lzpfql ftzc zmd aftftymprr md lkqpty hkct zmd yqdpz zpurd grpyctl pta zqdtemdql grvfty udgcqd mfs pta md eqfda prcha udgcqd prr zmd ecskptv zmpz md ichra zmpz wdqv tfymz qdtadq mfl ucav pta lchr zc zmd kcidql cg dwfr fg md sfymz uhz cwdqzpjd zmd idtem pta imfrd zmd qdwdrrdql lzcca pymplz pz zmd ghqv cg zmd spt ctd scqd ifejda cq fz spv ud scqd aqhtjdt zmpt zmd qdlz eqfda chz zmpz zmdv lmchra khz zmd mchtal hkct mdq imdqdpz mhyc qpt gqcs zmd mchld eqvfty zc mfl yqccsl zmpz zmdv lmchra lpaard mfl spqd pta htjdttdr zmd kpej pta yfwfty zmd mchtal p jdqemfdg cg zmd spfal md lihty zmds zc zmd rftd pta lc cgg ghrr eqv ft zmd scctrfymz cwdq zmd sccq tci gcq lcsd lkped zmd qdwdrrdql lzcca pypkd htpurd zc htadqlzpta prr zmpz mpa uddt actd ft lhem mplzd uhz ptct zmdfq udshlda ifzl picjd zc zmd tpzhqd cg zmd adda imfem ipl rfjd zc ud actd hkct zmd sccqrptal dwdqvzmfty ipl tci ft pt hkqcpq lcsd eprrfty gcq zmdfq kflzcrl lcsd gcq zmdfq mcqldl pta lcsd gcq ptczmdq grplj cg iftd uhz pz rdtyzm lcsd ldtld epsd upej zc zmdfq eqpbda sftal pta zmd imcrd cg zmds zmfqzddt ft thsudq zccj mcqld pta lzpqzda ft khqlhfz zmd scct lmctd erdpq pucwd zmds pta zmdv qcad lifgzrv puqdplz zpjfty zmpz echqld imfem zmd spfa shlz tddal mpwd zpjdt fg lmd idqd zc qdpem mdq cit mcsd zmdv mpa yctd p sfrd cq zic imdt zmdv kpllda ctd cg zmd tfymz lmdkmdqal hkct zmd sccqrptal pta zmdv eqfda zc mfs zc jtci fg md mpa lddt zmd mhtz pta zmd spt pl zmd lzcqv ycdl ipl lc eqpbda ifzm gdpq zmpz md echra lepqed lkdpj uhz pz rplz md lpfa zmpz md mpa ftadda lddt zmd htmpkkv spfadt ifzm zmd mchtal hkct mdq zqpej uhz f mpwd lddt scqd zmpt zmpz lpfa md gcq mhyc upljdqwfrrd kpllda sd hkct mfl urpej spqd pta zmdqd qpt shzd udmfta mfs lhem p mchta cg mdrr pl yca gcqufa lmchra dwdq ud pz sv mddrl lc zmd aqhtjdt lnhfqdl ehqlda zmd lmdkmdqa pta qcad ctipqa uhz lcct zmdfq ljftl zhqtda ecra gcq zmdqd epsd p yprrckfty peqcll zmd sccq pta zmd urpej spqd apuurda ifzm imfzd gqczm idtz kplz ifzm zqpfrfty uqfard pta dskzv lpaard zmdt zmd qdwdrrdql qcad ercld zcydzmdq gcq p yqdpz gdpq ipl ct zmds uhz zmdv lzfrr gcrrcida cwdq zmd sccq zmchym dpem mpa md uddt prctd ichra mpwd uddt qfymz yrpa zc mpwd zhqtda mfl mcqldl mdpa qfafty lrcirv ft zmfl gplmfct zmdv epsd pz rplz hkct zmd mchtal zmdld zmchym jtcit gcq zmdfq wprchq pta zmdfq uqdda idqd imfskdqfty ft p erhlzdq pz zmd mdpa cg p addk afk cq ycvpr pl id eprr fz hkct zmd sccq lcsd lrftjfty pipv pta lcsd ifzm lzpqzfty mpejrdl pta lzpqfty dvdl ypbfty acit zmd tpqqci wprrdv udgcqd zmds zmd ecskptv mpa ecsd zc p mprz scqd lcudq sdt pl vch spv yhdll zmpt imdt zmdv lzpqzda zmd sclz cg zmds ichra uv tc sdptl pawpted uhz zmqdd cg zmds zmd ucradlz cq fz spv ud zmd sclz aqhtjdt qcad gcqipqa acit zmd ycvpr tci fz ckdtda ftzc p uqcpa lkped ft imfem lzcca zic cg zmcld yqdpz lzctdl lzfrr zc ud lddt zmdqd imfem idqd ldz uv edqzpft gcqyczzdt kdckrdl ft zmd apvl cg cra zmd scct ipl lmftfty uqfymz hkct zmd erdpqfty pta zmdqd ft zmd edtzqd rpv zmd htmpkkv spfa imdqd lmd mpa gprrdt adpa cg gdpq pta cg gpzfyhd uhz fz ipl tcz zmd lfymz cg mdq ucav tcq vdz ipl fz zmpz cg zmd ucav cg mhyc upljdqwfrrd rvfty tdpq mdq imfem qpflda zmd mpfq hkct zmd mdpal cg zmdld zmqdd apqdadwfr qcvlzdqdql uhz fz ipl zmpz lzptafty cwdq mhyc pta krhejfty pz mfl zmqcpz zmdqd lzcca p gchr zmfty p yqdpz urpej udplz lmpkda rfjd p mchta vdz rpqydq zmpt ptv mchta zmpz dwdq scqzpr dvd mpl qdlzda hkct pta dwdt pl zmdv rccjda zmd zmfty zcqd zmd zmqcpz chz cg mhyc upljdqwfrrd ct imfem pl fz zhqtda fzl urpbfty dvdl pta aqfkkfty xpil hkct zmds zmd zmqdd lmqfdjda ifzm gdpq pta qcad gcq adpq rfgd lzfrr leqdpsfty peqcll zmd sccq ctd fz fl lpfa afda zmpz wdqv tfymz cg impz md mpa lddt pta zmd czmdq zipft idqd uhz uqcjdt sdt gcq zmd qdlz cg zmdfq apvl lhem fl zmd zprd sv lctl cg zmd ecsfty cg zmd mchta imfem fl lpfa zc mpwd krpyhda zmd gpsfrv lc lcqdrv dwdq lfted fg f mpwd ldz fz acit fz fl udephld zmpz imfem fl erdpqrv jtcit mpzm rdll zdqqcq zmpt zmpz imfem fl uhz mftzda pz pta yhdllda tcq ept fz ud adtfda zmpz sptv cg zmd gpsfrv mpwd uddt htmpkkv ft zmdfq adpzml imfem mpwd uddt lhaadt urccav pta svlzdqfchl vdz spv id lmdrzdq chqldrwdl ft zmd ftgftfzd yccatdll cg kqcwfadted imfem ichra tcz gcqdwdq khtflm zmd fttcedtz udvcta zmpz zmfqa cq gchqzm ydtdqpzfct imfem fl zmqdpzdtda ft mcrv iqfz zc zmpz kqcwfadted sv lctl f mdqduv ecssdta vch pta f echtldr vch uv ipv cg ephzfct zc gcqudpq gqcs eqcllfty zmd sccq ft zmcld apqj mchql imdt zmd kcidql cg dwfr pqd doprzda zmfl gqcs mhyc upljdqwfrrd zc mfl lctl qcaydq pta xcmt ifzm ftlzqhezfctl zmpz zmdv lpv tczmfty zmdqdcg zc zmdfq lflzdq drfbpudzm imdt aq scqzfsdq mpa gftflmda qdpafty zmfl lftyhrpq tpqqpzfwd md khlmda mfl lkdezperdl hk ct mfl gcqdmdpa pta lzpqda peqcll pz sq lmdqrcej mcrsdl zmd rpzzdq vpitda pta zcllda zmd dta cg mfl efypqdzzd ftzc zmd gfqd idrr lpfa md ac vch tcz gfta fz ftzdqdlzfty zc p ecrrdezcq cg gpfqv zprdl aq scqzfsdq aqdi p gcrada tdilkpkdq chz cg mfl kcejdz tci sq mcrsdl id ifrr yfwd vch lcsdzmfty p rfzzrd scqd qdedtz zmfl fl zmd adwct echtzv emqctferd cg spv zm cg zmfl vdpq fz fl p lmcqz peechtz cg zmd gpezl drfefzda pz zmd adpzm cg lfq empqrdl upljdqwfrrd imfem ceehqqda p gdi apvl udgcqd zmpz apzd sv gqfdta rdptda p rfzzrd gcqipqa pta mfl dokqdllfct udepsd ftzdtz chq wflfzcq qdpaxhlzda mfl yrplldl pta udypt zmd qdedtz lhaadt adpzm cg lfq empqrdl upljdqwfrrd imcld tpsd mpl uddt sdtzfctda pl zmd kqcupurd rfudqpr eptafapzd gcq sfaadwct pz zmd tdoz drdezfct mpl eplz p yrccs cwdq zmd echtzv zmchym lfq empqrdl mpa qdlfada pz upljdqwfrrd mprr gcq p ecskpqpzfwdrv lmcqz kdqfca mfl psfpufrfzv cg empqpezdq pta dozqdsd ydtdqclfzv mpa ict zmd pggdezfct pta qdlkdez cg prr imc mpa uddt uqchymz ftzc ectzpez ifzm mfs ft zmdld apvl cg tchwdpho qfemdl fz fl qdgqdlmfty zc gfta p epld imdqd zmd lefct cg pt cra echtzv gpsfrv imfem mpl gprrdt hkct dwfr apvl fl purd zc spjd mfl cit gcqzhtd pta zc uqfty fz upej ifzm mfs zc qdlzcqd zmd gprrdt yqptadhq cg mfl rftd lfq empqrdl pl fl idrr jtcit spad rpqyd lhsl cg sctdv ft lchzm pgqfept lkdehrpzfct scqd ifld zmpt zmcld imc yc ct htzfr zmd imddr zhqtl pypftlz zmds md qdprfbda mfl ypftl pta qdzhqtda zc dtyrpta ifzm zmds fz fl ctrv zic vdpql lfted md zccj hk mfl qdlfadted pz upljdqwfrrd mprr pta fz fl ecssct zprj mci rpqyd idqd zmcld lemdsdl cg qdectlzqhezfct pta fskqcwdsdtz imfem mpwd uddt ftzdqqhkzda uv mfl adpzm udfty mfsldrg emfrardll fz ipl mfl ckdtrv dokqdllda adlfqd zmpz zmd imcrd echtzqvlfad lmchra ifzmft mfl cit rfgdzfsd kqcgfz uv mfl ycca gcqzhtd pta sptv ifrr mpwd kdqlctpr qdplctl gcq udipfrfty mfl htzfsdrv dta mfl ydtdqchl actpzfctl zc rcepr pta echtzv empqfzfdl mpwd uddt gqdnhdtzrv emqctferda ft zmdld ecrhstl zmd efqehslzptedl ecttdezda ifzm zmd adpzm cg lfq empqrdl epttcz ud lpfa zc mpwd uddt dtzfqdrv erdpqda hk uv zmd ftnhdlz uhz pz rdplz dtchym mpl uddt actd zc aflkcld cg zmcld qhschql zc imfem rcepr lhkdqlzfzfct mpl yfwdt qfld zmdqd fl tc qdplct impzdwdq zc lhlkdez gchr krpv cq zc fspyftd zmpz adpzm echra ud gqcs ptv uhz tpzhqpr ephldl lfq empqrdl ipl p ifacidq pta p spt imc spv ud lpfa zc mpwd uddt ft lcsd ipvl cg pt deedtzqfe mpufz cg sfta ft lkfzd cg mfl ectlfadqpurd idprzm md ipl lfskrd ft mfl kdqlctpr zplzdl pta mfl ftaccq ldqwptzl pz upljdqwfrrd mprr ectlflzda cg p spqqfda echkrd tpsda upqqvscqd zmd mhlupta pezfty pl uhzrdq pta zmd ifgd pl mchldjddkdq zmdfq dwfadted ecqqcucqpzda uv zmpz cg ldwdqpr gqfdtal zdtal zc lmci zmpz lfq empqrdll mdprzm mpl gcq lcsd zfsd uddt fskpfqda pta kcftzl dlkdefprrv zc lcsd pggdezfct cg zmd mdpqz sptfgdlzfty fzldrg ft emptydl cg ecrchq uqdpzmrdlltdll pta pehzd pzzpejl cg tdqwchl adkqdllfct aq xpsdl scqzfsdq zmd gqfdta pta sdafepr pzzdtaptz cg zmd adedplda mpl yfwdt dwfadted zc zmd lpsd dggdez zmd gpezl cg zmd epld pqd lfskrd lfq empqrdl upljdqwfrrd ipl ft zmd mpufz dwdqv tfymz udgcqd ycfty zc uda cg iprjfty acit zmd gpschl vdi prrdv cg upljdqwfrrd mprr zmd dwfadted cg zmd upqqvscqdl lmcil zmpz zmfl mpa uddt mfl ehlzcs ct zmd zm cg spv lfq empqrdl mpa aderpqda mfl ftzdtzfct cg lzpqzfty tdoz apv gcq rctact pta mpa cqadqda upqqvscqd zc kqdkpqd mfl rhyypyd zmpz tfymz md idtz chz pl hlhpr gcq mfl tcezhqtpr iprj ft zmd echqld cg imfem md ipl ft zmd mpufz cg lscjfty p efypq md tdwdq qdzhqtda pz zidrwd cercej upqqvscqd gftafty zmd mprr accq lzfrr ckdt udepsd prpqsda pta rfymzfty p rptzdqt idtz ft ldpqem cg mfl splzdq zmd apv mpa uddt idz pta lfq empqrdll gcczspqjl idqd dplfrv zqpeda acit zmd prrdv mprgipv acit zmfl iprj zmdqd fl p ypzd imfem rdpal chz ct zc zmd sccq zmdqd idqd ftafepzfctl zmpz lfq empqrdl mpa lzcca gcq lcsd rfzzrd zfsd mdqd md zmdt kqceddada acit zmd prrdv pta fz ipl pz zmd gpq dta cg fz zmpz mfl ucav ipl aflecwdqda ctd gpez imfem mpl tcz uddt dokrpftda fl zmd lzpzdsdtz cg upqqvscqd zmpz mfl splzdql gcczkqftzl przdqda zmdfq empqpezdq gqcs zmd zfsd zmpz md kpllda zmd sccqypzd pta zmpz md pkkdpqda gqcs zmdted ctipqa zc mpwd uddt iprjfty hkct mfl zcdl ctd shqkmv p yfklv mcqldadprdq ipl ct zmd sccq pz tc yqdpz aflzpted pz zmd zfsd uhz md pkkdpql uv mfl cit ectgdllfct zc mpwd uddt zmd icqld gcq aqftj md aderpqdl zmpz md mdpqa eqfdl uhz fl htpurd zc lzpzd gqcs impz afqdezfct zmdv epsd tc lfytl cg wfcrdted idqd zc ud aflecwdqda hkct lfq empqrdll kdqlct pta zmchym zmd acezcql dwfadted kcftzda zc pt prsclz fteqdafurd gpefpr aflzcqzfctlc yqdpz zmpz aq scqzfsdq qdghlda pz gfqlz zc udrfdwd zmpz fz ipl ftadda mfl gqfdta pta kpzfdtz imc rpv udgcqd mfsfz ipl dokrpftda zmpz zmpz fl p lvskzcs imfem fl tcz hthlhpr ft epldl cg avlktcdp pta adpzm gqcs epqafpe domphlzfct zmfl dokrptpzfct ipl ucqtd chz uv zmd kclzscqzds dopsftpzfct imfem lmcida rctylzptafty cqyptfe afldpld pta zmd ecqctdql xhqv qdzhqtda p wdqafez ft peecqapted ifzm zmd sdafepr dwfadted fz fl idrr zmpz zmfl fl lc gcq fz fl cuwfchlrv cg zmd hzsclz fskcqzpted zmpz lfq empqrdll mdfq lmchra ldzzrd pz zmd mprr pta ectzfthd zmd ycca icqj imfem mpl uddt lc lparv ftzdqqhkzda mpa zmd kqclpfe gftafty cg zmd ecqctdq tcz gftprrv khz pt dta zc zmd qcsptzfe lzcqfdl imfem mpwd uddt imflkdqda ft ecttdezfct ifzm zmd pggpfq fz sfymz mpwd uddt afggfehrz zc gfta p zdtptz gcq upljdqwfrrd mprr fz fl htadqlzcca zmpz zmd tdoz cg jft fl sq mdtqv upljdqwfrrd fg md ud lzfrr prfwd zmd lct cg lfq empqrdl upljdqwfrrdl vchtydq uqczmdq zmd vchty spt imdt rplz mdpqa cg ipl ft psdqfep pta ftnhfqfdl pqd udfty ftlzfzhzda ifzm p wfdi zc ftgcqsfty mfs cg mfl ycca gcqzhtd aq scqzfsdq qdgcrada mfl kpkdq pta qdkrpeda fz ft mfl kcejdz zmcld pqd zmd khurfe gpezl sq mcrsdl ft ecttdezfct ifzm zmd adpzm cg lfq empqrdl upljdqwfrrd f shlz zmptj vch lpfa lmdqrcej mcrsdl gcq eprrfty sv pzzdtzfct zc p epld imfem edqzpftrv kqdldtzl lcsd gdpzhqdl cg ftzdqdlz f mpa culdqwda lcsd tdilkpkdq ecssdtz pz zmd zfsd uhz f ipl doeddaftyrv kqdceehkfda uv zmpz rfzzrd pggpfq cg zmd wpzfept epsdcl pta ft sv ptofdzv zc curfyd zmd kckd f rclz zchem ifzm ldwdqpr ftzdqdlzfty dtyrflm epldl zmfl pqzferd vch lpv ectzpftl prr zmd khurfe gpezl fz acdl zmdt rdz sd mpwd zmd kqfwpzd ctdl md rdptda upej khz mfl gftydqzfkl zcydzmdq pta pllhsda mfl sclz fskpllfwd pta xhafefpr dokqdllfct ft acfty lc lpfa aq scqzfsdq imc mpa udyht zc lmci lfytl cg lcsd lzqcty dsczfct f ps zdrrfty zmpz imfem f mpwd tcz ectgfada zc ptvctd sv sczfwd gcq ifzmmcrafty fz gqcs zmd ecqctdql ftnhfqv fl zmpz p spt cg lefdted lmqftjl gqcs krpefty mfsldrg ft zmd khurfe kclfzfct cg lddsfty zc ftacqld p kckhrpq lhkdqlzfzfct f mpa zmd ghqzmdq sczfwd zmpz upljdqwfrrd mprr pl zmd kpkdq lpvl ichra edqzpftrv qdspft htzdtptzda fg ptvzmfty idqd actd zc fteqdpld fzl prqdpav qpzmdq yqfs qdkhzpzfct gcq uczm zmdld qdplctl f zmchymz zmpz f ipl xhlzfgfda ft zdrrfty qpzmdq rdll zmpt f jtdi lfted tc kqpezfepr ycca echra qdlhrz gqcs fz uhz ifzm vch zmdqd fl tc qdplct imv f lmchra tcz ud kdqgdezrv gqptj zmd sccq fl wdqv lkpqldrv ftmpufzda pta zmcld imc rfwd tdpq dpem czmdq pqd zmqcit wdqv shem zcydzmdq gcq zmfl qdplct f lpi p ycca adpr cg lfq empqrdl upljdqwfrrd ifzm zmd doedkzfct cg sq gqptjrpta cg rpgzdq mprr pta sq lzpkrdzct zmd tpzhqprflz zmdqd pqd tc czmdq sdt cg dahepzfct ifzmft sptv sfrdl lfq empqrdl ipl p qdzfqfty spt uhz zmd empted cg mfl frrtdll uqchymz hl zcydzmdq pta p ecsshtfzv cg ftzdqdlzl ft lefdted jdkz hl lc md mpa uqchymz upej shem lefdtzfgfe ftgcqspzfct gqcs lchzm pgqfep pta sptv p empqsfty dwdtfty id mpwd lkdtz zcydzmdq aflehllfty zmd ecskpqpzfwd ptpzcsv cg zmd uhlmspt pta zmd mczzdtzcz ifzmft zmd rplz gdi sctzml fz udepsd fteqdplftyrv krpft zc sd zmpz lfq empqrdll tdqwchl lvlzds ipl lzqpftda zc zmd uqdpjfty kcftz md mpa zpjdt zmfl rdydta imfem f mpwd qdpa vch doeddaftyrv zc mdpqzlc shem lc zmpz przmchym md ichra iprj ft mfl cit yqchtal tczmfty ichra ftahed mfs zc yc chz hkct zmd sccq pz tfymz fteqdafurd pl fz spv pkkdpq zc vch sq mcrsdl md ipl mctdlzrv ectwfteda zmpz p aqdpaghr gpzd cwdqmhty mfl gpsfrv pta edqzpftrv zmd qdecqal imfem md ipl purd zc yfwd cg mfl ptedlzcql idqd tcz dtechqpyfty zmd fadp cg lcsd ymplzrv kqdldted ectlzptzrv mphtzda mfs pta ct scqd zmpt ctd ceeplfct md mpl pljda sd imdzmdq f mpa ct sv sdafepr xchqtdvl pz tfymz dwdq lddt ptv lzqptyd eqdpzhqd cq mdpqa zmd upvfty cg p mchta zmd rpzzdq nhdlzfct md khz zc sd ldwdqpr zfsdl pta pripvl ifzm p wcfed imfem wfuqpzda ifzm doefzdsdtz f ept idrr qdsdsudq aqfwfty hk zc mfl mchld ft zmd dwdtfty lcsd zmqdd iddjl udgcqd zmd gpzpr dwdtz md empteda zc ud pz mfl mprr accq f mpa adledtada gqcs sv yfy pta ipl lzptafty ft gqctz cg mfs imdt f lpi mfl dvdl gfo zmdsldrwdl cwdq sv lmchradq pta lzpqd kplz sd ifzm pt dokqdllfct cg zmd sclz aqdpaghr mcqqcq f imfljda qchta pta mpa xhlz zfsd zc epzem p yrfskld cg lcsdzmfty imfem f zccj zc ud p rpqyd urpej eprg kpllfty pz zmd mdpa cg zmd aqfwd lc doefzda pta prpqsda ipl md zmpz f ipl ecskdrrda zc yc acit zc zmd lkcz imdqd zmd ptfspr mpa uddt pta rccj pqchta gcq fz fz ipl yctd mcidwdq pta zmd ftefadtz pkkdpqda zc spjd zmd icqlz fskqdllfct hkct mfl sfta f lzpvda ifzm mfs prr zmd dwdtfty pta fz ipl ct zmpz ceeplfct zc dokrpft zmd dsczfct imfem md mpa lmcit zmpz md ectgfada zc sv jddkfty zmpz tpqqpzfwd imfem f qdpa zc vch imdt gfqlz f epsd f sdtzfct zmfl lsprr dkflcad udephld fz pllhsdl lcsd fskcqzpted ft wfdi cg zmd zqpydav imfem gcrrcida uhz f ipl ectwfteda pz zmd zfsd zmpz zmd spzzdq ipl dtzfqdrv zqfwfpr pta zmpz mfl doefzdsdtz mpa tc xhlzfgfepzfct fz ipl pz sv pawfed zmpz lfq empqrdl ipl puchz zc yc zc rctact mfl mdpqz ipl f jtdi pggdezda pta zmd ectlzptz ptofdzv ft imfem md rfwda mcidwdq emfsdqfepr zmd ephld cg fz sfymz ud ipl dwfadtzrv mpwfty p ldqfchl dggdez hkct mfl mdprzm f zmchymz zmpz p gdi sctzml pscty zmd aflzqpezfctl cg zcit ichra ldta mfs upej p tdi spt sq lzpkrdzct p shzhpr gqfdta imc ipl shem ectedqtda pz mfl lzpzd cg mdprzm ipl cg zmd lpsd ckftfct pz zmd rplz ftlzptz epsd zmfl zdqqfurd epzplzqckmd ct zmd tfymz cg lfq empqrdll adpzm upqqvscqd zmd uhzrdq imc spad zmd aflecwdqv ldtz kdqjftl zmd yqccs ct mcqldupej zc sd pta pl f ipl lfzzfty hk rpzd f ipl purd zc qdpem upljdqwfrrd mprr ifzmft pt mchq cg zmd dwdtz f emdejda pta ecqqcucqpzda prr zmd gpezl imfem idqd sdtzfctda pz zmd ftnhdlz f gcrrcida zmd gcczlzdkl acit zmd vdi prrdv f lpi zmd lkcz pz zmd sccqypzd imdqd md lddsda zc mpwd ipfzda f qdspqjda zmd emptyd ft zmd lmpkd cg zmd kqftzl pgzdq zmpz kcftz f tczda zmpz zmdqd idqd tc czmdq gcczlzdkl lpwd zmcld cg upqqvscqd ct zmd lcgz yqpwdr pta gftprrv f epqdghrrv dopsftda zmd ucav imfem mpa tcz uddt zchemda htzfr sv pqqfwpr lfq empqrdl rpv ct mfl gped mfl pqsl chz mfl gftydql ahy ftzc zmd yqchta pta mfl gdpzhqdl ectwhrlda ifzm lcsd lzqcty dsczfct zc lhem pt dozdtz zmpz f echra mpqarv mpwd licqt zc mfl fadtzfzv zmdqd ipl edqzpftrv tc kmvlfepr ftxhqv cg ptv jfta uhz ctd gprld lzpzdsdtz ipl spad uv upqqvscqd pz zmd ftnhdlz md lpfa zmpz zmdqd idqd tc zqpedl hkct zmd yqchta qchta zmd ucav md afa tcz culdqwd ptv uhz f afalcsd rfzzrd aflzpted cgg uhz gqdlm pta erdpq gcczkqftzl gcczkqftzl p sptl cq p icsptl aq scqzfsdq rccjda lzqptydrv pz hl gcq pt ftlzptz pta mfl wcfed lptj prsclz zc p imflkdq pl md ptlidqda sq mcrsdl zmdv idqd zmd gcczkqftzl cg p yfyptzfe mchta";
    std::string ciphered_text_1_no_spaces = ciphered_text_1;
    ciphered_text_1_no_spaces.erase(std::remove(ciphered_text_1_no_spaces.begin(), ciphered_text_1_no_spaces.end(), ' '), ciphered_text_1_no_spaces.end());

    auto repeating_words_1 = find_repeating_words(ciphered_text_1, 3, 3);

    std::vector<std::vector<int>> factors;
    for (auto &word : repeating_words_1)
        factors.push_back(analyze_repeating_word(ciphered_text_1, word, false));

    int key_len = 0;
    for (int i = 0; i < 20; i++) {
        bool found = true;
        for (auto &factor : factors) {
            if (!(std::find(factor.begin(), factor.end(), i) != factor.end())) {
                found = false;
            }
        }
        if (found) {
            key_len = i;
            break;
        }
    }
    std::cout << "Vigenere key length: " << key_len << std::endl;
    // -> Sifra neni Vigenerova

    auto freq_1 = analyze_frequency(ciphered_text_1_no_spaces);
    auto freq_1_rel = analyze_frequency_relative(ciphered_text_1_no_spaces);
    std::vector<std::pair<char, float>> freq_1_sorted;

    std::cout << "Frequency analysis: " << std::endl;
    for (int i = 0; i < 26; i++) {
        std::cout << char('a' + i) << ": " << freq_1[i] << " " << freq_1_rel[i] << std::endl;
        freq_1_sorted.emplace_back(char('a' + i), freq_1_rel[i]);
    }

    std::vector<std::pair<char, float>> freq_en_sorted = {
            // https://en.wikipedia.org/wiki/Letter_frequency
            {'a', 0.082},
            {'b', 0.015},
            {'c', 0.028},
            {'d', 0.043},
            {'e', 0.13},
            {'f', 0.022},
            {'g', 0.02},
            {'h', 0.061},
            {'i', 0.07},
            {'j', 0.0015},
            {'k', 0.0077},
            {'l', 0.04},
            {'m', 0.024},
            {'n', 0.067},
            {'o', 0.075},
            {'p', 0.019},
            {'q', 0.00095},
            {'r', 0.06},
            {'s', 0.063},
            {'t', 0.091},
            {'u', 0.028},
            {'v', 0.0098},
            {'w', 0.024},
            {'x', 0.0015},
            {'y', 0.02},
            {'z', 0.00074}
    };

    std::sort(freq_1_sorted.begin(), freq_1_sorted.end(), [](const std::pair<char, float> &a, const std::pair<char, float> &b) {
        return a.second > b.second;
    });
    std::sort(freq_en_sorted.begin(), freq_en_sorted.end(), [](const std::pair<char, float> &a, const std::pair<char, float> &b) {
        return a.second > b.second;
    });

    std::cout << "Frequency analysis sorted: " << std::endl;
    for (int i = 0; i < 26; i++)
        std::cout << "(EN) " << freq_en_sorted[i].first << ": " << freq_en_sorted[i].second << " | (Ciphered Text) " << freq_1_sorted[i].first << ": " << freq_1_sorted[i].second  << std::endl;

    auto bigrams = analyze_bigrams_relative(ciphered_text_1);
    std::sort(bigrams.begin(), bigrams.end(), [](const std::pair<std::string, float> &a, const std::pair<std::string, float> &b) {
        return a.second > b.second;
    });
    std::cout << "Bigrams analysis: " << std::endl;
    for (auto &bigram : bigrams)
        std::cout << bigram.first << ": " << bigram.second << std::endl;

    // zm -> th?
    // md -> he?
    // ft -> in?
    // https://www3.nd.edu/~busiforc/handouts/cryptography/Letter%20Frequencies.html
    // https://en.wikipedia.org/wiki/Bigram

    auto trigrams = analyze_trigrams_relative(ciphered_text_1);
    std::sort(trigrams.begin(), trigrams.end(), [](const std::pair<std::string, float> &a, const std::pair<std::string, float> &b) {
        return a.second > b.second;
    });
    std::cout << "Trigrams analysis: " << std::endl;
    for (auto &trigram : trigrams)
        std::cout << trigram.first << ": " << trigram.second << std::endl;

    // zmd -> the?
    // ft -> in => ft? -> ing (fty)
    // navic je casty trigram zmp a mpz, z ktereho uz znam z a m.. zmp -> th? & mpz -> h?t
    // podle statistik by se melo jednat o "tha" a "hat"... takze p -> a
    // kdyz mam 'a', slo by najit jednoduse "all", tedy "p??".. ve vypisu jsem nasel "prr", lze tedy usoudit ze r -> l
    // jelikoz uz mam 'a' a 'n', druhy nejcastejsi trigram je pta -> "an?", ocekavam tedy, ze to je "and" a -> d
    // dale jsou docela caste "her" a "ere", z cehoz 'h' a 'e' uz mam (the).. musim tedy najit "md?" a "d?d"
    // nasel jsem mdq a dqd -> usuzuji ze q -> r
    // dale bych chtel vyuzit podobnost pismen v "thi" "tio" a "ion"... hledam tedy "zm?" "z?!" a "?!t"
    // nasel jsem "zmf" "zfc" a "fct" ... soudim ted ze f -> i a c -> o
    // https://www3.nd.edu/~busiforc/handouts/cryptography/Letter%20Frequencies.html
    // https://en.wikipedia.org/wiki/Trigram

    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string ciphered_alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 26; i++)
        ciphered_alphabet[alphabet.find(freq_en_sorted[i].first)] = freq_1_sorted[i].first;

    // z -> t
    std::swap(ciphered_alphabet[ciphered_alphabet.find('z')], ciphered_alphabet['t' - 'a']);
    // m -> h
    std::swap(ciphered_alphabet[ciphered_alphabet.find('m')], ciphered_alphabet['h' - 'a']);
    // d -> e
    std::swap(ciphered_alphabet[ciphered_alphabet.find('d')], ciphered_alphabet['e' - 'a']);
    // f -> i
    std::swap(ciphered_alphabet[ciphered_alphabet.find('f')], ciphered_alphabet['i' - 'a']);
    // t -> n
    std::swap(ciphered_alphabet[ciphered_alphabet.find('t')], ciphered_alphabet['n' - 'a']);
    // y -> g
    std::swap(ciphered_alphabet[ciphered_alphabet.find('y')], ciphered_alphabet['g' - 'a']);
    // p -> a
    std::swap(ciphered_alphabet[ciphered_alphabet.find('p')], ciphered_alphabet['a' - 'a']);
    // r -> l
    std::swap(ciphered_alphabet[ciphered_alphabet.find('r')], ciphered_alphabet['l' - 'a']);
    // a -> d
    std::swap(ciphered_alphabet[ciphered_alphabet.find('a')], ciphered_alphabet['d' - 'a']);
    // q -> r
    std::swap(ciphered_alphabet[ciphered_alphabet.find('q')], ciphered_alphabet['r' - 'a']);
    //  f -> i
    std::swap(ciphered_alphabet[ciphered_alphabet.find('f')], ciphered_alphabet['i' - 'a']);
    // c -> o
    std::swap(ciphered_alphabet[ciphered_alphabet.find('c')], ciphered_alphabet['o' - 'a']);

//    // Text uz skoro dava smysl, ale nekde je spatne
//    // ".. verg late is the uorsisyn nave mpos thone.."
//    // verg je urcite very ... navic verg je v sifrovanem originalnim textu wdqv
//    // tedy lze vyvodit, ze v -> y
//    std::swap(ciphered_alphabet[ciphered_alphabet.find('v')], ciphered_alphabet['y' - 'a']);
//
//    // Dale jsem nasel ".. neated at the breakwant table i ntood.."
//    // breakwant by mohlo byt breakfast a ntood to potvrzuje, protoze by to mel byt stood..
//    // breakfast je v originale uqdpjgplz a stood je v originale lzcca
//    // lze tedy vyvodit, ze g -> f a l -> s
//    std::swap(ciphered_alphabet[ciphered_alphabet.find('g')], ciphered_alphabet['f' - 'a']);
//    std::swap(ciphered_alphabet[ciphered_alphabet.find('l')], ciphered_alphabet['s' - 'a']);
//
//    // Text uz zacina davat smysl. skoro
//    // "ur sherlock holues who was nsmally very late in the uornings save.."
//    // tak to asi bude mr sherlock holmes, nikoliv ur sherlock holues
//    // navic uornings zni jako mornings.. coz potvrzuje ten prepis ur na mr.. a holues take potvrzuje u->m
//    // mr je v originale sq, tedy s -> m
//    std::swap(ciphered_alphabet[ciphered_alphabet.find('s')], ciphered_alphabet['m' - 'a']);
//
//    // xames mortimer mel byt urcite james mortimer
//    // james je v puvodni sifre xpsdl, tedy x -> j
//    std::swap(ciphered_alphabet[ciphered_alphabet.find('x')], ciphered_alphabet['j' - 'a']);

    // Bohuzel jsem si precetl pozde, ze se to nema delat okometricky, ale automaticky programove
    // Vyse zakomentovany kod jsem tu nechal kvuli spravnym uvaham

    std::vector<std::string> en_dictionary; // https://www.bragitoff.com/2016/03/english-dictionary-in-csv-format/
    std::string filename = "dictionary.txt";
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
#ifdef __linux__
        en_dictionary.push_back(line.substr(0, line.size() - 1));
#elif _WIN32
        en_dictionary.push_back(line);
#endif
    std::sort(en_dictionary.begin(), en_dictionary.end());
    en_dictionary.erase(std::unique(en_dictionary.begin(), en_dictionary.end()), en_dictionary.end());

    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis2(0, 25);
    double score = 0;
    int number_of_words = 6075;
    std::string best_alphabet_so_far = ciphered_alphabet;
    double best_score = 0;
    std::vector<std::pair<char, char>> best_swaps;
    std::vector<std::pair<char, char>> swaps;
    std::vector<std::pair<char, char>> bad_swaps;
    int generation = 0;

    while (true) {
        std::string plaintext = substitute_alphabet_decipher(ciphered_text_1, ciphered_alphabet);
        while (plaintext.find(' ') != std::string::npos) {
            auto word = plaintext.substr(0, plaintext.find(' '));
            if (std::find(en_dictionary.begin(), en_dictionary.end(), word) != en_dictionary.end())
                score++;
            plaintext = plaintext.substr(plaintext.find(' ') + 1);
        }

        score /= number_of_words;
        if (score > best_score) {
            best_score = score;
            best_alphabet_so_far = ciphered_alphabet;
            best_swaps.clear();
            for (auto swap : swaps)
                best_swaps.push_back(swap);
        }

        if (score > 0.918)
            break;

        if (score < best_score) {
            for (auto swap : swaps)
                bad_swaps.push_back(swap);
        }
        std::sort(bad_swaps.begin(), bad_swaps.end());
        bad_swaps.erase(std::unique(bad_swaps.begin(), bad_swaps.end()), bad_swaps.end());

        score = 0;
        ciphered_alphabet = best_alphabet_so_far;
        swaps.clear();

        for (int i = 0; i < 26; i++) {
            if (dis(gen) < best_score)
                continue;
            int j = dis2(gen);
            if (std::find(bad_swaps.begin(), bad_swaps.end(), std::make_pair(ciphered_alphabet[i], ciphered_alphabet[j])) != bad_swaps.end() && dis(gen) < 0.5)
                continue;
            std::swap(ciphered_alphabet[i], ciphered_alphabet[j]);
            swaps.emplace_back(ciphered_alphabet[i], ciphered_alphabet[j]);
        }

        for (auto &swap : best_swaps)
            std::swap(ciphered_alphabet[ciphered_alphabet.find(swap.first)], ciphered_alphabet[swap.second - 'a']);

        // na zaklade bigramove a trigramove analyzy jsem si jisty pismeny:
        // z m d f t y p r a q f c -> t h e i n g a l d r i o
        std::swap(ciphered_alphabet[ciphered_alphabet.find('z')], ciphered_alphabet['t' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('m')], ciphered_alphabet['h' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('d')], ciphered_alphabet['e' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('f')], ciphered_alphabet['i' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('t')], ciphered_alphabet['n' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('y')], ciphered_alphabet['g' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('p')], ciphered_alphabet['a' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('r')], ciphered_alphabet['l' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('a')], ciphered_alphabet['d' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('q')], ciphered_alphabet['r' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('f')], ciphered_alphabet['i' - 'a']);
        std::swap(ciphered_alphabet[ciphered_alphabet.find('c')], ciphered_alphabet['o' - 'a']);

        if (!(generation % 10))
            std::cout << "Generation: " << generation << std::endl << "Best score: " << best_score << std::endl << "Best alphabet: " << best_alphabet_so_far << std::endl;
        if (!(generation % 50))
            bad_swaps.clear();
        if (!(generation % 100))
            best_swaps.clear();

        generation++;
    }

    std::cout << "Ciphered alphabet: " << ciphered_alphabet << std::endl;
    std::cout << "Deciphered text: " << std::endl;
    auto plaintext_1 = substitute_alphabet_decipher(ciphered_text_1, ciphered_alphabet);
    std::cout << plaintext_1 << std::endl;

    // Jedna se o The Hound of the Baskervilles od Sir Arthur Conan Doyle

    return 0;
}
