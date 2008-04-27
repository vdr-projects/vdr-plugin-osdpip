/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#if (APIVERSNUM < 10507)

#include "i18n.h"

const tI18nPhrase Phrases[] = {
    {   "OSD Picture-in-Picture", // English
        "OSD Bild-in-Bild", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "OSD-PIP Incrustation d'image dans l'image", // Fran�ais
        "", // Norsk
        "Kuva kuvassa -toiminto (OSD)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Picture-in-Picture", // English
        "Bild-in-Bild", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Incrustation d'image", // Fran�ais
        "", // Norsk
        "Kuva kuvassa", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Crop left", // English
        "Links abschneiden", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couper a gauche", // Fran�ais
        "", // Norsk
        "Leikkaa vasemmalta", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Crop right", // English
        "Rechts abschneiden", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couper a droite", // Fran�ais
        "", // Norsk
        "Leikkaa oikealta", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Crop at top", // English
        "Oben abschneiden", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couper la partie haute", // Fran�ais
        "", // Norsk
        "Leikkaa ylh��lt�", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Crop at bottom", // English
        "Unten abschneiden", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couper la partie basse", // Fran�ais
        "", // Norsk
        "Leikkaa alhaalta", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Color depth", // English
        "Farbtiefe", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Nobre de couleur pour l'image", // Fran�ais
        "", // Norsk
        "V�risyvyys", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Size", // English
        "Gr��e", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Taille", // Fran�ais
        "", // Norsk
        "Koko", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Frames to display", // English
        "Darzustellende Bilder", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Sequence a afficher", // Fran�ais
        "", // Norsk
        "N�yt� kehykset", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Drop frames", // English
        "Einzelbilder auslassen", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Sauter des sequences", // Fran�ais
        "", // Norsk
        "Hylk�� kehyksi�", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Swap FFMPEG output", // English
        "FFMPEG Ausgabe umdrehen", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Changer la sortie de FFMPEG", // Fran�ais
        "", // Norsk
        "Invertoi FFMPEG-kuva", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Show info window", // English
        "Info-Fenster anzeigen", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Montrer les informations de la fenetre", // Fran�ais
        "", // Norsk
        "N�yt� kanavatieto", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Info window width", // English
        "Breite Info-Fenster", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Information sur la taille de la fenetre", // Fran�ais
        "", // Norsk
        "Kanavatiedon leveys", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Info window position", // English
        "Position Info-Fenster", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Information sur la position de la fenetre", // Fran�ais
        "", // Norsk
        "Kanavatiedon sijainti", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "top left", // English
        "oben links", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "En haut a gauche", // Fran�ais
        "", // Norsk
        "vasen yl�reuna", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "top right", // English
        "oben rechts", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "En haut a droite", // Fran�ais
        "", // Norsk
        "oikea yl�reuna", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "bottom left", // English
        "unten links", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "En bas a gauche", // Fran�ais
        "", // Norsk
        "vasen alareuna", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "bottom right", // English
        "unten rechts", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "En bas a droite", // Fran�ais
        "", // Norsk
        "oikea alareuna", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Greyscale (16)", // English
        "16 Graustufen", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Niveau de gris (16)", // Fran�ais
        "", // Norsk
        "harmaas�vy (16)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Greyscale (256)", // English
        "256 Graustufen", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Niveau de gris (256)", // Fran�ais
        "", // Norsk
        "harmaas�vy (256)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Color (256, fixed)", // English
        "256 Farben (fest)", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couleur (256 fixe)", // Fran�ais
        "", // Norsk
        "v�ri (256)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Color (128, variable)", // English
        "128 Farben (variabel)", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Couleur (128 variable)", // Fran�ais
        "", // Norsk
        "v�ri (128, vaihtuva)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Color (256, dithered)", // English
        "256 Farben (dithered)", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "", // Fran�ais
        "", // Norsk
        "v�ri (256)", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "X Position", // English
        "X-Position", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "X Position", // Fran�ais
        "", // Norsk
        "Vaakasijainti", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Y Position", // English
        "Y-Position", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Y Position", // Fran�ais
        "", // Norsk
        "Pystysijainti", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "automatic", // English
        "automatisch", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Automatique", // Fran�ais
        "", // Norsk
        "automaattisesti", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "none", // English
        "keine", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Aucun", // Fran�ais
        "", // Norsk
        "ei", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "1 frame", // English
        "1 Frame", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Sequence 1", // Fran�ais
        "", // Norsk
        "1 kehys", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "2 frames", // English
        "2 Frames", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Sequence 2", // Fran�ais
        "", // Norsk
        "2 kehyst�", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "channel only", // English
        "nur Kanal", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Seulement la chaine", // Fran�ais
        "", // Norsk
        "kanava", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "simple", // English
        "einfach", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Simple", // Fran�ais
        "", // Norsk
        "lyhyt", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "complete", // English
        "komplett", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Complet", // Fran�ais
        "", // Norsk
        "pitk�", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Normal mode", // English
        "Normaler Modus", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Mode normale", // Fran�ais
        "", // Norsk
        "Normaalitoiminto", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Move mode", // English
        "Bewegungsmodus", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Portugu�s
        "Mode mouvement", // Fran�ais
        "", // Norsk
        "Siirtotoiminto", // Suomi
        "", // Polski
        "", // Espa�ol
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    { NULL }
};

#endif

