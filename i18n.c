/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * Copyright (C) 2004 - 2008 Andreas Regel <andreas.regel@powarman.de>
 * Copyright (C) 2004        Sascha Volkenandt <sascha@akv-soft.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if (APIVERSNUM < 10507)

#include "i18n.h"

const tI18nPhrase Phrases[] = {
    {   "OSD Picture-in-Picture", // English
        "OSD Bild-in-Bild", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Português
        "OSD-PIP Incrustation d'image dans l'image", // Français
        "", // Norsk
        "Kuva kuvassa -toiminto (OSD)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Incrustation d'image", // Français
        "", // Norsk
        "Kuva kuvassa", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couper a gauche", // Français
        "", // Norsk
        "Leikkaa vasemmalta", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couper a droite", // Français
        "", // Norsk
        "Leikkaa oikealta", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couper la partie haute", // Français
        "", // Norsk
        "Leikkaa ylhäältä", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couper la partie basse", // Français
        "", // Norsk
        "Leikkaa alhaalta", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Nobre de couleur pour l'image", // Français
        "", // Norsk
        "Värisyvyys", // Suomi
        "", // Polski
        "", // Español
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Size", // English
        "Größe", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Português
        "Taille", // Français
        "", // Norsk
        "Koko", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Sequence a afficher", // Français
        "", // Norsk
        "Näytä kehykset", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Sauter des sequences", // Français
        "", // Norsk
        "Hylkää kehyksiä", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Changer la sortie de FFMPEG", // Français
        "", // Norsk
        "Invertoi FFMPEG-kuva", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Montrer les informations de la fenetre", // Français
        "", // Norsk
        "Näytä kanavatieto", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Information sur la taille de la fenetre", // Français
        "", // Norsk
        "Kanavatiedon leveys", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Information sur la position de la fenetre", // Français
        "", // Norsk
        "Kanavatiedon sijainti", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "En haut a gauche", // Français
        "", // Norsk
        "vasen yläreuna", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "En haut a droite", // Français
        "", // Norsk
        "oikea yläreuna", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "En bas a gauche", // Français
        "", // Norsk
        "vasen alareuna", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "En bas a droite", // Français
        "", // Norsk
        "oikea alareuna", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Niveau de gris (16)", // Français
        "", // Norsk
        "harmaasävy (16)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Niveau de gris (256)", // Français
        "", // Norsk
        "harmaasävy (256)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couleur (256 fixe)", // Français
        "", // Norsk
        "väri (256)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Couleur (128 variable)", // Français
        "", // Norsk
        "väri (128, vaihtuva)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "", // Français
        "", // Norsk
        "väri (256)", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "X Position", // Français
        "", // Norsk
        "Vaakasijainti", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Y Position", // Français
        "", // Norsk
        "Pystysijainti", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Automatique", // Français
        "", // Norsk
        "automaattisesti", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Aucun", // Français
        "", // Norsk
        "ei", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Sequence 1", // Français
        "", // Norsk
        "1 kehys", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Sequence 2", // Français
        "", // Norsk
        "2 kehystä", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Seulement la chaine", // Français
        "", // Norsk
        "kanava", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Simple", // Français
        "", // Norsk
        "lyhyt", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Complet", // Français
        "", // Norsk
        "pitkä", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Mode normale", // Français
        "", // Norsk
        "Normaalitoiminto", // Suomi
        "", // Polski
        "", // Español
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
        "", // Português
        "Mode mouvement", // Français
        "", // Norsk
        "Siirtotoiminto", // Suomi
        "", // Polski
        "", // Español
        "", // Ellinika
        "", // Svenska
        "", // Romaneste
        "", // Magyar
        "", // Catala
        "", // Russian
        "", // Croatian
    },
    {   "Zapping mode", // English
        "Umschaltmodus", // Deutsch
        "", // Slovenski
        "", // Italiano
        "", // Nederlands
        "", // Português
        "", // Français
        "", // Norsk
        "", // Suomi
        "", // Polski
        "", // Español
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

