/*
 * OSD Picture in Picture plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 */

#include "i18n.h"

const tI18nPhrase Phrases[] = {
	{
		"OSD Picture-in-Picture", // English
		"OSD Bild-in-Bild", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Kuva kuvassa -toiminto (OSD)", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Picture-in-Picture", // English
		"Bild-in-Bild", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Kuva kuvassa", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Crop left", // English
		"Links abschneiden", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Leikkaa vasemmalta", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Crop right", // English
		"Rechts abschneiden", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Leikkaa oikealta", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Crop at top", // English
		"Oben abschneiden", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Leikkaa ylh��lt�", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Crop at bottom", // English
		"Unten abschneiden", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Leikkaa alhaalta", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Color depth", // English
		"Farbtiefe", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"V�risyvyys", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Size", // English
		"Gr��e", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Koko", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Frames to display", // English
		"Darzustellende Bilder", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"N�yt� kehykset", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Drop frames", // English
		"Einzelbilder auslassen", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Hylk�� kehyksi�", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Swap FFMPEG output", // English
		"FFMPEG Ausgabe umdrehen", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Invertoi FFMPEG-kuva", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Show info window", // English
		"Info-Fenster anzeigen", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"N�yt� kuvatieto", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Info window width", // English
		"Breite Info-Fenster", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Kuvatiedon leveys", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Info window position", // English
		"Position Info-Fenster", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Kuvatiedon sijainti", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"top left", // English
		"oben links", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"vasen yl�reuna", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"top right", // English
		"oben rechts", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"oikea yl�reuna", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"bottom left", // English
		"unten links", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"vasen alareuna", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"bottom right", // English
		"unten rechts", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"oikea alareuna", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Greyscale (16)", // English
		"16 Graustufen", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"harmaas�vy (16)", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Greyscale (256)", // English
		"256 Graustufen", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"harmaas�vy (256)", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Color (256, fixed)", // English
		"256 Farben (fest)", // Deutsch
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
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Color (128, variable)", // English
		"128 Farben (variabel)", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"v�ri (128, vaihtuva)", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"X Position", // English
		"X-Position", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Vaakasijainti", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"Y Position", // English
		"Y-Position", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"Pystysijainti", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"automatic", // English
		"automatisch", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"automaattisesti", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"none", // English
		"keine", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"ei", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"1 frame", // English
		"1 Frame", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"1 kehys", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{
		"2 frames", // English
		"2 Frames", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Portugu�s
		"", // Fran�ais
		"", // Norsk
		"2 kehyst�", // Suomi
		"", // Polski
		"", // Espa�ol
		"", // Ellinika
		"", // Svenska
		"", // Romaneste
		"", // Magyar
		"", // Catala
#if VDRVERSNUM > 10302
		"", // Russian
# if VDRVERSNUM > 10307
		"", // Croatian
# endif
#endif
	},
	{ NULL }
};

