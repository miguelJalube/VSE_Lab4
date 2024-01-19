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