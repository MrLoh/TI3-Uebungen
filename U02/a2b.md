Wenn das Codewort, im Gegensatz zum Text, relativ kurz ist, kann die Länge des Codeworts relativ leicht ermittelt werden. Bei der Codierung von nur einem Buchstaben läßt sich die Länge des Codeworts schon mit bloßem Auge erkennen:

> Passwort  : 1234
> Klartext  : eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
> Codierung : EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
> Codewort  : 1234123412341234123412341234123412341234123412
> ----------------------------------------------------------
> Chiffre   : 5678567856785678567856785678567856785678567856
> ----------------------------------------------------------

In einem normalen Text wird es, auch nach der Vigenere-Chifre Codierung, Stellen geben, die sich periodisch wiederholen. Deswegen läßt sich, ab einer gewissen Länge des Textes, mithilfe des Kasiski-, oder Friedman-Tests, die Länge des Codeworts ermitteln. Ist die Länge bekannt kann man den Code, wie bei einer typischen Caesar-Verschlüsselung, mittels Häufgkeitsanalyse knacken.

http://de.wikipedia.org/w/index.php?title=Polyalphabetische_Substitution
http://www.iti.fh-fensburg.de/lang/krypto/klassisch.htm
