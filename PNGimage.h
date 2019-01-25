
//{{BLOCK(PNGimage)

//======================================================================
//
//	PNGimage, 2048x1536@4, 
//	+ palette 256 entries, not compressed
//	+ 7791 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 256x192 
//	Total size: 512 + 249312 + 98304 = 348128
//
//	Time-stamp: 2018-11-25, 19:16:30
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PNGIMAGE_H
#define GRIT_PNGIMAGE_H

#define PNGimageTilesLen 249312
extern const unsigned char PNGimageTiles[249312];

#define PNGimageMapLen 98304
extern const unsigned short PNGimageMap[49152];

#define PNGimagePalLen 512
extern const unsigned short PNGimagePal[256];

#endif // GRIT_PNGIMAGE_H

//}}BLOCK(PNGimage)
