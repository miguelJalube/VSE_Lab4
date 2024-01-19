<style>
    .center-img {
        display: block;
        margin-left: auto;
        margin-right: auto;
    }
</style>

<div style="text-align:center; font-size: 40px; margin-top: 400px">Labo 4 VSE</div>
<div style="text-align:center; font-size: 40px; margin-top: 15px"></h1>
<div style="text-align:center; font-size: 18px; margin-top: 50px">2024-01-19</h3>
<div style="text-align:center; font-size: 18px; margin-top: 5px">Leandro SARAIVA MAIA</h3>
<div style="text-align:center; font-size: 18px; margin-top: 2px">Miguel JALUBE</h3>

<div style="page-break-after: always"></div>

## Partie 1

Nous avons utilisé des macros afin de rendre l'utilisation de avalon_read et avalon_write plus simple. Par exemple la macro AV_READ va appeler avalon_read en faisant un test qui quitte le programme c en cas d'erreur. Le byteenable est également passé en paramètre implicitement.
Ensuite nous avons fait une fonction C qui fait le calcul désiré ainsi qu'une fonction "testcase" qui permet de lancer un calcul en passant n, a, b et c.

Nous avons eu un bug à un moment car on avait mis bytenable à 0x11 au lieu de 0b11, ce qui fait qu'on faisait le calcul sur 1 byte et on avait les mauvais résultats.

Nous avons testé plusieurs types de valeurs et avons constaté que selon les valeur de a b et c (200, 400 et 500 par example) on a un résultat incorrect. Ceci est du au fait que le calcul se fait sur un nombre de bits limité et que le résultat maximum ne peut pas être supérieur à 65535 (avec DATASIZE à 16 par défaut). C'est un "bug" attendu car c'est une limitiation connue du système.

<div style="page-break-after: always"></div>

## Partie 2

Il a fallu recopier le code pour la computationNode et l'adapter pour getNbComputation et resetNbComputation. Ensuite il faut simplement créer une variable dans la classe FPGAAccess qui contient le nombre de computation. On incrémente cette variable à chaque appelle à compute et on la met à 0 lors de l'appel à resetNbComputation.

<div style="page-break-after: always"></div>

## Partie 3

Il faut adapter le getNbComputation et resetNbComputation pour qu'ils envoient un paquet à la FPGA pour faire respectivement une lecture et une écritre à l'adresse 6.
Pour ce faire on envoie un paquet de pseudo-json à SystemVerilog (via la librairie d'AMIQ). Pour le rd c'est `{rd:monAdresse}` et le write `{wr:adr,maValeur}`. Elle est ensuite parsé dans amiq_top.sv et exécuté via les task avalon_read et avalon_write.
Nous avons crée un script run.sh afin de compiler conditionellement les différents programmes et de les lancer.
`./run.sh` -> tests CLI connecté  à la FPGA  
`./run.sh -s` -> tests CLI avec la FPGA simulée  
`./run.sh -g` -> GUI connecté à la FPGA  
`./run.sh -s -g` -> GUI avec la FPGA simulée  

## Autre

Nous avons ajouté quelques tests pertinents dans l'application CLI.
Nous avons remarqué que nos tests donnaient des résultats différents pour la simulation et sur la FPGA. Le test avec une valeur qui nécessite plus de 16 bits a un résultat différent en simulation et sur la FPGA. Cela est du au fait que la simulation fait le calcul sur plus de bits que la FPGA.

