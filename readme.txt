Makefile : Am ales sa folosesc un makefile din scheletul oferit la laborator. Important este 

subscriber.c

Subscriber-ul este implementat intr-un fel simplu: verifica si afiseaza mesajele primite, respectiv le trimite daca este necesar.
Va verifica daca mesajul citit de la tastatura este exit, subscribe sau unsubscribe si va actiona conform cerintelor. Daca nu se vor primi mesaje de la tastatura, se va verifica daca server-ul a trimis un mesaj si se va printa cu formatul respectiv- gata trimis de server.
In privinta formatului, server-ul va face toata treaba, singurul lucru care ii mai ramane subscriber-ului va fi ca doar sa il printeze.

server.c

Pentru a putea retine topic-urile la care se va abona fiecare subscriber, se va folosi doua structuri: topicList si subsList.

topicList reprezinta o lista simplu inlantuita in care se vor retine numele topic-urilor la care se va abona un singur subscriber.

subsList reprinta o lista simplu inlantuita ce reprezinta subscriberii . Aceasta va avea id-ul subscriber-ului respetiv, lista sa de topic-uri , precum si o legatura catre urmatorul subscriber.

Reprezentarea folosind liste simplu intalnuite mi s-a parut una foarte potrivita dat fiind faptul ca trebuie sa verificam in parte, pentru fiecare subscriber, fiecare topic si mai ales pentru ca acest lucru ar trebui sa functioneze chiar si in cazul unui numar foarte mare de topic-uri sau subscriberi. Alocarea dinamica mi s-a parut potrivita in acest context deoarece nu putem stii exact numarul de topic-uri sau subscriberi si am dori sa nu avem vreo restrictie in acest caz.

In server.c sunt puse comentarii aditionale cu privire la rolul functiilor alese.

Prima verificare pe care o va face server-ul va fi existenta unui nou client. Altfel incepe partea de prelucrare a mesajului transmis de subscriber-ul respectiv: se va verifica daca este de tip subscribe sau unsubscribe si se va adauga topicul sau se va sterge, in functie de dorinta subscriber-ului. 

Server-ul va fi de asemenea atent la primirea unor mesaje din partea clientilor udp si va verifica pentru fiecare subscriber in parte daca este necesar sa retina informatia, sa o trimita, sau sa o ignore. Acest lucru se face folosind si cele doua structuri definite. 

