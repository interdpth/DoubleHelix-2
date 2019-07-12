
//{{BLOCK(dmansbg)

//======================================================================
//
//	dmansbg, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 753 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 24096 + 2048 = 26176
//
//	Time-stamp: 2018-11-29, 18:28:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DMANSBG_H
#define GRIT_DMANSBG_H

#define dmansbgTilesLen 24096
extern const unsigned char dmansbgTiles[24096];

#define dmansbgMapLen 2048
extern const unsigned short dmansbgMap[1024];

#define dmansbgPalLen 32
extern const unsigned short dmansbgPal[16];

#endif // GRIT_DMANSBG_H

//}}BLOCK(dmansbg)
