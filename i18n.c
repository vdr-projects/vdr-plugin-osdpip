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
		"", // Português
		"", // Français
		"", // Norsk
		"Kuva kuvassa -toiminto (OSD)", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Kuva kuvassa", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Leikkaa vasemmalta", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Leikkaa oikealta", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Leikkaa ylhäältä", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Leikkaa alhaalta", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Värisyvyys", // Suomi
		"", // Polski
		"", // Español
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
		"Größe", // Deutsch
		"", // Slovenski
		"", // Italiano
		"", // Nederlands
		"", // Português
		"", // Français
		"", // Norsk
		"Koko", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Näytä kehykset", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Hylkää kehyksiä", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Invertoi FFMPEG-kuva", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Näytä kuvatieto", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Kuvatiedon leveys", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Kuvatiedon sijainti", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"vasen yläreuna", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"oikea yläreuna", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"vasen alareuna", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"oikea alareuna", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"harmaasävy (16)", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"harmaasävy (256)", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"väri (128, vaihtuva)", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Vaakasijainti", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"Pystysijainti", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"automaattisesti", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"ei", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"1 kehys", // Suomi
		"", // Polski
		"", // Español
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
		"", // Português
		"", // Français
		"", // Norsk
		"2 kehystä", // Suomi
		"", // Polski
		"", // Español
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

